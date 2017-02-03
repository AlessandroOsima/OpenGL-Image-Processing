#include "stdafx.h"
#include "Material.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include "Renderer/GLUtilities.h"
#include "Logger/Asserts.h"

void Material::Bind()
{
	bool Found = false;

	Texture & tx = TextureManager::GetTextureManager().GetTextureFromID(DiffuseTexture, Found);

	if (Found)
	{

		tx.Bind();
		glCheckFunction(glBindSampler(0, DiffuseSampler));

	}

	ShaderManager::GetShaderManager().UseProgram(Program);
}

void Material::UnBind()
{
	bool Found = false;
	Texture & tx = TextureManager::GetTextureManager().GetTextureFromID(DiffuseTexture, Found);

	if (Found)
	{
		tx.UnBind();
		glCheckFunction(glBindSampler(0, 0));
	}

	glUseProgram(0);
}

Material::Material(size_t DiffuseTexture, size_t Program) : DiffuseTexture(DiffuseTexture), Program(Program)
{

}

Material::Material() : DiffuseTexture(0), Program(0)
{

}

Material::~Material()
{

}

void Material::CreateObjects()
{
	AssertWithMessage(Program, "No valid program texture in material");

	glCreateSamplers(1, &DiffuseSampler);
}

void Material::RemoveObjects()
{
	glDeleteSamplers(1, &DiffuseSampler);
}

