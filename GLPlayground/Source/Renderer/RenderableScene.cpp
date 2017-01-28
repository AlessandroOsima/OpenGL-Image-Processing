#include "RenderableScene.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include <limits>
#include "GLUtilities.h"
#include <glm/gtc/matrix_transform.hpp>


RenderableScene::RenderableScene(GLRenderer & Renderer) : Renderer(Renderer)
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

	CurrentProjection = glm::ortho((float)0, (float)info.Width, (float)0, (float)info.Height, 0.f , 1.f);

	ShaderManager::GetShaderManager().OnShaderAdded = [&](size_t HashedProgram)
	{
		static constexpr unsigned int MatricesBindingLocation = 0;
		static constexpr char * MatricesUniformName = "Matrices";

		bool found = false;
		ShaderProgram & program = ShaderManager::GetShaderManager().GetShader(HashedProgram, found);

		if (found)
		{
			program.BindBufferToUniform(UniformMatricesBufferID, MatricesBindingLocation, MatricesUniformName);
		}

		glNamedBufferSubData( UniformMatricesBufferID, 0, sizeof(glm::mat4), &CurrentProjection);

		glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4), sizeof(glm::mat4), &CurrentView);
	};



	/*TextureManager::GetTextureManager().CreateTexture("OffscreenColorAttachment", info.Width, info.Height, OffscreenFBColorAttachment);
	bool found = false;
	OffscreenFramebuffer.BindTextureToFramebuffer(TextureManager::GetTextureManager().GetTextureFromID(OffscreenFBColorAttachment, found), FrameBufferAttachmentType::COLOR);
	
	OffscreenFramebuffer.BindFramebuffer();*/
}

void RenderableScene::RenderScene()
{
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

			//Do base pass
			RenderPassGroup & group = Passes[mesh.Location];
			
			bool found;

			Texture & colorTarget = TextureManager::GetTextureManager().GetTextureFromID(group.GetOffscreenTexture(), found);
			Texture & textureAttachment = TextureManager::GetTextureManager().GetTextureFromID(group.GetAttachmentTexture(), found);

			size_t attachmentTexture = group.RenderPasses[0].GetMaterial().DiffuseTexture;
			size_t original;

			//Do all the addictive passes
			for (int i = 0; i < group.RenderPasses.size(); i++)
			{

				glViewport(0, 0, colorTarget.GetTextureInfo().Width, colorTarget.GetTextureInfo().Height);

				OffscreenFramebuffer.BindTextureToFramebuffer(colorTarget, FrameBufferAttachmentType::COLOR);
				OffscreenFramebuffer.BindFramebuffer(FramebufferBindType::FRAMEBUFFER);

				Renderer.Clear();

				mesh.Mesh->BindMesh();

				//We should load the proper attachment texture here

				if (group.RenderPasses[i].UsePreviousPassAsAttachment)
				{
					original = group.RenderPasses[i].GetMaterial().DiffuseTexture;
					group.RenderPasses[i].GetMaterial().DiffuseTexture = attachmentTexture;
				}

				group.RenderPasses[i].GetMaterial().Bind();
				group.RenderPasses[i].BindUniforms();

				glNamedBufferSubData(UniformMatricesBufferID, 0, sizeof(glm::mat4), &glm::mat4(1));
				glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4), sizeof(glm::mat4), &glm::mat4(1));
				glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &glm::mat4(1));

				Renderer.DrawMesh(*mesh.Mesh);

				group.RenderPasses[i].GetMaterial().UnBind();
				mesh.Mesh->UnbindMesh();

				glBindTexture(GL_TEXTURE_2D, textureAttachment.GetID());
				glCheckFunction(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, colorTarget.GetTextureInfo().Width, colorTarget.GetTextureInfo().Height));
				glBindTexture(GL_TEXTURE_2D, 0);

				if (group.RenderPasses[i].UsePreviousPassAsAttachment)
				{
					group.RenderPasses[i].GetMaterial().DiffuseTexture = original;
					attachmentTexture = group.GetAttachmentTexture();
				}

				if (group.RenderPasses[i].RenderOnMainFramebuffer)
				{
					OffscreenFramebuffer.UnBindFramebuffer();

					OffscreenFramebuffer.BindFramebuffer(FramebufferBindType::READ);

					glBlitFramebuffer(0, 0, colorTarget.GetTextureInfo().Width, colorTarget.GetTextureInfo().Height, 0, 0, colorTarget.GetTextureInfo().Width, colorTarget.GetTextureInfo().Height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
				}
			}

			OffscreenFramebuffer.UnbindFramebufferAttachment(FrameBufferAttachmentType::COLOR);
			OffscreenFramebuffer.UnBindFramebuffer();
		}
	}

	//Render on window framebuffer 

	Renderer.Present();
}

void RenderableScene::DeInitialize()
{

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
		Passes.push_back(PassesToAdd);
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
