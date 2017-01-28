#pragma once
#include <GL/glew.h>

struct Material
{
	size_t DiffuseTexture;
	size_t Program;

	void Bind();
	void UnBind();


	Material(size_t DiffuseTexture, size_t Program);
	~Material();

	void Init();
	void DeInit();

private:
	GLuint DiffuseSampler;
};
