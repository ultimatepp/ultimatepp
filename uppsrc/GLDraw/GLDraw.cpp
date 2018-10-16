#include "GLDraw.h"

#define LTIMING(x) // RTIMING(x)

#ifndef GL_USE_SHADERS

#define LLOG(x) // LOG(x)

namespace Upp {

void GLDraw::PutImage(Point p, const Image& img, const Rect& src)
{
	LTIMING("PutImage");
	
	if(img.GetLength() == 0)
		return;

	LLOG("PutImage " << img.GetSerialId() << ' ' << p.x << ", " << p.y << ", "<< img.GetSize());
	LLOG("SysImage cache pixels " << sTextureCache.GetSize() << ", count " << sTextureCache.GetCount());

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GetTextureForImage(img));
	
	if(src == img.GetSize()) {
		Rect r(p, img.GetSize());
		glBegin(GL_TRIANGLE_STRIP);
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0, 0);
			glVertex2i(r.left, r.top);
			glTexCoord2f(1, 0);
			glVertex2i(r.right, r.top);
			glTexCoord2f(0, 1);
			glVertex2i(r.left, r.bottom);
			glTexCoord2f(1, 1);
			glVertex2i(r.right, r.bottom);
		glEnd();
	}
	else {
		Sizef iszf = img.GetSize();
		Rect s = src & img.GetSize();
		Rect r(p, s.GetSize());
		Rectf h;
		h.left = (double)s.left / iszf.cx;
		h.right = (double)s.right / iszf.cx;
		h.top = (double)s.top / iszf.cy;
		h.bottom = (double)s.bottom / iszf.cy;
		glBegin(GL_TRIANGLE_STRIP);
			glColor3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(h.left, h.top);
			glVertex2i(r.left, r.top);
			glTexCoord2f(h.right, h.top);
			glVertex2i(r.right, r.top);
			glTexCoord2f(h.left, h.bottom);
			glVertex2i(r.left, r.bottom);
			glTexCoord2f(h.right, h.bottom);
			glVertex2i(r.right, r.bottom);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
}

void GLDraw::SetColor(Color c)
{
	glColor3f(c.GetR() / 255.0, c.GetG() / 255.0, c.GetB() / 255.0);
}

void GLDraw::PutRect(const Rect& r, Color color)
{
	LTIMING("PutRect");
	LLOG("PutRect " << r << " " << color);
	bool inv = color == InvertColor();
	if(inv)
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glBegin(GL_TRIANGLE_STRIP);
		if(inv)
			glColor3f(1.0f, 1.0f, 1.0f);
		else
			SetColor(color);
		glVertex2i(r.left, r.top);
		glVertex2i(r.right, r.top);
		glVertex2i(r.left, r.bottom);
		glVertex2i(r.right, r.bottom);
	glEnd();
	if(inv)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLDraw::Init(Size sz, uint64 context_)
{
	context = context_;

	SDraw::Init(sz);

	glEnable(GL_BLEND);

	glViewport(0, 0, (GLsizei)sz.cx, (GLsizei)sz.cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, sz.cx, sz.cy, 0, 0.0f, 1.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	LOG("==============================================================================");
/*	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	glGenBuffers(1, &tbo);
	glBindBuffer(GL_
*/
}

void GLDraw::Finish()
{
}

GLDraw::~GLDraw()
{
}

}

#endif