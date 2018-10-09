#include "GLCtrl.h"

namespace Upp {

#ifdef PLATFORM_WIN32

#pragma comment( lib, "opengl32.lib" )	// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )		// Search For GLu32.lib While Linking

void InitializeGlew()
{
	ONCELOCK { // create temporary window to initialize glew
		HWND hWND = CreateWindow("UPP-CLASS-A", "Fake Window",
		                         WS_CAPTION|WS_SYSMENU|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		                         0, 0, 1, 1, NULL, NULL,
		                         NULL, NULL);
		if(!hWND)
			return;
		HDC hDC = ::GetDC(hWND);
		if(!hDC)
			return;
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_GENERIC_FORMAT;
//		if (ctrl->doubleBuffering) pfd.dwFlags |= PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cAlphaBits = 8;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;
		int pixelFormatID = ChoosePixelFormat(hDC, &pfd);
	
		DescribePixelFormat(hDC, pixelFormatID, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
		SetPixelFormat(hDC, pixelFormatID, &pfd);

		HGLRC hRC = wglCreateContext(hDC);
		
		wglMakeCurrent(hDC, hRC);
	
		glewInit();
	
		wglMakeCurrent(NULL, NULL);
	    wglDeleteContext(hRC);
	    ReleaseDC(hWND, hDC);
	    DestroyWindow(hWND);
	}
}

void GLCtrl::GLPane::Init()
{
	InitializeGlew();

	HWND hwnd = GetHWND();
	if(!hwnd)
		return;
	hDC = ::GetDC(hwnd);
	if(!hDC)
		return;
	int pixelFormatID;
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));

	Vector<int> attr;
	attr
		<< WGL_DRAW_TO_WINDOW_ARB << GL_TRUE
		<< WGL_SUPPORT_OPENGL_ARB << GL_TRUE
		<< WGL_DOUBLE_BUFFER_ARB << GL_TRUE
		<< WGL_PIXEL_TYPE_ARB << WGL_TYPE_RGBA_ARB
		<< WGL_ACCELERATION_ARB << WGL_FULL_ACCELERATION_ARB
		<< WGL_COLOR_BITS_ARB << 32
		<< WGL_ALPHA_BITS_ARB << 8
		<< WGL_DEPTH_BITS_ARB << ctrl->depthSize
		<< WGL_STENCIL_BITS_ARB << ctrl->stencilSize
	;
	if(ctrl->numberOfSamples > 1)
		attr
			<< WGL_SAMPLE_BUFFERS_ARB << GL_TRUE
			<< WGL_SAMPLES_ARB << ctrl->numberOfSamples
		;
	attr << 0;
	UINT numFormats;
	if(!wglChoosePixelFormatARB(hDC, attr, NULL, 1, &pixelFormatID, &numFormats)) {
		Destroy();
		return;
	}

	DescribePixelFormat(hDC, pixelFormatID, sizeof(pfd), &pfd);
	if(!SetPixelFormat(hDC, pixelFormatID, &pfd)) {
		Destroy();
		return;
	}
	hRC = wglCreateContext(hDC);
	ActivateContext();
	glEnable(GL_MULTISAMPLE);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		ctrl->GLPaint();
		if(ctrl->doubleBuffering)
			SwapBuffers(hDC);
		else
			glFlush();
		EndPaint(GetHWND(), &ps);
		return 0;
	}
	
	return DHCtrl::WindowProc(message, wParam, lParam);
}

#endif

}
