#include "GLDraw.h"

#ifdef GL_USE_SHADERS

namespace Upp {

int u_imagetexture;

GLProgram gl_image, gl_rect;

void CheckError()
{
    /* Check for error conditions. */
    GLenum gl_error = glGetError( );
    if( gl_error != GL_NO_ERROR ) {
        Panic(String().Cat() << " OpenGL error: " << gl_error);
    }
}

void initializeGL()
{
	glewInit();

	gl_image.Create(
		"attribute vec4 a_position; \n"
		"attribute vec2 a_texCoord; \n"
		"uniform mat4 u_projection; \n"
		"varying vec2 v_texCoord; \n"
		"void main() \n"
		"{ \n"
		"   gl_Position = u_projection * a_position; \n"
		"   v_texCoord = a_texCoord; \n"
		"} \n",

		"#ifdef GL_ES\n"
		"precision mediump float; \n"
		"precision mediump int; \n"
		"#endif\n"
		"varying vec2 v_texCoord; \n"
		"uniform sampler2D s_texture; \n"
		"void main() \n"
		"{ \n"
		"   gl_FragColor = texture2D(s_texture, v_texCoord); \n"
		"} \n",
		ATTRIB_VERTEX, "a_position",
		ATTRIB_TEXPOS, "a_texCoord"
	);

	glUniform1i(gl_image.GetUniform("s_texture"), 0);

	gl_rect.Create(
		"attribute vec4 a_position; \n"
		"attribute vec4 a_color; \n"
		"uniform mat4 u_projection; \n"
		"varying vec4 v_color; \n"
		"void main() \n"
		"{ \n"
		" gl_Position = u_projection * a_position; \n"
		" v_color = a_color * vec4((1.0 / 255.0), (1.0 / 255.0), (1.0 / 255.0), 1); \n"
		"}",

		"#ifdef GL_ES\n"
		"precision mediump float;\n"
		"precision mediump int;\n"
		"#endif\n"
		"varying vec4 v_color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_FragColor = v_color;\n"
		"}",
		ATTRIB_VERTEX, "a_position",
		ATTRIB_COLOR, "a_color"
	);

	glEnableVertexAttribArray(ATTRIB_VERTEX);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLOrtho(float left, float right, float bottom, float top, float near, float far, GLuint u_projection)
{
	float a = 2.0f / (right - left);
	float b = 2.0f / (top - bottom);
	float c = -2.0f / (far - near);

	float tx = - (right + left) / (right - left);
	float ty = - (top + bottom) / (top - bottom);
	float tz = - (far + near) / (far - near);
	
	float ortho[16] = {
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, c, 0,
		tx, ty, tz, 1
	};

    glUniformMatrix4fv(u_projection, 1, 0, ortho);
}

void GLDraw::PutRect(const Rect& rect, Color color)
{
	gl_rect.Use();

	GLshort vertex[] = {
	    rect.left, rect.top,
	    rect.left, rect.bottom,
	    rect.right, rect.bottom,
	    rect.right, rect.top,
	};

	bool inv = color == InvertColor();
	if(inv) {
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
		color = Color(255, 255, 255);
	}

	GLubyte r = color.GetR();
	GLubyte g = color.GetG();
	GLubyte b = color.GetB();

	GLubyte colors[] = {
		r, g, b,
		r, g, b,
		r, g, b,
		r, g, b,
	};
	
	static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), colors);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), vertex);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	glDisableVertexAttribArray(ATTRIB_COLOR);

	if(inv)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLDraw::PutImage(Point p, const Image& img, const Rect& src)
{
	gl_image.Use();

	Rect s = src & img.GetSize();
	Rect r(p, s.GetSize());

	GLshort vertexCoords[] = {
	    r.left, r.top,
	    r.left, r.bottom,
	    r.right, r.bottom,
	    r.right, r.top,
	};

	static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	const float *tc;

	if(src == img.GetSize()) {
		static float fixed[] = {
			0.0, 0.0,
			0.0, 1.0,
			1.0, 1.0,
			1.0, 0.0,
		};
		tc = fixed;
	}
	else {
		Sizef iszf = img.GetSize();
		Rectf h;
		h.left = s.left / iszf.cx;
		h.right = s.right / iszf.cx;
		h.top = s.top / iszf.cy;
		h.bottom = s.bottom / iszf.cy;
		float partial[] = {
		    h.left, h.top,
		    h.left, h.bottom,
		    h.right, h.bottom,
		    h.right, h.top,
		};
		tc = partial;
	}

	glEnableVertexAttribArray(ATTRIB_TEXPOS);
	glVertexAttribPointer(ATTRIB_TEXPOS, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), tc);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), vertexCoords);
	glBindTexture(GL_TEXTURE_2D, GetTextureForImage(img));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	glDisableVertexAttribArray(ATTRIB_TEXPOS);
}

void GLDraw::Init(Size sz, uint64 context_)
{
	SDraw::Init(sz);

	ONCELOCK {
		initializeGL();	
	}

	gl_image.Use();
	GLOrtho(0, sz.cx, sz.cy, 0, 0.0f, 1.0f, gl_image.GetUniform("u_projection"));

	gl_rect.Use();
	GLOrtho(0, sz.cx, sz.cy, 0, 0.0f, 1.0f, gl_rect.GetUniform("u_projection"));
}

GLDraw::~GLDraw()
{
}

};

#endif
