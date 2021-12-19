#include "GLCtrl.h"

namespace Upp {

int  GLCtrl::depthSize = 24;
int  GLCtrl::stencilSize = 8;
bool GLCtrl::doubleBuffering = true;
int  GLCtrl::numberOfSamples = 1;
Size GLCtrl::current_viewport;

extern void (*restore_gl_viewport__)();

void GLCtrl::DoGLPaint()
{
	MemoryIgnoreLeaksBlock __;

	glClearDepth(1);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	glEnable(GL_MULTISAMPLE);
	Size sz = GetSize();
	current_viewport = sz;
	SetCurrentViewport();
	GLPaint();
}

void GLCtrl::Init()
{
	Transparent();
#ifdef PLATFORM_WIN32
	pane.ctrl = this;
	Add(pane.SizePos());
#endif
	restore_gl_viewport__ = SetCurrentViewport;
	MemoryIgnoreNonMainLeaks();
	MemoryIgnoreNonUppThreadsLeaks(); // Linux drivers leak memory in threads
}

Image GLCtrl::MouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	if(mouseTarget) {
		return mouseTarget->MouseEvent(event, p + GetScreenView().TopLeft() - mouseTarget->GetScreenView().TopLeft(), zdelta, keyflags);
	}
	return Ctrl::MouseEvent(event, p, zdelta, keyflags);
}

void GLCtrl::SetCurrentViewport()
{
	glViewport(0, 0, (GLsizei)current_viewport.cx, (GLsizei)current_viewport.cy);
}

void GLCtrl::StdView()
{
	MemoryIgnoreLeaksBlock __;

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

Image GLCtrl::GLPane::MouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	p = p - GetScreenView().TopLeft() + ctrl->GetScreenView().TopLeft();
	return ctrl->MouseEvent(event, p, zdelta, keyflags);
}

void GLCtrl::GLPane::Pen(Point p, const PenInfo& pen, dword keyflags)
{
	(ctrl->mouseTarget ? ctrl->mouseTarget : ctrl)->Pen(p, pen, keyflags);
}

#endif


}
