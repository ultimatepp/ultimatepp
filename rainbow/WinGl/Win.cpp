#include "WinFb.h"

NAMESPACE_UPP

HWND glHWND;

bool glEndSession;

bool GlEndSession()
{
	return fbEndSession;
}

bool GlIsWaitingEvent()
{
	MSG msg;
	return PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
}

bool GlProcessEvent(bool *quit)
{
	MSG msg;
	if(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT && quit)
			*quit = true;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		return true;
	}
	return false;
}

void GlSleep(int ms)
{
	MsgWaitForMultipleObjects(0, NULL, FALSE, ms, QS_ALLINPUT);
}

void GlInit(HINSTANCE hInstance)
{
	GuiLock __;
	
	Ctrl::InitGl();
	
	WNDCLASSW  wc;
	Zero(wc);
	wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)fbWindowProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)NULL;
	wc.lpszClassName = L"UPP-FB-CLASS";
	RegisterClassW(&wc);
	fbHWND = CreateWindowW(L"UPP-FB-CLASS", L"", WS_OVERLAPPED|WS_VISIBLE|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU,
	                       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	                       NULL, NULL, hInstance, NULL);
	                       
	                       
	hDC = ::GetDC(GetHWND());
	if(!hDC)
		return;
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_SUPPORT_COMPOSITION;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pf = ChoosePixelFormat(hDC, &pfd);
	if(!pf) {
		
		RLOG("OpenGL: ChoosePixelFormat error");
		DestroyGL();
		return;
	}
	if(!SetPixelFormat(hDC, pf, &pfd)) {
		RLOG("OpenGL: SetPixelFormat error");
		DestroyGL();
		return;
	}
	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	hRC = wglCreateContext(hDC);
	if(!hRC)
	{
		RLOG("OpenGL: wglCreateContext error");
		DestroyGL();
		return;
	}
	if(!wglMakeCurrent(hDC, hRC))
	{
		RLOG("OpenGL: wglMakeCurrent error");
		DestroyGL();
		return;
	}
	//ActivateGLContext();
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		RLOG("OpenGL: Glew library initialization error: " + String((const char*) glewGetErrorString(err)));
		DestroyGL();
		return;
	}
	
	//InitializeShaders();
	//wglSwapIntervalEXT(0);
	//SetTimeCallback(-10, THISBACK(Repaint), 1);
	                       
	//SetTimer(fbHWND, 1, 10, NULL);

}

END_UPP_NAMESPACE
