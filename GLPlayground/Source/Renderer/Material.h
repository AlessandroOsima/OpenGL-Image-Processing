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

	void CreateObjects();
	void RemoveObjects();

private:
	GLuint DiffuseSampler;
};
