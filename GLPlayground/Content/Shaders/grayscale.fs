#version 450 core

layout (location = 0) out vec4 color;
layout (binding = 0) uniform sampler2D diffuseSampler;

in vec4 fragmentColor;
in vec2 uvCoords;

vec4 ConvertToGrayscale(in vec4 RGBIn)
{
	float color = 0.2989f * RGBIn.r + 0.5870f * RGBIn.b + 0.1140f * RGBIn.g;

	return vec4(color, color, color, 1.f);
}

void main()
{
  color = ConvertToGrayscale(vec4(texture(diffuseSampler, uvCoords).xyz, 1));
}