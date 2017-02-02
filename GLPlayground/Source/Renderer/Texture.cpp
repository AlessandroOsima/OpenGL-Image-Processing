#include "stdafx.h"
#include "Texture.h"
#include <GL/glew.h>
#include "GLUtilities.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Logger/Logger.h"
#include <sstream>


Texture::Texture()
{
}


bool Texture::LoadFromFile(const std::string & ImageFile)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &ID);

	glCheckError("glCreateTextures");

	stbi_set_flip_vertically_on_load(1);
	unsigned char * data = stbi_load(ImageFile.data(), &Info.Width, &Info.Height, &Info.Format, 4);

	if (data)
	{
		glCheckFunction(glTextureStorage2D(ID, 1, GL_RGBA8, Info.Width, Info.Height));

		glCheckFunction(glTextureSubImage2D(ID, 0, 0, 0, Info.Width, Info.Height, GL_RGBA, GL_UNSIGNED_BYTE, data));
	}
	else
	{
		std::stringstream stream;
		stream << "Texture with filename " << ImageFile << " cannot be loaded" << std::ends;
		Logger::GetLogger().LogString(stream.str(), LogType::ERROR);
		return false;
	}

	stbi_image_free(data);
	return true;
}

void Texture::GenerateTextureWithSize(uint32_t Width, uint32_t Height, unsigned int Format)
{
	assert(Width);
	assert(Height);

	Info.Width = Width;
	Info.Height = Height;

	glCreateTextures(GL_TEXTURE_2D, 1, &ID);
	glTextureStorage2D(ID, 1, Format, Width, Height);
}



void Texture::SetImageData(unsigned int Level, int OffsetX, int OffsetY, int Width, int Height, unsigned int Format, unsigned int Type, void * Data)
{
	assert(Data);

	glCheckFunction(glTextureSubImage2D(ID, Level, OffsetX, OffsetY, Width, Height, Format, Type, Data));
}

void Texture::Bind()
{
	glCheckFunction(glBindTextureUnit(0, ID));
}

void Texture::UnBind()
{
	//glBindTextureUnit(0, 0);
}

Texture::~Texture()
{
	if (ID != INVALID_ID)
		glDeleteTextures(0, &ID);
}
