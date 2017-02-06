#include "stdafx.h"
#include "Mesh.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"

Mesh::Mesh(const std::vector<Vertex> & Vertices, const std::vector<Index> & Indices) 
{
	GenerateMeshData(Vertices, Indices);
}

Mesh::Mesh(): VEO(0), VBO(0), VAO(0), Model(glm::mat4(1))
{

}

Mesh::Mesh(Mesh && MeshToReplace)
{
	Vertices = std::move(MeshToReplace.Vertices);
	Indices = std::move(MeshToReplace.Indices);

	Model = std::move(MeshToReplace.Model);

	VBO = MeshToReplace.VBO;
	MeshToReplace.VBO = 0;

	VEO = MeshToReplace.VEO;
	MeshToReplace.VEO = 0;

	VAO = MeshToReplace.VAO;
	MeshToReplace.VAO = 0;

}

Mesh & Mesh::operator=(Mesh && MeshToReplace)
{
	Vertices = std::move(MeshToReplace.Vertices);
	Indices = std::move(MeshToReplace.Indices);

	Model = std::move(MeshToReplace.Model);

	VBO = MeshToReplace.VBO;
	MeshToReplace.VBO = 0;

	VEO = MeshToReplace.VEO;
	MeshToReplace.VEO = 0;

	VAO = MeshToReplace.VAO;
	MeshToReplace.VAO = 0;

	return *this;
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
