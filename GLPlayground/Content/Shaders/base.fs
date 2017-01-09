#version 450 core

layout (location = 0) out vec4 color;
layout (binding = 0) uniform sampler2D diffuseSampler;

in vec4 fragmentColor;
in vec2 uvCoords;

void main()
{
  color = vec4(texture(diffuseSampler, uvCoords).xyz, 1);
}