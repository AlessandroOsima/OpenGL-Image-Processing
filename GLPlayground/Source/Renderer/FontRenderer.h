#pragma once

class FontRenderer
{
public:
	void Init(const std::string & FontName);
	void Render();
	void DeInit();

private:
	void * AllocatedChars;

	size_t FontTextureID;
};