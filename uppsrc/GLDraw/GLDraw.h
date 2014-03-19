#ifndef _GlDraw_GlDraw_h_
#define _GlDraw_GlDraw_h_

#include <Draw/Draw.h>

#include <plugin/glew/glew.h>

#ifdef PLATFORM_WIN32
#include <plugin/glew/wglew.h>
#endif

#define GL_USE_SHADERS

#define GL_COMB_OPT

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
	
#ifdef GL_COMB_OPT
	struct RectColor : Moveable<RectColor> {
		Rect  rect;
		Color color;
	};
	Vector<RectColor> put_rect;
#endif

	void FlushPutRect();

public:
	virtual void  PutImage(Point p, const Image& img, const Rect& src);
#ifdef GL_USE_SHADERS
	virtual void  PutImage(Point p, const Image& img, const Rect& src, Color color);
#endif
	virtual void  PutRect(const Rect& r, Color color);
	
	void Init(Size sz, uint64 context = 0);
	void Finish();
	
	static void ClearCache();
	static void ResetCache();
	
	GLDraw()  { context = 0; }

	~GLDraw();
};

};

#endif