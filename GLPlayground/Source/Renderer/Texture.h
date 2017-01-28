#pragma once

#include <stdint.h>
#include <string>
using TextureID = uint32_t;
const TextureID INVALID_ID = 0;

struct TextureInfo
{
	int Width = 0;
	int Height = 0;
	int Format = -1;
};

class Texture
{
public:
	Texture();

	bool LoadFromFile(const std::string & ImageFile);

	void GenerateTextureWithSize(uint32_t Width, uint32_t Height);

	inline TextureID GetID() const
	{
		return ID;
	}

	inline const TextureInfo & GetTextureInfo() const
	{
		return Info;
	}

	void Bind();
	void UnBind();

	~Texture();

private:
	TextureID ID = INVALID_ID;
	TextureInfo Info;
};

