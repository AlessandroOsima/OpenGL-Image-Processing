#include "stdafx.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "FontRenderer.h"

#include <GL/glew.h>
#include "Managers/TextureManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/ShaderManager.h"
#include "Logger/Asserts.h"

#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>
#include <memory>
#include "Renderer/GLUtilities.h"

FontRenderer::FontRenderer() : Scale(30), RenderDatas()
{

}

FontRenderer::FontRenderer(FontRenderer && RendererToReplace) :
	Scale(RendererToReplace.Scale),
	BitMapHeight(RendererToReplace.BitMapHeight),
	BitMapWidth(RendererToReplace.BitMapWidth),
	AllocatedChars(RendererToReplace.AllocatedChars),
	FontTextureID(RendererToReplace.FontTextureID),
	FontMaterial(std::move(RendererToReplace.FontMaterial)),
	UniformMatricesBufferID(RendererToReplace.UniformMatricesBufferID),
	UniformMatricesBuffer(std::move(RendererToReplace.UniformMatricesBuffer)),
	RenderDatas(std::move(RendererToReplace.RenderDatas))
{
	RendererToReplace.AllocatedChars = 0;
	RendererToReplace.FontTextureID = 0;
	RendererToReplace.UniformMatricesBufferID = 0;
}

FontRenderer & FontRenderer::operator=(FontRenderer && RendererToReplace)
{
	Scale = RendererToReplace.Scale;
	BitMapHeight = RendererToReplace.BitMapHeight;
	BitMapWidth = RendererToReplace.BitMapWidth;

	AllocatedChars = RendererToReplace.AllocatedChars;
	FontTextureID = RendererToReplace.FontTextureID;
	FontMaterial = std::move(RendererToReplace.FontMaterial);
	UniformMatricesBufferID = RendererToReplace.UniformMatricesBufferID;
	UniformMatricesBuffer = std::move(RendererToReplace.UniformMatricesBuffer);
	RenderDatas = std::move(RendererToReplace.RenderDatas);

	RendererToReplace.AllocatedChars = 0;
	RendererToReplace.FontTextureID = 0;
	RendererToReplace.UniformMatricesBufferID = 0;

	return *this;
}

bool FontRenderer::Init(const std::string & FontName, WindowInfo Info)
{

	auto bitmap = std::make_unique<unsigned char[]>(BitMapWidth * BitMapHeight);

	FILE * fontFile = fopen((ResourceManager::GetFontsFolder() + FontName).c_str(), "rb");

	if (!fontFile)
	{
		std::stringstream stream;
		stream << "Font file " << FontName << " cannot be opened for read" << std::ends;

		Logger::GetLogger().LogString(stream.str(), LogType::ERROR);
		return false;
	}

	fseek(fontFile, 0, SEEK_END);

	long size = ftell(fontFile);

	fseek(fontFile, 0, SEEK_SET);

	unsigned char * data = new unsigned char[size];

	size_t readBits = fread(data, 1, size, fontFile);
	AllocatedChars = new stbtt_bakedchar[96];

	fclose(fontFile);

	int res = stbtt_BakeFontBitmap(data, 0, Scale, bitmap.get(), BitMapWidth, BitMapHeight, 32, 96, (stbtt_bakedchar*)AllocatedChars);


	delete[] data;

	bool Created = false;
	bool Found = false;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	Created = TextureManager::GetTextureManager().CreateTexture(FontName, GL_R8, BitMapWidth, BitMapHeight, FontTextureID);
	Texture fontTexture = TextureManager::GetTextureManager().GetTextureFromID(FontTextureID, Found);

	if (Created && Found)
	{
		fontTexture.SetImageData(0, 0, 0, BitMapWidth, BitMapHeight, GL_RED, GL_UNSIGNED_BYTE, bitmap.get());
	}
	else
	{
		delete[] AllocatedChars;

		std::stringstream stream;
		stream << "Creating texture for font " << FontName << " failed" << std::ends;
		Logger::GetLogger().LogString(stream.str(), LogType::ERROR);
		return false;
	}


	size_t fontShader;
	if (ShaderManager::GetShaderManager().CreateShader("font", "font.vs", "font.fs", fontShader))
	{

		FontMaterial.DiffuseTexture = FontTextureID;
		FontMaterial.Program = fontShader;
		FontMaterial.CreateObjects();
	}
	else
	{
		std::stringstream stream;
		stream << "Creating material for font " << FontName << " failed" << std::ends;
		Logger::GetLogger().LogString(stream.str(), LogType::ERROR);

		return false;
	}

	glCreateBuffers(1, &UniformMatricesBufferID);

	glm::mat4 model = glm::mat4(1);
	//model = glm::scale(glm::mat4(1), glm::vec3(1, 1, 1));
	//model = glm::translate(model, glm::vec3(Info.Width/2, Info.Height/2, 0));

	glm::mat4 projection = glm::ortho((float)0, (float)Info.Width, (float)0, (float)Info.Height, 0.f, 1.f);

	UniformMatricesBuffer = {projection, glm::mat4(1), model};

	glCheckFunction(glNamedBufferStorage(UniformMatricesBufferID, sizeof(UniformMatrices), &UniformMatricesBuffer, GL_DYNAMIC_STORAGE_BIT));

	return true;
}

