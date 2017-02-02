#pragma once
#include <vector>
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include "Renderer/Material.h"
#include "Renderer/Uniform.h"

class RenderPass
{

public:
	RenderPass(const Material & PassMaterial, bool RenderOnMainFramebuffer, bool UsePreviousPassAsAttachment);
	~RenderPass();

	inline Material & GetMaterial()
	{
		return CurrentMaterial;
	}

	inline size_t AddUniform(const UniformsToBind & Uniform)
	{
		Uniforms.push_back(Uniform);
		return Uniforms.size() - 1;
	}

	inline UniformsToBind * GetUniform(unsigned int Index)
	{
		if (Index < Uniforms.size())
		{
			return &Uniforms[Index];
		}

		return nullptr;
	}

	bool RenderOnMainFramebuffer;
	bool UsePreviousPassAsAttachment;

	void BindUniforms();

	void Init();
	void DeInit();

private:
	Material CurrentMaterial;
	std::vector<UniformsToBind> Uniforms;
};


class RenderPassGroup
{

public:
	RenderPassGroup(uint32_t OffscreenTextureWidth, uint32_t OffscreenTextureHeight);

	std::vector<RenderPass> RenderPasses;

	void Init();
	void DeInit();

	inline size_t GetOffscreenTexture() const
	{
		return OffscreenTexture;
	}

	inline size_t GetAttachmentTexture() const
	{
		return AttachmentTexture;
	}

private:

	uint32_t OffscreenTextureWidth;
	uint32_t OffscreenTextureHeight;
	size_t OffscreenTexture;
	size_t AttachmentTexture;

	
};
