#version 450 core

layout (location = 0) out vec4 color;
layout (binding = 0) uniform sampler2D diffuseSampler;

in vec4 fragmentColor;
in vec2 uvCoords;

void main()
{
   vec4 diffuseColor = vec4(texture(diffuseSampler, uvCoords).xyz, 1);
   color = diffuseColor;
   //color = vec4(uvCoords,0,0);
   //color = vec4(1,0,0,0);
}