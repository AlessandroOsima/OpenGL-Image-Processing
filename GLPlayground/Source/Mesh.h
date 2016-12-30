#pragma once
#include <GL/glew.h>
#include "MathTypes.h"
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"
#include <memory>

using Index = uint32_t;

struct Material
{
	std::unique_ptr<Texture> DiffuseTexture = nullptr;
	size_t Program = 0; 
};


class Mesh
{
public:
	Mesh(const std::vector<Vertex> & Vertices, const std::vector<Index> & Indices, Material && MaterialToUse);
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

	inline void SetModel(const glm::mat4 NewModel)
	{
		Model = NewModel;
	}

	inline glm::mat4 GetModel() const
	{
		return Model;
	}

	inline const Material & GetMaterial()
	{
		return CurrentMaterial;
	}
private:
	std::vector<Vertex> Vertices;
	std::vector<Index> Indices;
	Material CurrentMaterial;
	glm::mat4 Model;

	GLuint VBO;
	GLuint VEO;
	GLuint VAO;
	GLuint DiffuseSampler = -1;
};

