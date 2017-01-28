#pragma once
#include <GL/glew.h>
#include "Math/MathTypes.h"
#include <vector>
#include "Renderer/Texture.h"
#include "Renderer/ShaderProgram.h"
#include <memory>
#include "Renderer/Material.h"
#include "Renderer/Uniform.h"

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

	inline void SetModel(const glm::mat4 NewModel)
	{
		Model = NewModel;
	}

	inline glm::mat4 GetModel() const
	{
		return Model;
	}


private:
	std::vector<Vertex> Vertices;
	std::vector<Index> Indices;
	glm::mat4 Model;

	GLuint VBO;
	GLuint VEO;
	GLuint VAO;
};

