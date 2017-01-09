#include "RenderableScene.h"
#include "Managers/ShaderManager.h"
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
}

void RenderableScene::RenderScene()
{
	Renderer.Clear();

	for (auto & mesh : Meshes)
	{
		mesh->BindMesh();

		glNamedBufferSubData(UniformMatricesBufferID, sizeof(glm::mat4) * 2, sizeof(glm::mat4), &mesh->GetModel());

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

