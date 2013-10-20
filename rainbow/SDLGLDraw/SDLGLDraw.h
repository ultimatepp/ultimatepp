#ifndef _SDLGLDraw_SDLGLDraw_h
#define _SDLGLDraw_SDLGLDraw_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SDLGLDraw/SDLGLDraw.lay>
#include <CtrlCore/lay.h>

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

class SDLGLDraw : public TopWindow {
public:
	virtual void Paint(Draw& w);

	typedef SDLGLDraw CLASSNAME;
	SDLGLDraw();
};

#endif
