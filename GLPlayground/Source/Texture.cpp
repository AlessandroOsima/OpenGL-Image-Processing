#include "Texture.h"


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Logger.h"
#include <sstream>


Texture::Texture()
{
}


bool Texture::LoadFromFile(const std::string & ImageFile)
{
	unsigned char * data = stbi_load(ImageFile.data(), &Info.Width, &Info.Height, &Info.Format, 0);

	if (data)
	{
		
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

Texture::~Texture()
{
}
