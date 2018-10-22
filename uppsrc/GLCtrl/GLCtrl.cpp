#include "GLCtrl.h"

namespace Upp {

int  GLCtrl::depthSize = 24;
int  GLCtrl::stencilSize = 8;
bool GLCtrl::doubleBuffering = true;
int  GLCtrl::numberOfSamples = 1;
Size GLCtrl::current_viewport;

extern void (*restore_gl_viewport__)(); // in Draw/DrawUtil.cpp

void GLCtrl::Init()
{
	NoWantFocus();
	Transparent();
	pane.ctrl = this;
	Add(pane.SizePos());
	restore_gl_viewport__ = SetCurrentViewport;
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

#ifndef GUI_GTK

Image GLCtrl::GLPane::MouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	p = p - GetScreenView().TopLeft() + ctrl->GetScreenView().TopLeft();
	return ctrl->MouseEvent(event, p, zdelta, keyflags);
}

Vector<int> GLCtrl::Pick(int x, int y)
{
//	pane.ActivateContext();
//	return picking.Pick(x, y, THISBACK2(GLResize, GetSize().cx, GetSize().cy), THISBACK(GLPickingPaint));
	Vector<int> h;
	return h;
}

#endif

}
