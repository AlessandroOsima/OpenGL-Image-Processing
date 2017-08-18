#version 450 core

layout (location = 0) out vec4 color;
layout (binding = 0) uniform sampler2D diffuseSampler;

in vec4 fragmentColor;
in vec2 uvCoords;

void main()
{
   vec4 sum = vec4(0,0,0, 1.0f);

   //Up 1 col
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,-1));
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(0,-1));
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,-1));

   //Center
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,0));
   sum += vec4(texture(diffuseSampler, uvCoords).xyz, 1);
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,0));

   //Down 1 col
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,1));
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(0,1));
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,1));

   sum /= 9.f;

   sum.w = 1.0f;

   color = sum;
}