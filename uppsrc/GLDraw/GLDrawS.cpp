#include "GLDraw.h"

#define LTIMING(x) // RTIMING(x)

#ifdef GL_USE_SHADERS

namespace Upp {

GLProgram gl_image, gl_image_colored, gl_rect;

int u_color;

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

	gl_image.Create(R"(
		attribute vec4 a_position;
		attribute vec2 a_texCoord;
		uniform mat4 u_projection;
		varying vec2 v_texCoord;
		void main()
		{
		   gl_Position = u_projection * a_position;
		   v_texCoord = a_texCoord;
		}
		)", R"(
	#ifdef GL_ES
		precision mediump float;
		precision mediump int;
	#endif
		varying vec2 v_texCoord;
		uniform sampler2D s_texture;
		void main()
		{
			gl_FragColor = texture2D(s_texture, v_texCoord);
		}
		)",
		ATTRIB_VERTEX, "a_position",
		ATTRIB_TEXPOS, "a_texCoord"
	);

	glUniform1i(gl_image.GetUniform("s_texture"), 0);

	gl_image_colored.Create(R"(
		attribute vec4 a_position;
		attribute vec2 a_texCoord;
		attribute vec4 a_color;
		uniform mat4 u_projection;
		varying vec2 v_texCoord;
		varying vec4 v_color;
		void main()
		{
		   gl_Position = u_projection * a_position;
		   v_texCoord = a_texCoord;
		   v_color = a_color * vec4((1.0 / 255.0), (1.0 / 255.0), (1.0 / 255.0), 1);
		}
		)", R"(
	#ifdef GL_ES
		precision mediump float;
		precision mediump int;
	#endif
		varying vec2 v_texCoord;
		varying vec4 v_color;
		uniform sampler2D s_texture;
		void main()
		{
		   gl_FragColor = v_color;
		   gl_FragColor.a = texture2D(s_texture, v_texCoord).a;
		}
		)",
		ATTRIB_VERTEX, "a_position",
		ATTRIB_TEXPOS, "a_texCoord",
		ATTRIB_COLOR, "a_color"
	);

	glUniform1i(gl_image_colored.GetUniform("s_texture"), 0);

	gl_rect.Create(R"(
		attribute vec4 a_position;
		attribute vec4 a_color;
		uniform mat4 u_projection;
		varying vec4 v_color;
		void main()
		{
		 gl_Position = u_projection * a_position;
		 v_color = a_color * vec4((1.0 / 255.0), (1.0 / 255.0), (1.0 / 255.0), 1);
		}
		)", R"(
	#ifdef GL_ES
		precision mediump float;
		precision mediump int;
	#endif
		varying vec4 v_color;
		void main()
		{
		    gl_FragColor = v_color;
		}
		)",
		ATTRIB_VERTEX, "a_position",
		ATTRIB_COLOR, "a_color" // we need this per vertex because we want to group rects
	);

	u_color = gl_rect.GetUniform("u_color");

	glEnableVertexAttribArray(ATTRIB_VERTEX);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void GLOrtho(float left, float right, float bottom, float top, float near_, float far_, GLuint u_projection)
{
	float a = 2.0f / (right - left);
	float b = 2.0f / (top - bottom);
	float c = -2.0f / (far_ - near_);

	float tx = - (right + left) / (right - left);
	float ty = - (top + bottom) / (top - bottom);
	float tz = - (far_ + near_) / (far_ - near_);
	
	float ortho[16] = {
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, c, 0,
		tx, ty, tz, 1
	};

    glUniformMatrix4fv(u_projection, 1, 0, ortho);
}

void GLDraw::FlushPutRect()
{
	if(put_rect.GetCount() == 0)
		return;

	LTIMING("FlushPutRect");

	gl_rect.Use();

	Buffer<GLshort> vertex(8 * put_rect.GetCount());
	Buffer<GLubyte> color(12 * put_rect.GetCount());
	Buffer<GLushort> index(6 * put_rect.GetCount());
	GLshort *v = vertex;
	GLubyte *c = color;
	GLushort *n = index;
	for(int i = 0; i < put_rect.GetCount(); i++) {
		const RectColor& rc = put_rect[i];
	    *v++ = rc.rect.left;  *v++ = rc.rect.top;
	    *v++ = rc.rect.left;  *v++ = rc.rect.bottom;
	    *v++ = rc.rect.right; *v++ = rc.rect.bottom,
	    *v++ = rc.rect.right; *v++ = rc.rect.top;
		GLubyte r = rc.color.GetR();
		GLubyte g = rc.color.GetG();
		GLubyte b = rc.color.GetB();
		*c++ = r; *c++ = g; *c++ = b;
		*c++ = r; *c++ = g; *c++ = b;
		*c++ = r; *c++ = g; *c++ = b;
		*c++ = r; *c++ = g; *c++ = b;
		*n++ = 4 * i + 0;
		*n++ = 4 * i + 1;
		*n++ = 4 * i + 2;
		*n++ = 4 * i + 0;
		*n++ = 4 * i + 2;
		*n++ = 4 * i + 3;
	}

	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), color);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), vertex);

	glDisable(GL_BLEND);
	glDrawElements(GL_TRIANGLES, 6 * put_rect.GetCount(), GL_UNSIGNED_SHORT, index);
	glEnable(GL_BLEND);

	glDisableVertexAttribArray(ATTRIB_COLOR);
	
	put_rect.Clear();
}

