#include "Mesh.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"

Mesh::Mesh(const std::vector<Vertex> & Vertices, const std::vector<Index> & Indices, Material && MaterialToUse) : Vertices(Vertices), Indices(Indices), CurrentMaterial(std::move(MaterialToUse))
{
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

void Mesh::BindMesh()
{
	glBindVertexArray(VAO);
	
	if (CurrentMaterial.DiffuseTexture)
	{
		bool Found = false;
		Texture & tx = TextureManager::GetTextureManager().GetTextureFromID(CurrentMaterial.DiffuseTexture, Found);

		if (Found)
		{
			tx.Bind();
		}

		glBindSampler(0, DiffuseSampler);

		ShaderManager::GetShaderManager().UseProgram(CurrentMaterial.Program);
		
		Found = false;
		ShaderProgram & shader = ShaderManager::GetShaderManager().GetShader(CurrentMaterial.Program, Found);

		if (Found)
		{
			for (UniformsToBind & uniform : Uniforms)
			{
				unsigned int index = shader.GetUniformIndex(uniform.UniformName);

				if (index != GL_INVALID_INDEX)
				{
					switch (uniform.Type)
					{
						case  UniformType::Mat3 :
						{
							shader.SetUniformMatrix3(index, uniform.TypeData.mat3Val);
							break;
						}
						
						case  UniformType::Mat4:
						{
							shader.SetUniformMatrix4(index, uniform.TypeData.mat4Val);
							break;
						}
						
						case  UniformType::Vec3 :
						{
							shader.SetUniformVector3(index, uniform.TypeData.vec3Val);
							break;
						}
						case  UniformType::Vec4 :
						{
							shader.SetUniformVector4(index, uniform.TypeData.vec4Val);
							break;
						}

						case  UniformType::Float :
						{
							shader.SetUniformFloat(index, uniform.TypeData.floatVal);
							break;
						}

						default:
						{
							break;
						}
					}
				}
			}
		}

	}
}

void Mesh::UnbindMesh()
{
	if (CurrentMaterial.DiffuseTexture)
	{
		bool Found = false;
		Texture & tx = TextureManager::GetTextureManager().GetTextureFromID(CurrentMaterial.DiffuseTexture, Found);

		if (Found)
		{
			tx.UnBind();
		}

		glBindSampler(0, 0);
	}
	glBindVertexArray(0);
}
