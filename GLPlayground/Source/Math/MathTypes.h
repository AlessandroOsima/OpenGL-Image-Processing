#pragma once

#include <glm/glm.hpp>


struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 UV;
};

struct UniformMatrices
{
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
};