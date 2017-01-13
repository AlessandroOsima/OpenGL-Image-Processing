#version 450 core

layout (location = 0) out vec4 color;
layout (binding = 0) uniform sampler2D diffuseSampler;

in vec4 fragmentColor;
in vec2 uvCoords;



void main()
{

   mat3 SobelY;
   SobelY[0] = vec3(-1, -2, -1);
   SobelY[1] = vec3(0, 0, 0);
   SobelY[2] = vec3(1, 2, 1);

   mat3 SobelX;
   SobelX[0] = vec3(-1, 0, 1);
   SobelX[1] = vec3(-2, 0, 2);
   SobelX[2] = vec3(-1, 0, 1);

   vec4 sum = vec4(0,0,0,1.f);

   //0.2989 * R + 0.5870 * G + 0.1140 * B 

   //Up 1 col
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,-1)) * SobelY[0].x;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(0,-1)) * SobelY[0].y;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,-1)) * SobelY[0].z;

   //Center
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,0)) * SobelY[1].x;
   sum += vec4(texture(diffuseSampler, uvCoords).xyz, 1) * SobelY[1].y;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,0)) * SobelY[1].z;

   //Down 1 col
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,1)) * SobelY[2].x;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(0,1)) * SobelY[2].y;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,1)) * SobelY[2].z;


      //Up 1 col
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,-1)) * SobelX[0].x;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(0,-1)) * SobelX[0].y;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,-1)) * SobelX[0].z;

   //Center
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,0)) * SobelX[1].x;
   sum += vec4(texture(diffuseSampler, uvCoords).xyz, 1) * SobelX[1].y;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,0)) * SobelX[1].z;

   //Down 1 col
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(-1,1)) * SobelX[2].x;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(0,1)) * SobelX[2].y;
   sum += textureOffset(diffuseSampler, uvCoords, ivec2(1,1)) * SobelX[2].z;
  
   color = vec4(sum.xyz, 1.0f);
}