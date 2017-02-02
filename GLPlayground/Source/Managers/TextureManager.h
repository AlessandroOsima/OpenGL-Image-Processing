#pragma once
#include "Renderer/Texture.h"
#include <map>
#include <functional>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	bool CreateTextureFromFile(const std::string & TextureName, size_t & TextureID);
	bool CreateTexture(const std::string & TextureName, unsigned int Format, uint32_t Width, uint32_t Height, size_t & TextureID);

	Texture & GetTextureFromID(size_t TextureID, bool & Found);

	Texture & GetTextureAndIDFromName(const std::string & TextureName, bool & Found, size_t & ID);
	size_t GetIDFromName(const std::string & TextureName, bool & Found);
	Texture & GetTextureFromName(const std::string & TextureName, bool & Found);
	void DestroyTexture(size_t ID);
	bool TextureExist(size_t ID);

	static TextureManager & GetTextureManager()
	{
		static TextureManager Manager;
		return Manager;
	}

private:
	std::map<std::size_t, Texture> Textures;
};