void FontRenderer::Render(GLRenderer & Renderer, float DeltaTime)
{


	bool Found;

	ShaderProgram & fontProgram = ShaderManager::GetShaderManager().GetShader(FontMaterial.Program, Found);
	fontProgram.BindBufferToUniform(UniformMatricesBufferID, 0, "Matrices");

	AssertWithMessage(Found, "No shader found");

	FontMaterial.Bind();

	for (auto & renderData : RenderDatas)
	{
		renderData.TextMesh->Bind();

		Renderer.DrawMesh(*renderData.TextMesh);

		renderData.TextMesh->Unbind();
	}

	FontMaterial.UnBind();

	for (int i = 0; i < RenderDatas.size(); i++)
	{
		if (RenderDatas[i].Infinite)
		{
			continue;
		}

		RenderDatas[i].Duration = RenderDatas[i].Duration - DeltaTime;

		if (RenderDatas[i].Duration <= 0)
		{
			RenderDatas.erase(RenderDatas.begin() + i);
		}
	}

}

void FontRenderer::DeInit()
{
	if (AllocatedChars)
	{
		delete[] AllocatedChars;
	}

	TextureManager::GetTextureManager().DestroyTexture(FontTextureID);
	
	glDeleteBuffers(1, &UniformMatricesBufferID);

}

std::unique_ptr<Mesh> FontRenderer::CreateMeshFromText(TextInfo Text)
{
	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();

	stbtt_bakedchar* chars = (stbtt_bakedchar*)(AllocatedChars);
	Index n = 0;

	float y = -Text.Position.y;
	float x = Text.Position.x;

	std::vector<Vertex> Vertices;
	std::vector<Index> Indices;

	for (char c : Text.Text)
	{
		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(chars, BitMapWidth, BitMapHeight, c - 32, &x, &y, &q, 0);

		float ZPos = Text.Position.z;

		Vertex v[4];
		v[0].Position = glm::vec3(q.x0, -q.y0, ZPos);
		v[0].Color = Text.Color;
		v[0].UV = glm::vec2(q.s0, q.t0);

		v[1].Position = glm::vec3(q.x1, -q.y0, ZPos);
		v[1].Color = Text.Color;
		v[1].UV = glm::vec2(q.s1, q.t0);

		v[2].Position = glm::vec3(q.x1, -q.y1, ZPos);
		v[2].Color = Text.Color;
		v[2].UV = glm::vec2(q.s1, q.t1);

		v[3].Position = glm::vec3(q.x0, -q.y1, ZPos);
		v[3].Color = Text.Color;
		v[3].UV = glm::vec2(q.s0, q.t1);

		Vertices.push_back(v[0]);
		Vertices.push_back(v[1]);
		Vertices.push_back(v[2]);
		Vertices.push_back(v[3]);

		Indices.push_back(n + 0);
		Indices.push_back(n + 1);
		Indices.push_back(n + 3);
		Indices.push_back(n + 1);
		Indices.push_back(n + 2);
		Indices.push_back(n + 3);

		n += 4;
	}

	mesh->GenerateMeshData(Vertices, Indices);

	return mesh;
}

void FontRenderer::RenderText(TextInfo Info, float Duration, bool Infinite /*= false*/)
{
	TextRenderData renderData { std::move(CreateMeshFromText(Info)), Duration, Infinite};
	RenderDatas.push_back(std::move(renderData));
}

TextRenderData::TextRenderData(TextRenderData && RenderData)
{
	TextMesh = std::move(RenderData.TextMesh);
	Duration = RenderData.Duration;
	Infinite = RenderData.Infinite;
}

TextRenderData::TextRenderData(std::unique_ptr<Mesh> && MeshToReplace, float Duration, bool Infinite) :
	TextMesh(std::move(MeshToReplace)),
	Duration(Duration),
	Infinite(Infinite)
{

}

TextRenderData::TextRenderData() :
	TextMesh(nullptr),
	Duration(0),
	Infinite(false)
{

}

TextRenderData & TextRenderData::operator=(TextRenderData && RenderDataToReplace)
{
	TextMesh = std::move(RenderDataToReplace.TextMesh);
	Duration = RenderDataToReplace.Duration;
	Infinite = RenderDataToReplace.Infinite;

	return *this;
}
