#include "GLCtrl_glad.h"

namespace Upp {

#ifdef PLATFORM_WIN32

#pragma comment( lib, "opengl32.lib" )	// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )		// Search For GLu32.lib While Linking


static HGLRC                 s_openGLContext; // we only have single OpenGL context for all windows
static PIXELFORMATDESCRIPTOR s_pfd;
static int                   s_pixelFormatID;
static HWND hWND;



void MakeWGLContext(int depthBits, int stencilBits, int samples)
{
	ONCELOCK {
		for(int pass = 0; pass < 2; pass++) {
			hWND = CreateWindow("UPP-CLASS-A", "Fake Window",WS_CAPTION|WS_SYSMENU|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,0, 0, 1, 1, NULL, NULL,NULL, NULL);
			if(!hWND){
				RLOG("Error on OpenGL window creation");
				return;
			}
			HDC hDC = ::GetDC(hWND);
			if(!hDC){
				RLOG("Error during recuperation of hDC");
				return;
			}
			memset(&s_pfd, 0, sizeof(s_pfd));
			if(pass == 0) {
				s_pfd.nSize = sizeof(s_pfd);
				s_pfd.nVersion = 1;
				s_pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_GENERIC_ACCELERATED|PFD_GENERIC_FORMAT|PFD_DOUBLEBUFFER_DONTCARE;
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
					attr<< WGL_SAMPLE_BUFFERS_ARB << GL_TRUE<< WGL_SAMPLES_ARB << samples;
					attr << 0;
				UINT numFormats;
				if(!wglChoosePixelFormatARB(hDC, attr, NULL, 1, &s_pixelFormatID, &numFormats)){
					RLOG("Error during execution of wglChoosePixelFormatARB(...)");
					return;
				}
			}
			
			DescribePixelFormat(hDC, s_pixelFormatID, sizeof(PIXELFORMATDESCRIPTOR), &s_pfd);
			if(!SetPixelFormat(hDC, s_pixelFormatID, &s_pfd)){
				RLOG("Error during exeuction of SetPixelFormat(...)");
				return;
			}
			s_openGLContext = wglCreateContext(hDC);
			if(!s_openGLContext){
				RLOG("Error during creation of OpenGL context using wglCreateContext(...)");
			}
			bool enhanced_mode=false;
			if(pass == 0) {
				HGLRC hRC = wglCreateContext(hDC);
				wglMakeCurrent(hDC, s_openGLContext);
				if(!gladLoadGL()){
					RLOG("Failed to load all OpenGL functions");
					return;
				}
				if(!gladLoadWGL(hDC)){
					
					return;
				}
				RLOG(Upp::String("OpenGL ") + AsString(GLVersion.major) + Upp::String(".") + AsString(GLVersion.minor) + Upp::String(" used"));
				if ("GLAD_GL_VERSION_4_0"){
					enhanced_mode=true;
				}else{
					RLOG("OpenGL 4.0 or higher version is necessary !");
					exit(-1);
				}
				wglMakeCurrent(NULL, NULL);
			}
			dword id = GetCurrentThreadId();
			
		    ReleaseDC(hWND, hDC);
		    DestroyWindow(hWND);
		    
			if(!enhanced_mode) break; // In basic mode, this is it.
		}
	}
}

void GLCtrl_glad::CreateContext()
{
	MakeWGLContext(depthSize, stencilSize, numberOfSamples);
}

void GLCtrl_glad::GLPane::State(int reason)
{
	DHCtrl::State(reason);
	
	if(reason == OPEN) {
		HWND hwnd = GetHWND();
		GLCtrl_glad::CreateContext();
		HDC hDC = GetDC(hwnd);
		if(!SetPixelFormat(hDC, s_pixelFormatID, &s_pfd))
			return;
		ReleaseDC(hwnd, hDC);
	}
}



void GLCtrl_glad::GLPane::ExecuteGL(HDC hDC, Event<> paint, bool swap_buffers)
{
	ONCELOCK{
		wglMakeCurrent(hDC, s_openGLContext);
	}
	paint();
	if(swap_buffers)
		SwapBuffers(hDC);
	else
		glFlush();
	//wglMakeCurrent(NULL, NULL);
}

void GLCtrl_glad::GLPane::ExecuteGL(Event<> paint, bool swap_buffers)
{
	HWND hwnd = GetHWND();
	GLCtrl_glad::CreateContext();
	HDC hDC = GetDC(hwnd);
	ExecuteGL(hDC, paint, swap_buffers);
	ReleaseDC(hwnd, hDC);
}

LRESULT GLCtrl_glad::GLPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_PAINT && s_openGLContext) {
		PAINTSTRUCT ps;
		HWND hwnd = GetHWND();
		BeginPaint(hwnd, &ps);
		ExecuteGL(ps.hdc, [&] { ctrl->DoGLPaint(); }, ctrl->doubleBuffering);
		ReleaseDC(hwnd, ps.hdc);
		EndPaint(hwnd, &ps);
		return 0;
	}
	
	return DHCtrl::WindowProc(message, wParam, lParam);
}

void GLCtrl_glad::ExecuteGL(Event<> paint, bool swap_buffers)
{
	pane.ExecuteGL(paint, swap_buffers);
}

#endif

}
