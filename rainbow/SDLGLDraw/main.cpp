#include "SDLGLDraw.h"

extern unsigned short *BaseAddress;

GLProgram gl_image, gl_rect;

GLuint m_posLoc;
GLuint m_texLoc;
GLuint m_samplerLoc;
GLuint m_texture;
GLuint m_posLoc2;
GLuint m_colorLoc;

GLuint u_projection, u_projection2;

float vertexCoords[] =
{
    10, 10,
    20, 10,
    20, 20,
    10, 20,
};

float texCoords[] =
{
0.0, 0.0,
0.0, 1.0,
1.0, 0.0,
1.0, 1.0
};

float colors[] =
{
1.0, 1.0, 0.0,
1.0, 1.0, 0.0,
1.0, 1.0, 0.0,
1.0, 1.0, 0.0,
};

GLushort indices[] = { 0, 1, 2, 2, 1, 3 };

#define INDEX 0 //ARRAY

void CheckError()
{
    /* Check for error conditions. */
    GLenum gl_error = glGetError( );
    if( gl_error != GL_NO_ERROR ) {
        Panic(String().Cat() << " OpenGL error: " << gl_error);
    }
}

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
		"   gl_FragColor = texture2D( s_texture, v_texCoord ); \n"
		"} \n"
	);

	m_posLoc = gl_image.AttribLocation("a_position");
	m_texLoc = gl_image.AttribLocation("a_texCoord");
	m_samplerLoc = gl_image.UniformLocation("s_texture");
	u_projection = gl_image.UniformLocation("u_projection");

	gl_rect.Create(
		"attribute vec4 a_position; \n"
		"attribute vec4 a_color; \n"
		"uniform mat4 u_projection; \n"
		"varying vec4 v_color; \n"
		"void main() \n"
		"{ \n"
		" gl_Position = u_projection * a_position; \n"
		" v_color = a_color; \n"
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
		"}"
	);

	m_posLoc2 = gl_rect.AttribLocation("a_position");
	m_colorLoc = gl_rect.AttribLocation("a_color");
	u_projection2 = gl_rect.UniformLocation("u_projection");
	
	InitTexture(CtrlImg::exclamation());
}

void GLOrtho(float left, float right, float bottom, float top, float near, float far, GLuint u_projection)
{
    float a = 2.0f / (right - left);
    float b = 2.0f / (top - bottom);
    float c = -2.0f / (far - near);

    float tx = - (right + left)/(right - left);
    float ty = - (top + bottom)/(top - bottom);
    float tz = - (far + near)/(far - near);

	float ortho[16] = {
	    a, 0, 0, 0,
	    0, b, 0, 0,
	    0, 0, c, 0,
	    tx, ty, tz, 1
	};

    glUniformMatrix4fv(u_projection, 1, 0, &ortho[0]);
}

void GLRect(Size sz, const Rect& rect, const Color& color)
{
	gl_rect.Use();
	GLOrtho(0, sz.cx, sz.cy, 0, 0.0f, 1.0f, u_projection2);

	float vertexCoords[] = {
	    rect.left, rect.top,
	    rect.left, rect.bottom,
	    rect.right, rect.bottom,
	    rect.right, rect.top,
	};
	float r = color.GetR() / 255.0;
	float g = color.GetG() / 255.0;
	float b = color.GetB() / 255.0;
	float colors[] = {
		r, g, b,
		r, g, b,
		r, g, b,
		r, g, b,
	};
	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	glVertexAttribPointer(m_posLoc2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertexCoords );
	glVertexAttribPointer(m_colorLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), colors );
 
	glEnableVertexAttribArray(m_posLoc2);
	glEnableVertexAttribArray(m_colorLoc);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	glDisableVertexAttribArray(m_posLoc2);
	glDisableVertexAttribArray(m_colorLoc);
}

void paintGL(Size sz)
{

	glClear( GL_COLOR_BUFFER_BIT );

#if 0
	// Use the program object
	glUseProgram ( m_program );

	glVertexAttribPointer(m_posLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertexCoords );
	glVertexAttribPointer(m_texLoc, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), texCoords );

	glEnableVertexAttribArray(m_posLoc);
	glEnableVertexAttribArray(m_texLoc);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i( m_samplerLoc, 0 );
	glDisableVertexAttribArray(m_posLoc);
	glDisableVertexAttribArray(m_texLoc);
#endif

#if 0
	CheckError();

	glVertexAttribPointer( m_posLoc2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), vertexCoords );
	CheckError();
	glVertexAttribPointer( m_colorLoc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), colors );

	CheckError();

	glEnableVertexAttribArray( m_posLoc );
	CheckError();
	glEnableVertexAttribArray( m_colorLoc );
	CheckError();

	glBindTexture(GL_TEXTURE_2D, m_texture);
/*	
	copyScreen( m_ScreenPtr, BaseAddress );
	
	glTexSubImage2D( GL_TEXTURE_2D,0,
				 0,0, 256,256,
				 GL_RGB,GL_UNSIGNED_SHORT_5_6_5, m_ScreenPtr );
*/
	CheckError();

	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
#endif

	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 10; j++)
			GLRect(sz, RectC(i * 16, j * 16, 12, 12), LtRed());
 // CheckError();
}

void SDLGLDraw::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White());
	w.DrawText(0, 0, "Hello world!");
	
	paintGL(GetSize());
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
