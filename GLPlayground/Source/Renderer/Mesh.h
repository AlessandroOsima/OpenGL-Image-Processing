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
	Mesh();

	Mesh(Mesh && MeshToReplace);
	Mesh & operator=(Mesh && MeshToReplace);

	~Mesh();

	void GenerateMeshData(const std::vector<Vertex> & NewVertices, const std::vector<Index> & NewIndices);

	void Bind();
	void Unbind();

	inline std::vector<Vertex> & GetVertices()
	{
		return Vertices;
	}

	inline std::vector<Index> & GetIndices()
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

	void UpdateVertexData();
	void UpdateIndexData();
private:
	std::vector<Vertex> Vertices;
	std::vector<Index> Indices;
	glm::mat4 Model;

	GLuint VBO;
	GLuint VEO;
	GLuint VAO;
};

