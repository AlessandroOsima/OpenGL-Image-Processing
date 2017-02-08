#pragma once
#include "Renderer/FontRenderer.h"
#include "Renderer/Mesh.h"
#include <memory>
#include "Renderer/GLRenderer.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderPass.h"
#include <limits>


using RenderableMeshLocation = uint64_t;
using RenderablPassLocation = uint64_t;

constexpr RenderablPassLocation InvalidRenderpassIndex = std::numeric_limits<size_t>::max();


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

	void RenderScene(float DeltaTime);

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

	size_t CreateFontRenderer(std::string FontName);

	void DestroyFontRenderer(size_t FontRendererID);
	
	inline FontRenderer * GetFontRenderer(size_t FontRendererID)
	{
		std::map<std::size_t, FontRenderer>::iterator it = FontRenderers.find(FontRendererID);

		if (it == FontRenderers.end())
		{
			return nullptr;
		}
		
		return &FontRenderers[FontRendererID];
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

	std::map<size_t, FontRenderer> FontRenderers;
};

