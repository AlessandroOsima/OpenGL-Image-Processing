#pragma once
#include "Renderer/Mesh.h"
#include <memory>
#include "Renderer/GLRenderer.h"

using RenderableMeshLocation = uint64_t;

struct UniformMatrices
{
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
};

class RenderableScene
{

public:
	RenderableScene(GLRenderer & Renderer);
	~RenderableScene();

	void Initialize();

	void RenderScene();

	void DeInitialize();

	RenderableMeshLocation AddMesh(std::shared_ptr<Mesh> MeshToAdd);
	void RemoveMesh(RenderableMeshLocation Location);

	inline void SetProjection(const glm::mat4 & Projection)
	{
		CurrentProjection = Projection;
	}

	inline void SetView(const glm::mat4 & View)
	{
		CurrentView = View;
	}

private:
	std::vector<std::shared_ptr<Mesh>> Meshes;
	GLRenderer & Renderer;

	RenderableMeshLocation FirstFree = 0;

	uint32_t UniformMatricesBufferID;
	UniformMatrices UniformMatricesBuffer;

	glm::mat4 CurrentView;
	glm::mat4 CurrentProjection;
};

