#include "GLCtrl.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#pragma comment( lib, "opengl32.lib" )	// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )		// Search For GLu32.lib While Linking

void GLCtrl::GLPane::Init()
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
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_GENERIC_FORMAT;
	if (ctrl->doubleBuffering) pfd.dwFlags |= PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = ctrl->depthSize;
	pfd.cStencilBits = ctrl->stencilSize;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pf = ChoosePixelFormat(hDC, &pfd);
	if(!pf) {
		Destroy();
		return;
	}
	if(!SetPixelFormat(hDC, pf, &pfd)) {
		Destroy();
		return;
	}
	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	hRC = wglCreateContext(hDC);
	
	if (!hRC)
		return;
	
	ActivateContext();
	ctrl->GLInit();
	ctrl->GLResize(GetSize().cx, GetSize().cy);
	ctrl->GLPaint();
}

void GLCtrl::GLPane::Destroy()
{
	if (hDC != NULL && hRC != NULL)
	{
		ActivateContext();
		ctrl->GLDone();
		wglMakeCurrent(NULL, NULL);
	}
	
	if(hRC)
	    wglDeleteContext(hRC);
	if(hDC)
	    ReleaseDC(GetHWND(), hDC);
}

void GLCtrl::GLPane::ActivateContext()
{
	if (hRC != NULL && wglGetCurrentContext() != hRC)
		wglMakeCurrent(hDC, hRC);
}

void GLCtrl::GLPane::State(int reason)
{
	if (reason == CLOSE)
		Destroy();
	
	if ((reason == LAYOUTPOS || reason == POSITION) && hDC != NULL && hRC != NULL)
	{
		ActivateContext();
		ctrl->GLResize(GetSize().cx, GetSize().cy);
	}
	
	DHCtrl::State(reason);
	
	if (reason == OPEN)
		Init();
}

LRESULT GLCtrl::GLPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if((message == WM_PAINT || message == WM_SIZE || message == WM_ERASEBKGND) && hDC && hRC) 
	{
		PAINTSTRUCT ps;
		BeginPaint(GetHWND(), &ps);
		ActivateContext();
		ctrl->GLPaint();
		ctrl->doubleBuffering ? SwapBuffers(hDC) : glFlush();
		EndPaint(GetHWND(), &ps);
		return 0;
	}
	
	return DHCtrl::WindowProc(message, wParam, lParam);
}

#endif

END_UPP_NAMESPACE
