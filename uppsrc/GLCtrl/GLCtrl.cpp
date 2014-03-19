#include "GLCtrl.h"

NAMESPACE_UPP

void GLCtrl::GLResize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
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
	pane.ActivateContext();
	return picking.Pick(x, y, THISBACK2(GLResize, GetSize().cx, GetSize().cy), THISBACK(GLPickingPaint));
}

#endif

END_UPP_NAMESPACE
