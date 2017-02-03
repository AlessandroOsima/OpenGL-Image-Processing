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

FontRenderer::FontRenderer()
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
	AllocatedChars = new stbtt_bakedchar[size];

	fclose(fontFile);

	stbtt_BakeFontBitmap(data, 0, Scale, bitmap.get(), BitMapWidth, BitMapHeight, 32, 96, (stbtt_bakedchar*)AllocatedChars);


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

	Quad.GenerateMeshData({  //Vertices
		{ glm::vec3(1.f, 1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(1,1) }, //0
		{ glm::vec3(1.f,  -1.f, -0.1f), glm::vec4(0, 1, 0, 1), glm::vec2(1,0) },  //1
		{ glm::vec3(-1.f, 1.f, -0.1f), glm::vec4(1, 0, 0, 1), glm::vec2(0,1) }, //2
		{ glm::vec3(-1.f,   -1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(0,0) }  //3
	},
		//Indices
	{
		0,
		1,
		2,
		2,
		1,
		3
	});


	glCreateBuffers(1, &UniformMatricesBufferID);


	glm::mat4 model;
	model = glm::scale(glm::mat4(1), glm::vec3(1, 1, 1));
	model = glm::translate(model, glm::vec3(Info.Width/2, Info.Height/2, 0));

	glm::mat4 projection = glm::ortho((float)0, (float)Info.Width, (float)0, (float)Info.Height, 0.f, 1.f);

	UniformMatrices matrices{projection, glm::mat4(1), model};

	glCheckFunction(glNamedBufferStorage(UniformMatricesBufferID, sizeof(UniformMatrices), &matrices, GL_DYNAMIC_STORAGE_BIT));
}

void FontRenderer::Render(GLRenderer & Renderer)
{

	//{ glm::vec3(1.f, 1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(1, 1) }, //0
	//{ glm::vec3(1.f,  -1.f, -0.1f), glm::vec4(0, 1, 0, 1), glm::vec2(1,0) },  //1
	//{ glm::vec3(-1.f, 1.f, -0.1f), glm::vec4(1, 0, 0, 1), glm::vec2(0,1) }, //2
	//{ glm::vec3(-1.f,   -1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(0,0) }  //3

	bool Found;

	ShaderProgram & fontProgram = ShaderManager::GetShaderManager().GetShader(FontMaterial.Program, Found);
	fontProgram.BindBufferToUniform(UniformMatricesBufferID, 0, "Matrices");

	AssertWithMessage(Found, "No shader found");

	FontMaterial.Bind();

	float x = 0;
	float y = 0;

	stbtt_aligned_quad q;
	stbtt_GetBakedQuad((stbtt_bakedchar*)AllocatedChars, BitMapWidth, BitMapHeight, 'A' - 32, &x, &y, &q, 1);//1=opengl & d3d10+,0=d3d9


	float w = (q.x1 - q.x0);
	float h = (q.y1 - q.y0);

	glm::mat4 model;

	Quad.GetVertices()[0].Position = glm::vec3(w/2, h/2, -0.1f);
	Quad.GetVertices()[1].Position = glm::vec3(w/2, -(h/2), -0.1f);
	Quad.GetVertices()[2].Position = glm::vec3(-(w/2), h/2, -0.1f);
	Quad.GetVertices()[3].Position = glm::vec3(-(w / 2), -(h / 2), -0.1f);

	Quad.GetVertices()[0].UV = glm::vec2(q.s1, q.t0);
	Quad.GetVertices()[1].UV = glm::vec2(q.s1, q.t1);
	Quad.GetVertices()[2].UV = glm::vec2(q.s0, q.t0);
	Quad.GetVertices()[3].UV = glm::vec2(q.s0, q.t1); 

	Quad.UpdateVertexData();

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
