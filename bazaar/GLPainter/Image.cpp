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

void GLTexture::Set(const Image& img, dword flags)
{
	Clear();
	data = new Data;
	data->sz = img.GetSize();
	data->textureid = CreateGLTexture(img, flags);
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

void GLDrawTexture(const GLContext2D& dd, const Rect& rect, int textureid, double alpha)
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

void GLDrawTexture(const GLContext2D& dd, const Rect& rect, const GLTexture& img, double alpha)
{
	GLDrawTexture(dd, rect, img.GetID(), alpha);
}

void GLDrawImage(const GLContext2D& dd, const Rect& rect, const Image& img, double alpha)
{
	GLDrawTexture(dd, rect, GetTextureForImage(img), alpha);
}

};
