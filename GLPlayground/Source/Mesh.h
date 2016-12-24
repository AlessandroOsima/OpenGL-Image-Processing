#pragma once
#include <GL/glew.h>
#include "MathTypes.h"
#include <vector>
using Index = uint32_t;

class Mesh
{
public:
	Mesh(const std::vector<Vertex> & Vertices, const std::vector<Index> & Indices);
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

	GLuint VBO;
	GLuint VEO;
	GLuint VAO;
};