void GLDraw::PutRect(const Rect& rect, Color color)
{
	LTIMING("PutRect");
	
#if 0
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
	else
		glDisable(GL_BLEND);

	GLubyte r = color.GetR();
	GLubyte g = color.GetG();
	GLubyte b = color.GetB();

	glUniform4f(u_color, r / 255.0, g / 255.0, b / 255.0, 1.0);
	
	static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), vertex);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	if(inv)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	else
		glEnable(GL_BLEND);
#endif

#if 1
	LTIMING("PutRect");
#ifdef GL_COMB_OPT
	if(color != InvertColor()) {
		RectColor& rc = put_rect.Add();
		rc.rect = rect;
		rc.color = color;
		if(put_rect.GetCount() > 100)
			FlushPutRect();
		return;
	}
#endif

	gl_rect.Use();

	GLshort vertex[] = {
	    (GLshort)rect.left, (GLshort)rect.top,
	    (GLshort)rect.left, (GLshort)rect.bottom,
	    (GLshort)rect.right, (GLshort)rect.bottom,
	    (GLshort)rect.right, (GLshort)rect.top,
	};

	bool inv = color == InvertColor();
	if(inv) {
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
		color = Color(255, 255, 255);
	}
	else
		glDisable(GL_BLEND);

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
#endif
}

static float *sMakePartial(float *partial, Size isz, const Rect& s)
{
	Sizef iszf = isz;
	Rectf h;
	h.left = s.left / iszf.cx;
	h.right = s.right / iszf.cx;
	h.top = s.top / iszf.cy;
	h.bottom = s.bottom / iszf.cy;
	partial[0] = (float)h.left;
	partial[1] = (float)h.top;
	partial[2] = (float)h.left;
	partial[3] = (float)h.bottom;
	partial[4] = (float)h.right;
	partial[5] = (float)h.bottom;
	partial[6] = (float)h.right;
	partial[7] = (float)h.top;
	return partial;
}

void GLDraw::PutImage(Point p, const Image& img, const Rect& src)
{
	LTIMING("PutImage");

	FlushPutRect();

	gl_image.Use();

	Size isz = img.GetSize();
	Rect s = src & isz;
	Rect r(p, s.GetSize());
	
	GLshort vertex[] = {
	    (GLshort)r.left, (GLshort)r.top,
	    (GLshort)r.left, (GLshort)r.bottom,
	    (GLshort)r.right, (GLshort)r.bottom,
	    (GLshort)r.right, (GLshort)r.top,
	};

	static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
	float  partial[8];

	const float *tc;

	if(src == isz) {
		static float fixed[] = {
			0.0, 0.0,
			0.0, 1.0,
			1.0, 1.0,
			1.0, 0.0,
		};
		tc = fixed;
	}
	else
		tc = sMakePartial(partial, isz, s);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnableVertexAttribArray(ATTRIB_TEXPOS);
	glVertexAttribPointer(ATTRIB_TEXPOS, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), tc);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), vertex);
	glBindTexture(GL_TEXTURE_2D, GetTextureForImage(img));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	glDisableVertexAttribArray(ATTRIB_TEXPOS);
}

void GLDraw::PutImage(Point p, const Image& img, const Rect& src, Color color)
{
	FlushPutRect();

	gl_image_colored.Use();

	Size isz = img.GetSize();
	Rect s = src & isz;
	Rect rect(p, s.GetSize());

	GLshort vertex[] = {
	    (GLshort)rect.left, (GLshort)rect.top,
	    (GLshort)rect.left, (GLshort)rect.bottom,
	    (GLshort)rect.right, (GLshort)rect.bottom,
	    (GLshort)rect.right, (GLshort)rect.top,
	};

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
	float  partial[8];

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
	else
		tc = sMakePartial(partial, isz, s);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableVertexAttribArray(ATTRIB_TEXPOS);
	glVertexAttribPointer(ATTRIB_TEXPOS, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), tc);
	glEnableVertexAttribArray(ATTRIB_COLOR);
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), colors);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), vertex);
	glBindTexture(GL_TEXTURE_2D, GetTextureForImage(img));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	glDisableVertexAttribArray(ATTRIB_COLOR);
	glDisableVertexAttribArray(ATTRIB_TEXPOS);
}

void GLDraw::Init(Size sz, uint64 context_)
{
	SDraw::Init(sz);

	ONCELOCK {
		initializeGL();
	}

	gl_image.Use();
	static int uni1 = gl_image.GetUniform("u_projection");
	GLOrtho(0, (float)sz.cx, (float)sz.cy, 0, 0.0f, 1.0f, uni1);
	gl_image_colored.Use();
	static int uni2 = gl_image_colored.GetUniform("u_projection");
	GLOrtho(0, (float)sz.cx, (float)sz.cy, 0, 0.0f, 1.0f, uni2);
	gl_rect.Use();
	static int uni3 = gl_rect.GetUniform("u_projection");
	GLOrtho(0, (float)sz.cx, (float)sz.cy, 0, 0.0f, 1.0f, uni3);
}

void GLDraw::Finish()
{
#ifdef GL_COMB_OPT
	FlushPutRect();
#endif
}

GLDraw::~GLDraw()
{
	FlushPutRect();
}

};

#endif
