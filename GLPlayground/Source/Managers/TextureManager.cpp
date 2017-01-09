#include "TextureManager.h"
#include <memory>
#include <string>
#include "Managers/ResourceManager.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

bool TextureManager::CreateTexture(const std::string & TextureName, size_t & TextureID)
{
	bool alreadyCreated = false;

	GetTextureAndIDFromName(TextureName, alreadyCreated, TextureID);

	if (alreadyCreated)
	{
		return true;
	}


	Texture texture;

	std::string txStr = ResourceManager::GetTexturesFolder();
	if (!texture.LoadFromFile(txStr += TextureName))
	{
		return false;
	}

	std::size_t hash = std::hash<std::string>{}(TextureName);
	TextureID = hash;

	Textures[hash] = std::move(texture);

	return true;
}

inline Texture & TextureManager::GetTextureFromID(size_t TextureID, bool & Found)
{
	Found = true;
	std::map<std::size_t, Texture>::iterator it = Textures.find(TextureID);

	if (it == Textures.end())
	{
		Found = false;
	}

	return Textures[TextureID];
}

Texture & TextureManager::GetTextureAndIDFromName(const std::string & TextureName, bool & Found, size_t & ID)
{
	Found = true;

	std::size_t hash = std::hash<std::string>{}(TextureName);
	ID = hash;
	
	return GetTextureFromID(hash, Found);
}

Texture & TextureManager::GetTextureFromName(const std::string & TextureName, bool & Found)
{
	Found = true;

	std::size_t hash = std::hash<std::string>{}(TextureName);

	return GetTextureFromID(hash, Found);
}
