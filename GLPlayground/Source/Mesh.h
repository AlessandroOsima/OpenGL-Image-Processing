#pragma once
#include <GL/glew.h>
#include "MathTypes.h"
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"

using Index = uint32_t;

struct Material
{
	Texture * DiffuseTexture;
	ShaderProgram * Program; 
};


class Mesh
{
public:
	Mesh(const std::vector<Vertex> & Vertices, const std::vector<Index> & Indices, const Material & MaterialToUse);
	~Mesh();

	void GenerateMeshData();

	void BindMesh();
	void UnbindMesh();

	inline const std::vector<Vertex> & GetVertices() const
	{
		return Vertices;
	}

	inline const std::vector<Index> & GetIndices() const
	{
		return Indices;
	}

private:
	std::vector<Vertex> Vertices;
	std::vector<Index> Indices;
	Material CurrentMaterial;

	GLuint VBO;
	GLuint VEO;
	GLuint VAO;
	GLuint DiffuseSampler = -1;
};

