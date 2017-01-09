#pragma once
#include "Renderer/Texture.h"
#include <map>
#include <functional>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	bool CreateTexture(const std::string & TextureName, size_t & TextureID);
	
	Texture & GetTextureFromID(size_t TextureID, bool & Found);

	Texture & GetTextureAndIDFromName(const std::string & TextureName, bool & Found, size_t & ID);
	Texture & GetTextureFromName(const std::string & TextureName, bool & Found);

	static TextureManager & GetTextureManager()
	{
		static TextureManager Manager;
		return Manager;
	}

private:
	std::map<std::size_t, Texture> Textures;
};

