#include "GLPainter.h"

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

GLTexture::GLTexture(const GLTexture& src)
{
	data = src.data;
	data->refcount++;
}

GLTexture& GLTexture::operator=(const GLTexture& src)
{
	if(data != src.data) {
		if(data) Clear();
		data = src.data;
		data->refcount++;
	}
	return *this;
}

void GLBind(const Image& img, dword style)
{
	glBindTexture(GL_TEXTURE_2D, GetTextureForImage(style, img));
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

void GLDrawTexture(const GLContext2D& dd, const Rectf& rect, int textureid, double alpha)
{
	static GLCode program(R"(
		#version 330 core
		uniform vec2 offset;
		uniform vec2 scale;
	    in      vec2 aPos;
		out vec2 tPos;
	    void main()
	    {
			gl_Position = vec4(scale * aPos + offset, 0, 1);
			tPos = aPos;
	    }
	)", R"(
		#version 330 core
		in vec2 tPos;
		uniform float alpha;
		uniform sampler2D s_texture;
		void main()
		{
		   gl_FragColor = alpha * texture2D(s_texture, tPos);
		}
	)");

	static int offset = program["offset"];
	static int scale = program["scale"];
	static int ialpha = program["alpha"];

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, textureid);
	GLRectMesh().Draw(
		program(offset, dd.vs * rect.TopLeft() + Sizef(-1, 1))
		       (scale, dd.vs * rect.GetSize())
		       (ialpha, dd.alpha * alpha)
	);
}

void GLDrawTexture(const GLContext2D& dd, const Rectf& rect, const GLTexture& img, double alpha)
{
	GLDrawTexture(dd, rect, img.GetID(), alpha);
}

void GLDrawImage(const GLContext2D& dd, const Rectf& rect, const Image& img, double alpha)
{
	GLDrawTexture(dd, rect, GetTextureForImage(img), alpha);
}

void GLDrawTexture(const GLContext2D& dd, const Rectf& rect, int textureid, Size tsz, const Rect& src, double alpha)
{
	static GLCode program(R"(
		#version 330 core
		uniform vec2 offset;
		uniform vec2 scale;

		uniform vec2 toffset;
		uniform vec2 tscale;

	    in      vec2 aPos;
		out vec2 tPos;
	    void main()
	    {
			gl_Position = vec4(scale * aPos + offset, 0, 1);
			tPos = tscale * aPos + toffset;
	    }
	)", R"(
		#version 330 core
		in vec2 tPos;
		uniform float alpha;
		uniform sampler2D s_texture;
		void main()
		{
		   gl_FragColor = alpha * texture2D(s_texture, tPos);
		}
	)");

	static int ioffset = program["offset"];
	static int iscale = program["scale"];
	static int ialpha = program["alpha"];
	static int itscale = program["tscale"];
	static int itoffset = program["toffset"];

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, textureid);
	GLRectMesh().Draw(
		program(ioffset, dd.vs * rect.TopLeft() + Sizef(-1, 1))
		       (iscale, dd.vs * rect.GetSize())
		       (ialpha, dd.alpha * alpha)
		       (itoffset, Sizef((float)src.left / tsz.cx, (float)src.top / tsz.cy))
		       (itscale, Sizef((float)src.GetWidth() / tsz.cx, (float)src.GetHeight() / tsz.cy))
	);
}

void GLDrawTexture(const GLContext2D& dd, const Rectf& rect, const GLTexture& img, const Rect& src, double alpha)
{
	GLDrawTexture(dd, rect, img.GetID(), img.GetSize(), src, alpha);
}

void GLDrawImage(const GLContext2D& dd, const Rectf& rect, const Image& img, const Rect& src, double alpha)
{
	GLDrawTexture(dd, rect, GetTextureForImage(img), img.GetSize(), src, alpha);
}

};
