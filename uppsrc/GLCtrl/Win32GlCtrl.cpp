#include "GLCtrl.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#pragma comment( lib, "opengl32.lib" )	// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )		// Search For GLu32.lib While Linking

GLCtrl::GLCtrl()
{
	hDC = NULL;
	hRC = NULL;
}

GLCtrl::~GLCtrl()
{
	CloseGL();
}

void GLCtrl::OpenGL()
{
	HWND hwnd = GetHWND();
	if(!hwnd)
		return;
	hDC = ::GetDC(hwnd);
	if(!hDC)
		return;
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | 0x00008000;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pf = ChoosePixelFormat(hDC, &pfd);
	if(!pf) {
		CloseGL();
		return;
	}
	if(!SetPixelFormat(hDC, pf, &pfd)) {
		CloseGL();
		return;
	}
	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	hRC = wglCreateContext(hDC);
}

void GLCtrl::CloseGL()
{
	if(hRC)
	    wglDeleteContext(hRC);
	if(hDC)
	    ReleaseDC(GetHWND(), hDC);
}

void GLCtrl::State(int reason)
{
	if(reason == CLOSE)
		CloseGL();
	DHCtrl::State(reason);
	if(reason == OPEN)
		OpenGL();
}

void GLCtrl::GLPaint()
{
	WhenGLPaint();
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

LRESULT GLCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_PAINT && hDC && hRC) {
		wglMakeCurrent(hDC, hRC);
		GLPaint();
		glFlush();
		glFinish();
		SwapBuffers(hDC);
		wglMakeCurrent(NULL, NULL);
		PAINTSTRUCT ps;
		BeginPaint(GetHWND(), &ps);
		EndPaint(GetHWND(), &ps);
		return 0;
	}
	if(message == WM_ERASEBKGND)
		return 1;
	return DHCtrl::WindowProc(message, wParam, lParam);
}

#endif

END_UPP_NAMESPACE
