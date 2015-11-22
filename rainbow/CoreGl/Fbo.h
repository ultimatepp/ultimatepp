#ifndef _CoreGl_Fbo_h_
#define _CoreGl_Fbo_h_

#include <Core/Core.h>
#include <Draw/Draw.h>
#include "Gl.h"

NAMESPACE_UPP

struct Fbo : Moveable<Fbo>
{
	GLuint texId;
	GLuint fbId;
	GLuint rbId;
	GLenum status;
	int width;
	int height;
	bool ready;
	
	bool Create(int width, int height, bool resize = false);
	void Resize(int width, int height);
	void Bind();
	void Unbind();
	void Clear();
	void BlitToScreen();
	void BlitTo(const Fbo& fbo);
	void Remove();
	String GetError();
	
	Fbo() : width(0), height(0), ready(false)
	{}
};

END_UPP_NAMESPACE

#endif
