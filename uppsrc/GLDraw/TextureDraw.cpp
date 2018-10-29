#include "GLDraw.h"

namespace Upp {

extern void (*restore_gl_viewport__)(); // in Draw/DrawUtil.cpp

void GLTextureDraw::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	if(texture)
		glDeleteTextures(1, &texture);
	if(rbo)
		glDeleteRenderbuffers(1, &rbo);
	if(framebuffer)
		glDeleteFramebuffers(1, &framebuffer);
	framebuffer = rbo = texture = 0;
	(*restore_gl_viewport__)();
}

GLTexture GLTextureDraw::GetResult()
{
	ASSERT(texture);
	Flush();
	GLTexture t;
	if(msaa > 1) {
		GLuint framebuffer2, texture2, rbo2;
		glGenFramebuffers(1, &framebuffer2);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
		
		glGenTextures(1, &texture2);
		
		glBindTexture(GL_TEXTURE_2D, texture2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sz.cx, sz.cy, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture2, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glGenRenderbuffers(1, &rbo2);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo2);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sz.cx, sz.cy);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo2);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		    NEVER_("Failed to create final texture draw framebuffer");

		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer2);
		glBlitFramebuffer(0, 0, sz.cx, sz.cy, 0, 0, sz.cx, sz.cy, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		t.Set(texture2, sz);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glDeleteRenderbuffers(1, &rbo2);
		glDeleteFramebuffers(1, &framebuffer2);
		Clear();
	}
	else {
		t.Set(texture, sz);
		texture = 0;
		Clear();
	}
	return t;
}

void GLTextureDraw::Create(Size sz_, int msaa_)
{
	sz = sz_;
	msaa = msaa_;

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	glGenTextures(1, &texture);
	
	if(msaa > 1) {
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGB, sz.cx, sz.cy, GL_TRUE);
		glEnable(GL_MULTISAMPLE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sz.cx, sz.cy, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, msaa > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, texture, 0);
	
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	if(msaa > 1)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH24_STENCIL8, sz.cx, sz.cy);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sz.cx, sz.cy);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    NEVER_("Failed to create texture draw framebuffer");
	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, sz.cx, sz.cy);
	
	DrawGL::Init(sz, 1);
	dd.vs.cy *= -1;
	dd.off = Sizef(-1, -1);
}

};