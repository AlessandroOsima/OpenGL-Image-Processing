#include "stdafx.h"
#include "RenderPass.h"
#include <sstream>

RenderPass::RenderPass(Material && PassMaterial, bool RenderOnMainFramebuffer, bool UsePreviousPassAsAttachment) :
	CurrentMaterial(std::move(PassMaterial)),
	RenderOnMainFramebuffer(RenderOnMainFramebuffer),
	UsePreviousPassAsAttachment(UsePreviousPassAsAttachment)
{
}


RenderPass::RenderPass(const RenderPass & RP)
{
	int blabla = 0;
}

RenderPass::RenderPass(RenderPass && RenderPassToReplace)
{
	RenderOnMainFramebuffer = RenderPassToReplace.RenderOnMainFramebuffer;
	UsePreviousPassAsAttachment = RenderPassToReplace.UsePreviousPassAsAttachment;

	Uniforms = std::move(RenderPassToReplace.Uniforms);
	CurrentMaterial = std::move(RenderPassToReplace.CurrentMaterial);
}

RenderPass::~RenderPass()
{
}

void RenderPass::BindUniforms()
{

	bool Found = false;
	ShaderProgram & shader = ShaderManager::GetShaderManager().GetShader(CurrentMaterial.Program, Found);

	if (Found)
	{
		for (UniformsToBind & uniform : Uniforms)
		{
			unsigned int index = shader.GetUniformIndex(uniform.UniformName);

			if (index != GL_INVALID_INDEX)
			{
				switch (uniform.Type)
				{
				case  UniformType::Mat3:
				{
					shader.SetUniformMatrix3(index, uniform.TypeData.mat3Val);
					break;
				}

				case  UniformType::Mat4:
				{
					shader.SetUniformMatrix4(index, uniform.TypeData.mat4Val);
					break;
				}

				case  UniformType::Vec3:
				{
					shader.SetUniformVector3(index, uniform.TypeData.vec3Val);
					break;
				}
				case  UniformType::Vec4:
				{
					shader.SetUniformVector4(index, uniform.TypeData.vec4Val);
					break;
				}

				case  UniformType::Float:
				{
					shader.SetUniformFloat(index, uniform.TypeData.floatVal);
					break;
				}

				default:
				{
					break;
				}
				}
			}
		}
	}
}

void RenderPass::Init()
{
	CurrentMaterial.CreateObjects();
}

void RenderPass::DeInit()
{
	CurrentMaterial.RemoveObjects();
}

RenderPassGroup::RenderPassGroup(uint32_t OffscreenTextureWidth, uint32_t OffscreenTextureHeight) : OffscreenTextureWidth(OffscreenTextureWidth), OffscreenTextureHeight(OffscreenTextureHeight), RenderPasses({})
{

}

RenderPassGroup::RenderPassGroup(RenderPassGroup && RenderPassGroupToReplace)
{
	RenderPasses = std::move(RenderPassGroupToReplace.RenderPasses);
	OffscreenTextureHeight = RenderPassGroupToReplace.OffscreenTextureHeight;
	OffscreenTextureWidth = RenderPassGroupToReplace.OffscreenTextureWidth;

	OffscreenTexture = RenderPassGroupToReplace.OffscreenTexture;
	RenderPassGroupToReplace.OffscreenTexture = 0;
	AttachmentTexture = RenderPassGroupToReplace.AttachmentTexture;
}

RenderPassGroup & RenderPassGroup::operator=(RenderPassGroup && RenderPassGroupToReplace)
{
	RenderPasses = std::move(RenderPassGroupToReplace.RenderPasses);
	OffscreenTextureHeight = RenderPassGroupToReplace.OffscreenTextureHeight;
	OffscreenTextureWidth = RenderPassGroupToReplace.OffscreenTextureWidth;

	OffscreenTexture = RenderPassGroupToReplace.OffscreenTexture;
	RenderPassGroupToReplace.OffscreenTexture = 0;
	AttachmentTexture = RenderPassGroupToReplace.AttachmentTexture;

	return *this;
}

void RenderPassGroup::Init()
{
	static std::string AttachmentTextureName = "StaticPassAttachment";

	TextureManager::GetTextureManager().CreateTexture(AttachmentTextureName, GL_RGBA8, OffscreenTextureWidth, OffscreenTextureHeight, AttachmentTexture);
	
	static int ID = 0;
	std::stringstream stream;
	stream << "PassGroupOffscrenResult" << ID << std::ends;
	TextureManager::GetTextureManager().CreateTexture(stream.str(), GL_RGBA8, OffscreenTextureWidth, OffscreenTextureHeight, OffscreenTexture);
	ID++;

	for (auto & pass : RenderPasses)
	{
		pass.Init();
	}
}

void RenderPassGroup::DeInit()
{
	for (auto & pass : RenderPasses)
	{
		pass.DeInit();
	}

	TextureManager::GetTextureManager().DestroyTexture(OffscreenTexture);
}
