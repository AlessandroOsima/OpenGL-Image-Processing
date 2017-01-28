#pragma once
#include <glm/glm.hpp>
#include <string>

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

