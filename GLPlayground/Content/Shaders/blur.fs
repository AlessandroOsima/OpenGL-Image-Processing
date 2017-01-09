#version 450 core

layout (location = 0) out vec4 color;
layout (binding = 0) uniform sampler2D diffuseSampler;

in vec4 fragmentColor;
in vec2 uvCoords;

void main()
{
   vec4 centerColor = vec4(texture(diffuseSampler, uvCoords).xyz, 1);

   float weight = 1.0f / 9.0f;

   float summedBlur = 0f;

   summedBlur += textureOffset(diffuseSampler, uvCoords, ivec2(0,-1)).r;
   summedBlur += textureOffset(diffuseSampler, uvCoords, ivec2(1,-1)).r;
   summedBlur += textureOffset(diffuseSampler, uvCoords, ivec2(-1,-1)).r;

   summedBlur += textureOffset(diffuseSampler, uvCoords, ivec2(0,1)).r;
   summedBlur += textureOffset(diffuseSampler, uvCoords, ivec2(1,1)).r;
   summedBlur += textureOffset(diffuseSampler, uvCoords, ivec2(-1,1)).r;

   summedBlur += textureOffset(diffuseSampler, uvCoords, ivec2(1,0)).r;
   summedBlur += textureOffset(diffuseSampler, uvCoords, ivec2(-1,0)).r;

   summedBlur *= weight;

   color = vec4(summedBlur, summedBlur, summedBlur, 1.0f);
}