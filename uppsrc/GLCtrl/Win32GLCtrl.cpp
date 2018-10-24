#include "GLCtrl.h"

namespace Upp {

#ifdef PLATFORM_WIN32

#pragma comment( lib, "opengl32.lib" )	// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )		// Search For GLu32.lib While Linking

static HGLRC                 s_openGLContext; // we only have single OpenGL context for all windows
static PIXELFORMATDESCRIPTOR s_pfd;
static int                   s_pixelFormatID;

void MakeGLContext(int depthBits, int stencilBits, int samples)
{
	ONCELOCK {
		for(int pass = 0; pass < 2; pass++) {
			HWND hWND = CreateWindow("UPP-CLASS-A", "Fake Window",
			                         WS_CAPTION|WS_SYSMENU|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
			                         0, 0, 1, 1, NULL, NULL,
			                         NULL, NULL);
			if(!hWND)
				return;
			HDC hDC = ::GetDC(hWND);
			if(!hDC)
				return;
			memset(&s_pfd, 0, sizeof(s_pfd));
			if(pass == 0) {
				s_pfd.nSize = sizeof(s_pfd);
				s_pfd.nVersion = 1;
				s_pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_GENERIC_ACCELERATED|PFD_GENERIC_FORMAT;
				s_pfd.iPixelType = PFD_TYPE_RGBA;
				s_pfd.cColorBits = 32;
				s_pfd.cAlphaBits = 8;
				s_pfd.cDepthBits = 24;
				s_pfd.cStencilBits = 8;
				s_pfd.iLayerType = PFD_MAIN_PLANE;
				s_pixelFormatID = ChoosePixelFormat(hDC, &s_pfd);
			}
			else {
				Vector<int> attr;
				attr
					<< WGL_DRAW_TO_WINDOW_ARB << GL_TRUE
					<< WGL_SUPPORT_OPENGL_ARB << GL_TRUE
					<< WGL_DOUBLE_BUFFER_ARB << GL_TRUE
					<< WGL_PIXEL_TYPE_ARB << WGL_TYPE_RGBA_ARB
					<< WGL_ACCELERATION_ARB << WGL_FULL_ACCELERATION_ARB
					<< WGL_COLOR_BITS_ARB << 32
					<< WGL_ALPHA_BITS_ARB << 8
					<< WGL_DEPTH_BITS_ARB << depthBits
					<< WGL_STENCIL_BITS_ARB << stencilBits
				;
				if(samples > 1)
					attr
						<< WGL_SAMPLE_BUFFERS_ARB << GL_TRUE
						<< WGL_SAMPLES_ARB << samples
					;
				attr << 0;
				UINT numFormats;
				if(!wglChoosePixelFormatARB(hDC, attr, NULL, 1, &s_pixelFormatID, &numFormats))
					return;
			}
			
			DescribePixelFormat(hDC, s_pixelFormatID, sizeof(PIXELFORMATDESCRIPTOR), &s_pfd);
			if(!SetPixelFormat(hDC, s_pixelFormatID, &s_pfd))
				return;
	
			if(pass == 0) {
				HGLRC hRC = wglCreateContext(hDC);
				wglMakeCurrent(hDC, hRC);
				glewInit();
				wglMakeCurrent(NULL, NULL);
			    wglDeleteContext(hRC);
			}
			else
				s_openGLContext = wglCreateContext(hDC);

		    ReleaseDC(hWND, hDC);
		    DestroyWindow(hWND);
		}
	}
}

void GLCtrl::CreateContext()
{
	MakeGLContext(depthSize, stencilSize, numberOfSamples);
}

void GLCtrl::GLPane::State(int reason)
{
	DHCtrl::State(reason);
	
	if(reason == OPEN) {
		HWND hwnd = GetHWND();
		GLCtrl::CreateContext();
		HDC hDC = GetDC(hwnd);
		if(!SetPixelFormat(hDC, s_pixelFormatID, &s_pfd))
			return;
		ReleaseDC(hwnd, hDC);
	}
}

LRESULT GLCtrl::GLPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_PAINT && s_openGLContext) {
		static int i = 0;
		PAINTSTRUCT ps;
		HWND hwnd = GetHWND();
		BeginPaint(hwnd, &ps);
		HDC hDC = ps.hdc;
		wglMakeCurrent(hDC, s_openGLContext);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
		glEnable(GL_MULTISAMPLE);
		Size sz = GetSize();
		current_viewport = sz;
		SetCurrentViewport();
		ctrl->GLPaint();
		if(ctrl->doubleBuffering)
			SwapBuffers(hDC);
		else
			glFlush();
		wglMakeCurrent(NULL, NULL);
		ReleaseDC(hwnd, hDC);
		EndPaint(hwnd, &ps);
		return 0;
	}
	
	return DHCtrl::WindowProc(message, wParam, lParam);
}

#endif

}
