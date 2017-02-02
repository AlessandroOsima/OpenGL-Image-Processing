#include "stdafx.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include "FontRenderer.h"

#include <GL/glew.h>
#include "Managers/TextureManager.h"
#include "Managers/ResourceManager.h"
#include <fstream>


void FontRenderer::Init(const std::string & FontName)
{
	unsigned char bitmap[521 * 512];

	std::ifstream file(ResourceManager::GetFontsFolder() + FontName, std::ios::binary | std::ios::ate);

	if (!file.is_open() || file.bad())
	{
		std::stringstream stream;
		stream << "Font file " << FontName << " cannot be opened for read" << std::ends;

		Logger::GetLogger().LogString(stream.str(), LogType::ERROR);
	}

	int size = file.tellg();

	file.seekg(0, file.beg);

	char * data = new char[size];

	file.read(data, size);

	if (file.bad())
	{
		std::stringstream stream;
		stream << "Font file " << FontName << " read error" << std::ends;

		Logger::GetLogger().LogString(stream.str(), LogType::ERROR);
	}

	file.close();

	AllocatedChars = new stbtt_bakedchar[96];

	stbtt_BakeFontBitmap((unsigned char *)data, 0, 32.0, bitmap, 512, 512, 32, 96, (stbtt_bakedchar*)AllocatedChars);


	bool Created, Found = false;

	Created = TextureManager::GetTextureManager().CreateTexture(FontName, GL_R8, 512, 512, FontTextureID);
	Texture FontTexture = TextureManager::GetTextureManager().GetTextureFromID(FontTextureID, Found);

	if (Created && Found)
	{
		FontTexture.SetImageData(0, 0, 0, 512, 512, GL_RED, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		delete[] AllocatedChars;

		std::stringstream stream;
		stream << "Creating texture for font " << FontName << " failed" << std::ends;
		Logger::GetLogger().LogString(stream.str(), LogType::ERROR);
	}

	delete[] data;
}

void FontRenderer::Render()
{
	float x = 0;
	float y = 0;
	
	stbtt_aligned_quad q;
	stbtt_GetBakedQuad((stbtt_bakedchar*)AllocatedChars, 512, 512, 't' - 32, &x, &y, &q, 1);//1=opengl & d3d10+,0=d3d9


}

void FontRenderer::DeInit()
{
	delete[] AllocatedChars;
	TextureManager::GetTextureManager().DestroyTexture(FontTextureID);
}
