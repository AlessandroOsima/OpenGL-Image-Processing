#version 450 core

layout (location = 0) out vec4 color;

layout (binding = 0) uniform sampler2D diffuseSampler;

uniform mat3 Mask;

in vec4 fragmentColor;
in vec2 uvCoords;

void main()
{
   vec4 sum = vec4(0,0,0,1.f);

   //Up 1 col
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,-1)) * Mask[0].x;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(0,-1)) * Mask[0].y;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,-1)) * Mask[0].z;

   //Center
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,0)) * Mask[1].x;
   sum += vec4(texture(diffuseSampler, uvCoords).xyz, 1) * Mask[1].y;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,0)) * Mask[1].z;

   //Down 1 col
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,1)) * Mask[2].x;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(0,1)) * Mask[2].y;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,1)) * Mask[2].z;
  
   color = vec4(sum.xyz, 1.0f);
}