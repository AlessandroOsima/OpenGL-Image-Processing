#pragma once
#include "Renderer/Texture.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum class FrameBufferAttachmentType : uint8_t
{
	COLOR,
	DEPTH,
	STENCIL
};

enum class FramebufferBindType : uint8_t
{
	READ,
	DRAW,
	FRAMEBUFFER
};

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void BindTextureToFramebuffer(const Texture & TextureToBind, FrameBufferAttachmentType Attachment, uint32_t Level = 0);
	void UnbindFramebufferAttachment(FrameBufferAttachmentType Attachment);
	void BindFramebuffer(FramebufferBindType BindType);
	void UnBindFramebuffer();

	inline uint32_t GetID() const
	{
		return OffscreenFramebufferID;
	}

private:
	GLuint OffscreenFramebufferID;
};

