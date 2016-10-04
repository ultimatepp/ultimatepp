#include "GLCtrl.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32


#pragma comment( lib, "opengl32.lib" )	// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )		// Search For GLu32.lib While Linking

GLCtrl::GLCtrl()
{
	hDC = NULL;
	hRC = NULL;
	glpane.ctrl = this;
	Add(glpane.SizePos());
}

GLCtrl::~GLCtrl()
{
	CloseGL();
}

void GLCtrl::OpenGL()
{
	HWND hwnd = glpane.GetHWND();
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
	
	if (!hRC)
		return;
	
	wglMakeCurrent(hDC, hRC);
	GLInit();
	GLResize(GetSize().cx, GetSize().cy);
}

void GLCtrl::CloseGL()
{
	if (hDC != NULL && hRC != NULL)
	{
		wglMakeCurrent(hDC, hRC);
		GLDone();
		wglMakeCurrent(NULL, NULL);
	}
	
	if(hRC)
	    wglDeleteContext(hRC);
	if(hDC)
	    ReleaseDC(glpane.GetHWND(), hDC);
}

void GLCtrl::GLPaint()
{
	WhenGLPaint();
}

void GLCtrl::GLPane::State(int reason)
{
	if (reason == CLOSE)
		ctrl->CloseGL();
	
	if (reason == LAYOUTPOS && ctrl->hDC != NULL && ctrl->hRC != NULL)
	{
		wglMakeCurrent(ctrl->hDC, ctrl->hRC);
		ctrl->GLResize(GetSize().cx, GetSize().cy);
		wglMakeCurrent(NULL, NULL);
	}
	
	DHCtrl::State(reason);
	
	if (reason == OPEN)
		ctrl->OpenGL();
}

LRESULT GLCtrl::GLPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_PAINT && ctrl->hDC && ctrl->hRC) 
	{
		PAINTSTRUCT ps;
		BeginPaint(GetHWND(), &ps);
		wglMakeCurrent(ctrl->hDC, ctrl->hRC);
		ctrl->GLPaint();
		glFlush();
		glFinish();
		SwapBuffers(ctrl->hDC);
		wglMakeCurrent(NULL, NULL);
		EndPaint(GetHWND(), &ps);
		return 0;
	}
	else if(message == WM_ERASEBKGND)
		return 1;
	
	return DHCtrl::WindowProc(message, wParam, lParam);
}

Vector<int> GLCtrl::Pick(int x, int y)
{
	wglMakeCurrent(hDC, hRC);
	Vector<int> result = _picking.Pick(x, y, THISBACK2(GLResize, GetSize().cx, GetSize().cy), THISBACK(GLPickingPaint));
	wglMakeCurrent(NULL, NULL);
	
	return result;
}

#endif

END_UPP_NAMESPACE
