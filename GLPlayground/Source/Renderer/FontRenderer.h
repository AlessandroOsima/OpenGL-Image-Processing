#pragma once
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/GLRenderer.h"

struct TextInfo
{
	std::string Text;
	glm::vec4 Color;
	glm::vec3 Position;
};

class FontRenderer
{
public:
	FontRenderer();
	FontRenderer(FontRenderer && RendererToReplace);
	FontRenderer & operator=(FontRenderer && RendererToReplace);

	bool Init(const std::string & FontName, WindowInfo Info);
	void Render(GLRenderer & Renderer, std::vector<std::unique_ptr<Mesh>> & TextMeshes);
	void DeInit();

	std::unique_ptr<Mesh> CreateMeshFromText(TextInfo Text);

private:

	void * AllocatedChars;

	size_t FontTextureID;

	Material FontMaterial;

	uint32_t UniformMatricesBufferID;

	int BitMapWidth = 1024;
	int BitMapHeight = 1024;
	float Scale;

	UniformMatrices UniformMatricesBuffer;
};