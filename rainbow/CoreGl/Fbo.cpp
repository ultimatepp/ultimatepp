#include "Fbo.h"

NAMESPACE_UPP

bool Fbo::Create(int width, int height, bool resize)
{
	this->width = width;
	this->height = height;

	if(!resize)
	{
		glGenFramebuffers(1, &fbId);
		glGenRenderbuffers(1, &rbId);
		glGenTextures(1, &texId);
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
	
	glBindRenderbuffer(GL_RENDERBUFFER, rbId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbId, 0);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbId);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbId);
	status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		
	ready = true;
	
	return status == GL_FRAMEBUFFER_COMPLETE;
}

void Fbo::Resize(int width, int height)
{
	if(ready)
		Create(width, height, true);
}

void Fbo::Bind()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbId);
}

void Fbo::Unbind()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Fbo::Clear()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Fbo::BlitToScreen()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Fbo::BlitTo(const Fbo& fbo)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo.fbId);
	glBlitFramebuffer(0, 0, width, height, 0, 0, fbo.width, fbo.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Fbo::Remove()
{
	glDeleteTextures(1, &texId);
	glDeleteRenderbuffers(1, &rbId);
	glDeleteFramebuffers(1, &fbId);
}

String Fbo::GetError()
{
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		return "Framebuffer complete";
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		return "Framebuffer incomplete attachment";
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		return "Framebuffer incomplete missing attachment";
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		return "Framebuffer incomplete draw buffer";
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		return "Framebuffer incomplete read buffer";
	case GL_FRAMEBUFFER_UNSUPPORTED:
		return "Framebuffer unsupported";
	default:
		return "Unknown error";
	}
}

END_UPP_NAMESPACE