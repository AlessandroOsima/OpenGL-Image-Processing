#pragma once
#include <GL/glew.h>
#include "Math/MathTypes.h"
#include <vector>
#include "Renderer/Texture.h"
#include "Renderer/ShaderProgram.h"
#include <memory>

using Index = uint32_t;

struct Material
{
	size_t DiffuseTexture = 0;
	size_t Program = 0; 
};

union UniformTypeData
{
	glm::mat3 mat3Val;
	glm::mat4 mat4Val;
	glm::vec4 vec4Val;
	glm::vec3 vec3Val;
	float floatVal;
};

enum class UniformType
{
	Mat4,
	Mat3,
	Vec4,
	Vec3,
	Float
};

struct UniformsToBind
{
    std::string UniformName;
	UniformTypeData TypeData;
	UniformType Type;
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

	inline unsigned int AddUniform(const UniformsToBind & Uniform)
	{
		Uniforms.push_back(Uniform);
		return Uniforms.size() - 1;
	}

	inline UniformsToBind * GetUniform(unsigned int Index)
	{
		if (Index < Uniforms.size())
		{
			return &Uniforms[Index];
		}

		return nullptr;
	}


private:
	std::vector<UniformsToBind> Uniforms;
	std::vector<Vertex> Vertices;
	std::vector<Index> Indices;
	Material CurrentMaterial;
	glm::mat4 Model;

	GLuint VBO;
	GLuint VEO;
	GLuint VAO;
	GLuint DiffuseSampler = -1;
};

