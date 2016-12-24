#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 fragmentColor;

void main()
{
  gl_Position = vec4(position.xyz, 1.0);
  fragmentColor = color;
}