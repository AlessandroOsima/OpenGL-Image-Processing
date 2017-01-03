#include "RenderableScene.h"
#include "ShaderManager.h"
#include <limits>

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
	glBindBuffer(GL_UNIFORM_BUFFER, UniformMatricesBufferID);
	glNamedBufferStorage(UniformMatricesBufferID, sizeof(UniformMatricesBuffer), &UniformMatricesBuffer, GL_DYNAMIC_STORAGE_BIT);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RenderableScene::RenderScene()
{
	Renderer.Clear();

	for (auto & mesh : Meshes)
	{
		//mesh->GetMaterial().Program
		bool ShaderFound = false;
		auto & Shader = ShaderManager::GetShaderManager().GetShader(mesh->GetMaterial().Program, ShaderFound);

		if (ShaderFound)
		{
			unsigned int shaderUniformID = Shader.GetUniformBlockIndex("Matrices");

			if (shaderUniformID != GL_INVALID_INDEX)
			{
				glBindBuffer(GL_UNIFORM_BUFFER, UniformMatricesBufferID);

				glUniformBlockBinding(Shader.GetShaderProgramID(), shaderUniformID, 0);

				glBindBufferBase( GL_UNIFORM_BUFFER, 0, UniformMatricesBufferID);

				//glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
		}

		mesh->BindMesh();
		Renderer.DrawMesh(*mesh);
		mesh->UnbindMesh();
	}

	Renderer.Present();
}

void RenderableScene::DeInitialize()
{

}

RenderableMeshLocation RenderableScene::AddMesh(std::shared_ptr<Mesh> MeshToAdd)
{
	if (FirstFree >= Meshes.size())
	{
		Meshes.push_back(MeshToAdd);
	}
	else
	{
		Meshes[FirstFree] = MeshToAdd;
	}
	
	FirstFree = Meshes.size();
	return FirstFree - 1;
}

void RenderableScene::RemoveMesh(RenderableMeshLocation Location)
{
	if (Location >= Meshes.size())
	{
		return;
	}

	FirstFree = Location;
	Meshes.erase(Meshes.begin() + Location);
}
