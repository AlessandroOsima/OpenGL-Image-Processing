#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;

uniform Matrices
{
	mat4 Projection;
	mat4 View;
	mat4 Model;
};


out vec4 fragmentColor;
out vec2 uvCoords;

void main()
{
  gl_Position = vec4(position.xyz, 1.0);
  uvCoords = uv; 
  fragmentColor = color;
}