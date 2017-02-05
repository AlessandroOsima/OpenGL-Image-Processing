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

FontRenderer::FontRenderer() : Scale(50)
{

}

void FontRenderer::Init(const std::string & FontName, WindowInfo Info)
{

	auto bitmap = std::make_unique<unsigned char[]>(BitMapWidth * BitMapHeight);

	FILE * fontFile = fopen((ResourceManager::GetFontsFolder() + FontName).c_str(), "rb");

	if (!fontFile)
	{
		std::stringstream stream;
		stream << "Font file " << FontName << " cannot be opened for read" << std::ends;

		Logger::GetLogger().LogString(stream.str(), LogType::ERROR);
		return;
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
	}


	size_t fontShader;
	if (ShaderManager::GetShaderManager().CreateShader("font", "font.vs", "font.fs", fontShader))
	{

		FontMaterial.DiffuseTexture = FontTextureID;
		FontMaterial.Program = fontShader;
		FontMaterial.CreateObjects();
	}

	//Quad.GenerateMeshData({  //Vertices
	//	{ glm::vec3(1.f, 1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(1,1) }, //0
	//	{ glm::vec3(1.f,  -1.f, -0.1f), glm::vec4(0, 1, 0, 1), glm::vec2(1,0) },  //1
	//	{ glm::vec3(-1.f, 1.f, -0.1f), glm::vec4(1, 0, 0, 1), glm::vec2(0,1) }, //2
	//	{ glm::vec3(-1.f,   -1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(0,0) }  //3
	//},
	//	//Indices
	//{
	//	0,
	//	1,
	//	2,
	//	2,
	//	1,
	//	3
	//});


	glCreateBuffers(1, &UniformMatricesBufferID);


	glm::mat4 model;
	model = glm::scale(glm::mat4(1), glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(Info.Width/2, Info.Height/2, 0));

	glm::mat4 projection = glm::ortho((float)0, (float)Info.Width, (float)0, (float)Info.Height, 0.f, 1.f);

	UniformMatricesBuffer = {projection, glm::mat4(1), model};

	glCheckFunction(glNamedBufferStorage(UniformMatricesBufferID, sizeof(UniformMatrices), &UniformMatricesBuffer, GL_DYNAMIC_STORAGE_BIT));
}

void FontRenderer::Render(GLRenderer & Renderer)
{

	//{ glm::vec3(1.f, 1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(1, 1) }, //0
	//{ glm::vec3(1.f,  -1.f, -0.1f), glm::vec4(0, 1, 0, 1), glm::vec2(1,0) },  //1
	//{ glm::vec3(-1.f, 1.f, -0.1f), glm::vec4(1, 0, 0, 1), glm::vec2(0,1) }, //2
	//{ glm::vec3(-1.f,   -1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(0,0) }  //3


	std::string text = "Hello World";
	bool Found;

	ShaderProgram & fontProgram = ShaderManager::GetShaderManager().GetShader(FontMaterial.Program, Found);
	fontProgram.BindBufferToUniform(UniformMatricesBufferID, 0, "Matrices");

	AssertWithMessage(Found, "No shader found");

	FontMaterial.Bind();

	float y = 0;
	float x = 0;

	Mesh Quad;

	FillMeshFromText(Quad, text);

	Quad.Bind();

	Renderer.DrawMesh(Quad);

	Quad.Unbind();

	FontMaterial.UnBind();

}

void FontRenderer::DeInit()
{
	delete[] AllocatedChars;
	TextureManager::GetTextureManager().DestroyTexture(FontTextureID);
}

void FontRenderer::FillMeshFromText(Mesh & MesshToFill, std::string Text)
{

	stbtt_bakedchar* chars = (stbtt_bakedchar*)(AllocatedChars);
	Index n = 0;

	float y = 0;
	float x = 0;

	std::vector<Vertex> Vertices;
	std::vector<Index> Indices;

	for (char c : Text)
	{
		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(chars, BitMapWidth, BitMapHeight, c - 32, &x, &y, &q, 0);

		Vertex v[4];
		v[0].Position = glm::vec3(q.x0, -q.y0, -0.1f);
		v[0].Color = glm::vec4(0, 0, 1, 1);
		v[0].UV = glm::vec2(q.s0, q.t0);

		v[1].Position = glm::vec3(q.x1, -q.y0, -0.1f);
		v[1].Color = glm::vec4(0, 1, 0, 1);
		v[1].UV = glm::vec2(q.s1, q.t0);

		v[2].Position = glm::vec3(q.x1, -q.y1, -0.1f);
		v[2].Color = glm::vec4(1, 0, 0, 1);
		v[2].UV = glm::vec2(q.s1, q.t1);

		v[3].Position = glm::vec3(q.x0, -q.y1, -0.1f);
		v[3].Color = glm::vec4(0, 0, 1, 1);
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

	MesshToFill.GenerateMeshData(Vertices, Indices);
}
