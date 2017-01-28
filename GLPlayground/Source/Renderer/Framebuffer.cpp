#include "Framebuffer.h"
#include "Renderer/GLUtilities.h"


Framebuffer::Framebuffer()
{
	glCheckFunction(glCreateFramebuffers(1, &OffscreenFramebufferID));
}

Framebuffer::~Framebuffer()
{
	glCheckFunction(glDeleteFramebuffers(1, &OffscreenFramebufferID));
}

void Framebuffer::BindFramebuffer(FramebufferBindType BindType)
{

	switch (BindType)
	{
	case FramebufferBindType::READ:
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, OffscreenFramebufferID);
		break;
	}
	case FramebufferBindType::DRAW:
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, OffscreenFramebufferID);
		break;
	}
	case FramebufferBindType::FRAMEBUFFER:
	{
		glBindFramebuffer(GL_FRAMEBUFFER, OffscreenFramebufferID);
		break;
	}
	default:
		break;
	}
}

void Framebuffer::UnBindFramebuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Framebuffer::BindTextureToFramebuffer(const Texture & TextureToBind, FrameBufferAttachmentType Attachment, uint32_t Level)
{
	switch (Attachment)
	{
	case FrameBufferAttachmentType::COLOR:
	{
		glNamedFramebufferTexture(OffscreenFramebufferID, GL_COLOR_ATTACHMENT0, TextureToBind.GetID(), Level);
		break;
	}

	case FrameBufferAttachmentType::DEPTH:
	{
		glNamedFramebufferTexture(OffscreenFramebufferID, GL_DEPTH_ATTACHMENT, TextureToBind.GetID(), Level);
		break;
	}

	case FrameBufferAttachmentType::STENCIL:
	{
		glNamedFramebufferTexture(OffscreenFramebufferID, GL_STENCIL_ATTACHMENT, TextureToBind.GetID(), Level);
		break;
	}

	default:
		break;
	}
}

void Framebuffer::UnbindFramebufferAttachment(FrameBufferAttachmentType Attachment)
{
	switch (Attachment)
	{
	case FrameBufferAttachmentType::COLOR:
	{
		glNamedFramebufferTexture(OffscreenFramebufferID, GL_COLOR_ATTACHMENT0, 0, 0);
		break;
	}

	case FrameBufferAttachmentType::DEPTH:
	{
		glNamedFramebufferTexture(OffscreenFramebufferID, GL_DEPTH_ATTACHMENT, 0, 0);
		break;
	}

	case FrameBufferAttachmentType::STENCIL:
	{
		glNamedFramebufferTexture(OffscreenFramebufferID, GL_STENCIL_ATTACHMENT, 0, 0);
		break;
	}

	default:
		break;
	}
}
