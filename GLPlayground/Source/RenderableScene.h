#pragma once
#include "Mesh.h"
#include <memory>
#include "GLRenderer.h"

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

private:
	std::vector<std::shared_ptr<Mesh>> Meshes;
	GLRenderer & Renderer;

	RenderableMeshLocation FirstFree = 0;

	GLuint UniformMatricesBufferID;
	UniformMatrices UniformMatricesBuffer;
};

