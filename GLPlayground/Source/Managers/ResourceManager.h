#pragma once
//Currently just used to record resources folder locations in the filesystem
#include <string>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static const std::string & GetShadersForlder()
	{
		return Shaders;
	}

	static const std::string & GetTexturesFolder()
	{
		return Textures;
	}

private:
	static const std::string Shaders;
	static const std::string Textures;
};

