#ifndef _SDLGLDraw_SDLGLDraw_h
#define _SDLGLDraw_SDLGLDraw_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SDLGLDraw/SDLGLDraw.lay>
#include <CtrlCore/lay.h>

class GLProgram {
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

public:
	void Create(const char *vertex_shader, const char *fragment_shader);
	void Clear();
	
	int  AttribLocation(const char *name)  { return glGetAttribLocation(program, name); }
	int  UniformLocation(const char *name) { return glGetUniformLocation(program, name); }
	
	void Use()                             { glUseProgram(program); }

	GLProgram();
};

class SDLGLDraw : public TopWindow {
public:
	virtual void Paint(Draw& w);

	typedef SDLGLDraw CLASSNAME;
	SDLGLDraw();
};

#endif
