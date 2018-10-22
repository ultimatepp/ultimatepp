#include "GLPainter.h"

namespace Upp {

void GLTextureDraw::Clear()
{
	if(texture)
		glDeleteTextures(1, &texture);
	if(rbo)
		glDeleteRenderbuffers(1, &rbo);
	if(framebuffer)
		glDeleteFramebuffers(1, &framebuffer);
	framebuffer = rbo = texture = 0;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
}

GLTexture GLTextureDraw::GetResult()
{
	ASSERT(texture);
	GLTexture t;
	t.Set(texture, sz);
	texture = 0;
	Clear();
	return t;
}

void GLTextureDraw::Create(Size sz_)
{
	sz = sz_;

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texture);
 
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, sz.cx, sz.cy, GL_TRUE);

//  glBindTexture(GL_TEXTURE_2D, texture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sz.cx, sz.cy, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // TODO: A
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGBA8, sz.cx, sz.cy);
//	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, sz.cx, sz.cy);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sz.cx, sz.cy);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        NEVER_("Texture draw has failed");

	glClearColor(0.5f, 0.5f, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

};