#include "stdafx.h"
#include "RenderableScene.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include <limits>
#include "GLUtilities.h"
#include <glm/gtc/matrix_transform.hpp>

RenderableScene::RenderableScene(GLRenderer & Renderer) : Renderer(Renderer), BaseMaterial(0,0)
{
}

RenderableScene::~RenderableScene()
{
	glDeleteBuffers(1, &UniformMatricesBufferID);
}

void RenderableScene::Initialize()
{
	glCreateBuffers(1, &UniformMatricesBufferID);
	glNamedBufferStorage(UniformMatricesBufferID, sizeof(UniformMatricesBuffer), &UniformMatricesBuffer, GL_DYNAMIC_STORAGE_BIT);

	WindowInfo info;
	Renderer.GetCurrentWindowInfo(info);

	CurrentProjection = glm::ortho((float)0, (float)info.Width, (float)0, (float)info.Height, 0.f, 1.f);

	ShaderManager::GetShaderManager().OnShaderAdded = [&](size_t HashedProgram)
	{
		constexpr unsigned int MatricesBindingLocation = 0;
		constexpr char * MatricesUniformName = "Matrices";

		bool found = false;
		ShaderProgram & program = ShaderManager::GetShaderManager().GetShader(HashedProgram, found);

		if (found)
		{
			program.BindBufferToUniform(UniformMatricesBufferID, MatricesBindingLocation, MatricesUniformName);
		}

		glNamedBufferSubData(UniformMatricesBufferID, 0, sizeof(glm::mat4), &CurrentProjection);

		glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4), sizeof(glm::mat4), &CurrentView);
	};

	size_t baseShaderHash;
	if (ShaderManager::GetShaderManager().CreateShader("base", "base.vs", "base.fs", baseShaderHash))
	{
		BaseMaterial = { 0, baseShaderHash };
		BaseMaterial.CreateObjects();
	}
	else
	{
		Logger::GetLogger().LogString("Unable to create base material for Renderable Scene", LogType::ERROR);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void RenderableScene::RenderScene(float DeltaTime)
{

	WindowInfo info;
	Renderer.GetCurrentWindowInfo(info);

	Renderer.Clear();


	for (auto & mesh : Meshes)
	{
		//Render with only one pass
		if (mesh.Location == InvalidRenderpassIndex)
		{
			Logger::GetLogger().LogString("mesh does not have a valid renderpass", LogType::ERROR);
		}
		else
		{

			RenderPassGroup & group = Passes[mesh.Location];
			
			bool found;

			Texture & colorTarget = TextureManager::GetTextureManager().GetTextureFromID(group.GetOffscreenTexture(), found);
			Texture & textureAttachment = TextureManager::GetTextureManager().GetTextureFromID(group.GetAttachmentTexture(), found);

			size_t attachmentTexture = group.RenderPasses[0].GetMaterial().DiffuseTexture;
			size_t original;

			//Do all the addictive passes
			for (int i = 0; i < group.RenderPasses.size(); i++)
			{
				//The viewport is the size of the texture we are rendering
				glViewport(0, 0, colorTarget.GetTextureInfo().Width, colorTarget.GetTextureInfo().Height);

				//Bind the attachment texture to the framebuffer as a color target
				OffscreenFramebuffer.BindTextureToFramebuffer(colorTarget, FrameBufferAttachmentType::COLOR);
				OffscreenFramebuffer.BindFramebuffer(FramebufferBindType::FRAMEBUFFER);

				//Setup the shaders for the current RenderPass
				Renderer.Clear();

				mesh.Mesh->Bind();

				if (group.RenderPasses[i].UsePreviousPassAsAttachment)
				{
					original = group.RenderPasses[i].GetMaterial().DiffuseTexture;
					group.RenderPasses[i].GetMaterial().DiffuseTexture = attachmentTexture;
				}

				group.RenderPasses[i].GetMaterial().Bind();
				group.RenderPasses[i].BindUniforms();

				bool ShaderFound;

				ShaderProgram & program = ShaderManager::GetShaderManager().GetShader(group.RenderPasses[i].GetMaterial().Program, ShaderFound);
			
				AssertWithMessage(ShaderFound, "Unable to find shader");

				constexpr unsigned int MatricesBindingLocation = 0;
				constexpr char * MatricesUniformName = "Matrices";
				program.BindBufferToUniform(UniformMatricesBufferID, MatricesBindingLocation, MatricesUniformName);

				//When we render to an offscreen texture the model, view and projection matrices are all identity matrices because there is no need for transformations
				glNamedBufferSubData(UniformMatricesBufferID, 0, sizeof(glm::mat4), &glm::mat4(1));
				glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4), sizeof(glm::mat4), &glm::mat4(1));
				glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &glm::mat4(1));

				Renderer.DrawMesh(*mesh.Mesh);

				group.RenderPasses[i].GetMaterial().UnBind();
				mesh.Mesh->Unbind();

				if (group.RenderPasses[i].UsePreviousPassAsAttachment)
				{
					//Copy the framebuffer color target to another texture to use as input for the fragment shader in the RenderPass
					glBindTexture(GL_TEXTURE_2D, textureAttachment.GetID());
					glCheckFunction(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, colorTarget.GetTextureInfo().Width, colorTarget.GetTextureInfo().Height));
					glBindTexture(GL_TEXTURE_2D, 0);

					group.RenderPasses[i].GetMaterial().DiffuseTexture = original;
					attachmentTexture = group.GetAttachmentTexture();
				}

				if (group.RenderPasses[i].RenderOnMainFramebuffer)
				{
					//Render the mesh in the main framebuffer
					OffscreenFramebuffer.UnbindFramebufferAttachment(FrameBufferAttachmentType::COLOR);
					OffscreenFramebuffer.UnBindFramebuffer();

					glViewport(0, 0, info.Width, info.Height);

					mesh.Mesh->Bind();

					BaseMaterial.DiffuseTexture = attachmentTexture;
					BaseMaterial.Bind();
					
					//Since we are rendering on the main window we need to use the correct matrices
					glNamedBufferSubData(UniformMatricesBufferID, 0, sizeof(glm::mat4), &CurrentProjection);
					glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4), sizeof(glm::mat4), &CurrentView);
					glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &mesh.Mesh->GetModel());

					Renderer.DrawMesh(*mesh.Mesh);

					BaseMaterial.UnBind();
					mesh.Mesh->Unbind();
				
				}
			}

			OffscreenFramebuffer.UnbindFramebufferAttachment(FrameBufferAttachmentType::COLOR);
			OffscreenFramebuffer.UnBindFramebuffer();
		}
	}

	OffscreenFramebuffer.UnbindFramebufferAttachment(FrameBufferAttachmentType::COLOR);
	OffscreenFramebuffer.UnBindFramebuffer();

	//Restore the viewport and render all the TextRenderers
	glViewport(0, 0, info.Width, info.Height);


	for (auto & fontRenderer : FontRenderers)
	{
		fontRenderer.second.Render(Renderer, DeltaTime);
	}

	Renderer.Present();
}

