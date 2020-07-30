#include "GLCtrl_glad.h"

namespace Upp {

int  GLCtrl_glad::depthSize = 24;
int  GLCtrl_glad::stencilSize = 8;
bool GLCtrl_glad::doubleBuffering = true;
int  GLCtrl_glad::numberOfSamples = 1;
Size GLCtrl_glad::current_viewport;

extern void (*restore_gl_viewport__)();

void GLCtrl_glad::DoGLPaint()
{
	glClearDepth(1);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	glEnable(GL_MULTISAMPLE);
	Size sz = GetSize();
	current_viewport = sz;
	SetCurrentViewport();
	GLPaint();
}

void GLCtrl_glad::Init()
{
	
	
	Transparent();
#ifdef PLATFORM_WIN32
	CreateContext();//IMPORTANT
	pane.ctrl = this;
	Add(pane.SizePos());
#endif
	restore_gl_viewport__ = SetCurrentViewport;

}

Image GLCtrl_glad::MouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	if(mouseTarget) {
		return mouseTarget->MouseEvent(event, p + GetScreenView().TopLeft() - mouseTarget->GetScreenView().TopLeft(), zdelta, keyflags);
	}
	return Ctrl::MouseEvent(event, p, zdelta, keyflags);
}

void GLCtrl_glad::SetCurrentViewport()
{
	glViewport(0, 0, (GLsizei)current_viewport.cx, (GLsizei)current_viewport.cy);
}

void GLCtrl_glad::StdView()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	Size sz = GetSize();
	glViewport(0, 0, (GLsizei)sz.cx, (GLsizei)sz.cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)(sz.cx)/(GLfloat)(sz.cy), 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

#ifdef PLATFORM_WIN32

Image GLCtrl_glad::GLPane::MouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	p = p - GetScreenView().TopLeft() + ctrl->GetScreenView().TopLeft();
	return ctrl->MouseEvent(event, p, zdelta, keyflags);
}

#endif


}
