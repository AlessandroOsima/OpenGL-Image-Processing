#pragma once
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/GLRenderer.h"

class FontRenderer
{
public:
	FontRenderer();
	void Init(const std::string & FontName, WindowInfo Info);
	void Render(GLRenderer & Renderer);
	void DeInit();

private:
	void * AllocatedChars;

	size_t FontTextureID;

	Material FontMaterial;
	Mesh Quad;

	uint32_t UniformMatricesBufferID;

	const int BitMapWidth = 512;
	const int BitMapHeight = 512;
	const float Scale = 100;

	UniformMatrices UniformMatricesBuffer;
};