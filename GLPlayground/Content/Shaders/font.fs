#version 450 core

layout (location = 0) out vec4 color;
layout (binding = 0) uniform sampler2D diffuseSampler;

in vec4 fragmentColor;
in vec2 uvCoords;

void main()
{
	vec4 colorSample = texture(diffuseSampler, uvCoords).rgba;
    color = vec4(colorSample.r * fragmentColor.rgb, colorSample.r);
}