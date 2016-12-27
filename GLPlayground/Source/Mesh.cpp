#include "Mesh.h"



Mesh::Mesh(const std::vector<Vertex> & Vertices, const std::vector<Index> & Indices,const Material & MaterialToUse)
{
	this->Vertices = Vertices;
	this->Indices = Indices;
	this->CurrentMaterial = MaterialToUse;

	GenerateMeshData();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VEO);

	glDeleteVertexArrays(1, &VAO);
	glDeleteSamplers(1, &DiffuseSampler);
}

void Mesh::GenerateMeshData()
{

	glCreateBuffers(1, &VEO);
	glNamedBufferStorage(VEO, sizeof(Index) * Indices.size(), Indices.data(), 0);


	glCreateBuffers(1, &VBO);
	glNamedBufferStorage(VBO, sizeof(Vertex) * Vertices.size(), Vertices.data(), 0);

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

	if (CurrentMaterial.DiffuseTexture)
	{
		glCreateSamplers(1, &DiffuseSampler);
	}
}

void Mesh::BindMesh()
{
	glBindVertexArray(VAO);
	
	if (CurrentMaterial.DiffuseTexture)
	{
		CurrentMaterial.DiffuseTexture->Bind();
		glBindSampler(0, DiffuseSampler);
	}
}

void Mesh::UnbindMesh()
{
	if (CurrentMaterial.DiffuseTexture)
	{
		CurrentMaterial.DiffuseTexture->UnBind();
		glBindSampler(0, 0);
	}
	glBindVertexArray(0);
}
