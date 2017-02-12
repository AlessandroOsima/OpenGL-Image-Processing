#pragma once

#include <glm/glm.hpp>


struct Vertex
{
	//Position of a Vertex in Model Space
	glm::vec3 Position;
	//Color of the vertex
	glm::vec4 Color;
	//UV coordinates of the texture bound to the primitive if there is one
	glm::vec2 UV;
};

struct UniformMatrices
{
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
};