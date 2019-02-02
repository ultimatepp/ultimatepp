#include "GLDraw.h"

namespace Upp {
	
void GLTexture::Clear()
{
	if(data && --data->refcount == 0) {
		glDeleteTextures(1, &data->textureid);
		delete data;
	}
	data = NULL;
}

void GLTexture::Set(GLuint id, Size sz, Point hotspot)
{
	Clear();
	data = new Data;
	data->sz = sz;
	data->hotspot = hotspot;
	data->textureid = id;
}

void GLTexture::Set(const Image& img, dword flags)
{
	Set(CreateGLTexture(img, flags), img.GetSize(), img.GetHotSpot());
}

extern int sTextureCounter;

void GLTexture::Bind(int ii) const
{
	if(data) {
		glActiveTexture(GL_TEXTURE0 + ii);
		glBindTexture(GL_TEXTURE_2D, data->textureid);
		sTextureCounter++;
	}
}

GLTexture::GLTexture(const GLTexture& src)
{
	data = src.data;
	if(data)
		data->refcount++;
}

GLTexture& GLTexture::operator=(const GLTexture& src)
{
	Clear();
	data = src.data;
	if(data)
		data->refcount++;
	return *this;
}

void GLBind(int ii, const Image& img, dword style)
{
	extern int sTextureCounter;
	glActiveTexture(GL_TEXTURE0 + ii);
	glBindTexture(GL_TEXTURE_2D, GetTextureForImage(style, img));
	sTextureCounter++;
}

void GLBind(const Image& img, dword style)
{
	GLBind(0, img, style);
}

const GLVertexData& GLRectMesh()
{
	static GLVertexData mesh;
	ONCELOCK {
		static const float box[] = {
			0, 0, // 0
			0, 1, // 1
			1, 0, // 2
			1, 1, // 3
		};
		static const int ndx[] = {
			0, 1, 2, 1, 2, 3
		};
		mesh.Add(box, 2, 4).Index(ndx, 6);
	}
	return mesh;
}

void GLDrawTexture(const GLContext2D& dd, const Rectf& rect, int textureid, Size tsz, const Rect& src)
{
	static GLCode program(R"(
//		#version 330 core
		uniform vec2 offset;
		uniform vec2 scale;

		uniform vec2 toffset;
		uniform vec2 tscale;

	    attribute vec2 aPos;
		varying vec2 tPos;
	    void main()
	    {
			gl_Position = vec4(scale * aPos + offset, 0, 1);
			tPos = tscale * aPos + toffset;
	    }
	)", R"(
//		#version 330 core
		varying vec2 tPos;
		uniform float alpha;
		uniform sampler2D s_texture;
		void main()
		{
			vec4 v = texture2D(s_texture, tPos);
			gl_FragColor = alpha * clamp(v, vec4(0, 0, 0, 0), v.aaaa); // clamp fixes non-premultiplied textures
		}
	)");

	if(tsz.cx * tsz.cy == 0)
		return;
	
	static int ioffset = program["offset"];
	static int iscale = program["scale"];
	static int ialpha = program["alpha"];
	static int itscale = program["tscale"];
	static int itoffset = program["toffset"];

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, textureid);
	sTextureCounter++;
	GLRectMesh().Draw(
		program(ioffset, dd.vs * rect.TopLeft() + dd.off)
		       (iscale, dd.vs * rect.GetSize())
		       (ialpha, dd.alpha)
		       (itoffset, Sizef((float)src.left / tsz.cx, (float)src.top / tsz.cy))
		       (itscale, Sizef((float)src.GetWidth() / tsz.cx, (float)src.GetHeight() / tsz.cy))
	);
}

void GLDrawTexture(const GLContext2D& dd, const Rectf& rect, const GLTexture& img, const Rect& src)
{
	GLDrawTexture(dd, rect, img.GetID(), img.GetSize(), src);
}

void GLDrawImage(const GLContext2D& dd, const Rectf& rect, const Image& img, const Rect& src)
{
	GLDrawTexture(dd, rect, GetTextureForImage(img), img.GetSize(), src);
}

void GLDrawTexture(const GLContext2D& dd, const Rectf& rect, const GLTexture& img)
{
	GLDrawTexture(dd, rect, img, img.GetSize());
}

void GLDrawImage(const GLContext2D& dd, const Rectf& rect, const Image& img)
{
	GLDrawTexture(dd, rect, img, img.GetSize());
}

};
