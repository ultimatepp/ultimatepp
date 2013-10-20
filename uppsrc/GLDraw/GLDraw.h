#ifndef _GlDraw_GlDraw_h_
#define _GlDraw_GlDraw_h_

#include <Draw/Draw.h>

#include "glew.h"

#ifdef flagWINGL
#include "wglew.h"
#endif

#define GL_USE_SHADERS

#include <GL/gl.h>

namespace Upp {
	
GLuint GetTextureForImage(const Image& img, uint64 context = 0);

#ifdef GL_USE_SHADERS

enum {
	ATTRIB_VERTEX = 1,
	ATTRIB_COLOR,
	ATTRIB_TEXPOS,
};

class GLProgram {
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

public:
	void Create(const char *vertex_shader, const char *fragment_shader,
	            Tuple2<int, const char *> *bind_attr = NULL, int bind_count = 0);
	void Create(const char *vertex_shader, const char *fragment_shader,
	            int attr1, const char *attr1name);
	void Create(const char *vertex_shader, const char *fragment_shader,
	            int attr1, const char *attr1name,
	            int attr2, const char *attr2name);
	void Create(const char *vertex_shader, const char *fragment_shader,
	            int attr1, const char *attr1name,
	            int attr2, const char *attr2name,
	            int attr3, const char *attr3name);
	void Clear();
	
	int  GetAttrib(const char *name)           { return glGetAttribLocation(program, name); }
	int  GetUniform(const char *name)          { return glGetUniformLocation(program, name); }
	
	void Use()                                 { glUseProgram(program); }

	GLProgram();
};

#endif

class GLDraw : public SDraw {
	void SetColor(Color c);

	uint64   context;

public:
	virtual void  PutImage(Point p, const Image& m, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);
	
	void Init(Size sz, uint64 context = 0);
	
	static void ClearCache();
	static void ResetCache();
	
	GLDraw()  { context = 0; }

	~GLDraw();
};

};

#endif