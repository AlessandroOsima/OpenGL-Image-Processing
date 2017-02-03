#include "stdafx.h"
#include "Mesh.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"

Mesh::Mesh(const std::vector<Vertex> & Vertices, const std::vector<Index> & Indices) 
{
	GenerateMeshData(Vertices, Indices);
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VEO);

	glDeleteVertexArrays(1, &VAO);
}

void Mesh::GenerateMeshData(const std::vector<Vertex> & NewVertices, const std::vector<Index> & NewIndices)
{
	Vertices = NewVertices;
	Indices = NewIndices;

	glCreateBuffers(1, &VEO);
	glNamedBufferStorage(VEO, sizeof(Index) * Indices.size(), Indices.data(), GL_DYNAMIC_STORAGE_BIT);


	glCreateBuffers(1, &VBO);
	glNamedBufferStorage(VBO, sizeof(Vertex) * Vertices.size(), Vertices.data(), GL_DYNAMIC_STORAGE_BIT);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
	
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);

	//UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)((sizeof(glm::vec3)) + sizeof(glm::vec4)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

void Mesh::Bind()
{
	glBindVertexArray(VAO);
}

void Mesh::Unbind()
{
	glBindVertexArray(0);
}

void Mesh::UpdateVertexData()
{
	glNamedBufferSubData(VBO, 0, Vertices.size() * sizeof(Vertex), Vertices.data());
}

void Mesh::UpdateIndexData()
{
	glNamedBufferSubData(VEO, 0, Indices.size() * sizeof(Index), Indices.data());
}
