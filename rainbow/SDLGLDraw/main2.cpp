#include "SDLGLDraw.h"

#if 1

int u_rectprojection;
int u_imageprojection;
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

GLuint m_texture;

void InitTexture(const Image &img)
{
	// Generate a texture object
	glGenTextures ( 1, &m_texture );
	
	// Bind the texture object
	glBindTexture ( GL_TEXTURE_2D, m_texture );
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	CheckError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	CheckError();

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	CheckError();
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	CheckError();

	// Set the filtering mode
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	Size sz = img.GetSize();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sz.cx, sz.cy, 0, GL_BGRA, GL_UNSIGNED_BYTE, ~img);
}

void initializeGL()
{
/*
	glDepthFunc(GL_ALWAYS);
	CheckError();
	glDisable(GL_DEPTH_TEST);
	CheckError();
	glDisable(GL_STENCIL_TEST);
	CheckError();
	glDisable(GL_CULL_FACE);
	CheckError();
*/

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

	u_imagetexture = gl_image.GetUniform("s_texture");
	u_imageprojection = gl_image.GetUniform("u_projection");

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

	u_rectprojection = gl_rect.GetUniform("u_projection");
	
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glEnableVertexAttribArray(ATTRIB_TEXPOS);
	glEnableVertexAttribArray(ATTRIB_COLOR);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	InitTexture(CtrlImg::exclamation());
}

void GLOrtho(float left, float right, float bottom, float top, float near, float far, GLuint u_projection)
{
	right *= 1.25;
	bottom *= 1.25;
	
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

void GLRect(Size sz, const Rect& rect, const Color& color)
{
	gl_rect.Use();

	GLOrtho(0, sz.cx, sz.cy, 0, 0.0f, 1.0f, u_rectprojection); // TODO

	GLshort vertex[] = {
	    rect.left, rect.top,
	    rect.left, rect.bottom,
	    rect.right, rect.bottom,
	    rect.right, rect.top,
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

	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), vertex);
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), colors);
 
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void GLImage(Size sz, Point p, const Image& m)
{
	gl_image.Use();

	GLOrtho(0, sz.cx, sz.cy, 0, 0.0f, 1.0f, u_imageprojection); // TODO

	sz = m.GetSize();
	DDUMP(sz);
	GLshort vertexCoords[] = {
		p.x, p.y,
		p.x, p.y + sz.cy,
		p.x + sz.cx, p.y + sz.cy,
		p.x + sz.cx, p.y,
	};

	static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	static float texCoords[] = // TODO
	{
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0
	};

	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_SHORT, GL_FALSE, 2 * sizeof(GLshort), vertexCoords );
	glVertexAttribPointer(ATTRIB_TEXPOS, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), texCoords );

	glBindTexture(GL_TEXTURE_2D, GetTextureForImage(m));
	glUniform1i(u_imagetexture, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void paintGL(Size sz)
{
//	glClear( GL_COLOR_BUFFER_BIT );
	
//	GLRect(sz, sz, LtRed());
//	GLRect(sz, Rect(sz).Deflated(2), Blue());
//	return;

//	GLOrtho(0, sz.cx, sz.cy, 0, 0.0f, 1.0f);

	DDUMP(sz);

//	GLRect(sz, sz, LtGray());
	GLRect(sz, RectC(sz.cx - 1, sz.cy - 1, 1, 1), Black());

	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 10; j++)
			GLRect(sz, RectC(i * 16, j * 16, 2, 2), decode(i, 0, LtBlue(), 1, White(), LtRed()));


	for(int i = 0; i < sz.cx / 2; i++)
		GLRect(sz, RectC(i * 2, 500, 1, 2), Black());

	for(int i = 0; i < 5; i++)
		GLRect(sz, RectC(i * 2, 520, 1, 1), Black());

	for(int i = 0; i < 5; i++)
		GLRect(sz, RectC(i, 530 + 2 * i, 1, 1), Black());

	for(int i = 0; i < 5; i++)
		GLRect(sz, RectC(2 * i, 600 + 2 * i, 1, 1), Black());

	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 10; j++) {
			GLRect(sz, RectC(120 + i * 40, 120 + j * 40, 32, 32), LtCyan());
			GLImage(sz, Point(120 + i * 40, 120 + j * 40), CtrlImg::reporticon());
		}
 // CheckError();
}

namespace Upp {
extern int SDLwidth;
extern int SDLheight;
};

void SDLGLDraw::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White());
	w.DrawText(0, 0, "Hello world!");
	paintGL(Size(SDLwidth, SDLheight));
}

SDLGLDraw::SDLGLDraw()
{
	glewInit();

	initializeGL();
}

GUI_APP_MAIN
{
	SDLGLDraw().Run();
}

#endif