void RenderableScene::DeInitialize()
{
	for (auto & fontRenderer : FontRenderers)
	{
		fontRenderer.second.DeInit();
	}

	BaseMaterial.RemoveObjects();
}

RenderableMeshLocation RenderableScene::AddMesh(std::shared_ptr<Mesh> MeshToAdd)
{
	MeshStorageInfo info;  
	info.Mesh = MeshToAdd;
	info.Location = InvalidRenderpassIndex;

	if (FirstRenderableMeshFree >= Meshes.size())
	{
		Meshes.push_back(info);
	}
	else
	{
		Meshes[FirstRenderableMeshFree] = info;
	}
	
	FirstRenderableMeshFree = Meshes.size();
	return FirstRenderableMeshFree - 1;
}

RenderableMeshLocation RenderableScene::AddMeshMultipass(std::shared_ptr<Mesh> MeshToAdd, RenderPassGroup && PassesToAdd)
{
	RenderablPassLocation passLoc = AddRenderPassGroup(std::move(PassesToAdd));
	RenderableMeshLocation meshLoc = AddMesh(MeshToAdd);
	
	LinkMeshMultiPass(meshLoc, passLoc);

	return meshLoc;
}

void RenderableScene::RemoveMesh(RenderableMeshLocation Location)
{
	if (Location >= Meshes.size())
	{
		return;
	}

	FirstRenderableMeshFree = Location;
	Meshes.erase(Meshes.begin() + Location);
}


RenderablPassLocation RenderableScene::AddRenderPassGroup(RenderPassGroup && PassesToAdd)
{
	if (FirstRenderPassFree >= Passes.size())
	{
		Passes.push_back(std::move(PassesToAdd));
		Passes[Passes.size() - 1].Init();
	}
	else
	{
		Passes[FirstRenderPassFree] = std::move(PassesToAdd);
		Passes[FirstRenderPassFree].Init();
	}

	

	FirstRenderPassFree = Passes.size();
	return FirstRenderPassFree - 1;
}

void RenderableScene::RemoveRenderPassGroup(RenderablPassLocation Location)
{
	if (Location >= Passes.size())
	{
		return;
	}

	FirstRenderPassFree = Location;
	Passes[Location].DeInit();

	Passes.erase(Passes.begin() + Location);
}

bool RenderableScene::LinkMeshMultiPass(RenderableMeshLocation Mesh, RenderablPassLocation Pass)
{
	if (Mesh >= Meshes.size() && Pass >= Passes.size())
	{
		return false;
	}

	Meshes[Mesh].Location = Pass;

	return true;
}

size_t RenderableScene::CreateFontRenderer(std::string FontName)
{
	size_t hash = std::hash<std::string>{}(FontName);

	std::map<std::size_t, FontRenderer>::iterator it = FontRenderers.find(hash);

	if (it != FontRenderers.end())
	{
		return hash;
	}

	FontRenderer FRenderer;

	WindowInfo info;
	Renderer.GetCurrentWindowInfo(info);

	FRenderer.Init(FontName, info);

	FontRenderers[hash] = std::move(FRenderer);

	return hash;
}

void RenderableScene::DestroyFontRenderer(size_t FontRendererID)
{
	FontRenderer * fRenderer = GetFontRenderer(FontRendererID);

	if (fRenderer)
	{
		FontRenderers.erase(FontRendererID);
	}
}
