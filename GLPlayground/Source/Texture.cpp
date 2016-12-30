#include "Texture.h"
#include <GL/glew.h>
#include "GLUtilities.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Logger.h"
#include <sstream>


Texture::Texture()
{
}


bool Texture::LoadFromFile(const std::string & ImageFile)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &ID);

	glCheckError("glCreateTextures");

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

void Texture::Bind()
{
	glBindTextureUnit(0, ID);
}

void Texture::UnBind()
{
	glBindTextureUnit(0, 0);
}

Texture::~Texture()
{
	if (ID != INVALID_ID)
		glDeleteTextures(0, &ID);
}
