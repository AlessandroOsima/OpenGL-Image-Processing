#pragma once
#include "Renderer/Mesh.h"
#include <memory>
#include "Renderer/GLRenderer.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"
#include <limits>

using RenderableMeshLocation = uint64_t;
using RenderablPassLocation = uint64_t;

constexpr RenderablPassLocation InvalidRenderpassIndex = std::numeric_limits<size_t>::max();

struct UniformMatrices
{
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
};

struct MeshStorageInfo
{
	std::shared_ptr<Mesh> Mesh;
	RenderablPassLocation Location = InvalidRenderpassIndex;
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
	RenderableMeshLocation AddMeshMultipass(std::shared_ptr<Mesh> MeshToAdd, RenderPassGroup && PassesToAdd);
	
	void RemoveMesh(RenderableMeshLocation Location);

	RenderablPassLocation AddRenderPassGroup(RenderPassGroup && PassesToAdd);
	void RemoveRenderPassGroup(RenderablPassLocation Location);

	bool LinkMeshMultiPass(RenderableMeshLocation Mesh, RenderablPassLocation Pass);

	inline void SetProjection(const glm::mat4 & Projection)
	{
		CurrentProjection = Projection;
	}

	inline void SetView(const glm::mat4 & View)
	{
		CurrentView = View;
	}

private:
	std::vector<MeshStorageInfo> Meshes;
	std::vector<RenderPassGroup> Passes;

	GLRenderer & Renderer;

	RenderableMeshLocation FirstRenderableMeshFree = 0;
	RenderablPassLocation  FirstRenderPassFree = 0;

	uint32_t UniformMatricesBufferID;
	UniformMatrices UniformMatricesBuffer;

	glm::mat4 CurrentView;
	glm::mat4 CurrentProjection;

	Framebuffer OffscreenFramebuffer;
	size_t OffscreenFBColorAttachment;

	Material BaseMaterial;
};

