#pragma once
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/GLRenderer.h"
#include "Renderer/Font.h"

struct TextRenderData
{
	std::unique_ptr<Mesh> TextMesh;
	float Duration;
	bool Infinite = false;

	TextRenderData(std::unique_ptr<Mesh> && MeshToReplace, float Duration, bool Infinite);
	TextRenderData(TextRenderData && RenderData);
	TextRenderData();
	TextRenderData & operator=(TextRenderData && RenderDataToReplace);
};

class FontRenderer
{
public:
	FontRenderer();
	FontRenderer(FontRenderer && RendererToReplace);
	FontRenderer & operator=(FontRenderer && RendererToReplace);

	bool Init(const std::string & FontName, WindowInfo Info);
	void Render(GLRenderer & Renderer, float DeltaTime);
	void DeInit();

	std::unique_ptr<Mesh> CreateMeshFromText(TextInfo Text);
	void RenderText(TextInfo Info, float Duration, bool Infinite = false);

private:

	void * AllocatedChars;

	size_t FontTextureID;

	Material FontMaterial;

	uint32_t UniformMatricesBufferID;

	int BitMapWidth = 1024;
	int BitMapHeight = 1024;
	float Scale;

	std::vector<TextRenderData> RenderDatas;

	UniformMatrices UniformMatricesBuffer;
};