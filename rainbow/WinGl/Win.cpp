#include <CtrlCore/CtrlCore.h>

#ifdef GUI_WINGL
extern void GuiMainFn_();


NAMESPACE_UPP

HWND   glHWND;
HDC    hDC;
HGLRC  hRC;

bool glEndSession = false;

bool GlEndSession()
{
	return glEndSession;
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

void ActivateGLContext()
{
	if (hRC != NULL && wglGetCurrentContext() != hRC)
		wglMakeCurrent(hDC, hRC);
}

void DestroyGL()
{
	if (hDC != NULL && hRC != NULL)
	{
		ActivateGLContext();
		wglMakeCurrent(NULL, NULL);
	}
	
	if(hRC)
	    wglDeleteContext(hRC);
	if(hDC)
	    ReleaseDC(glHWND, hDC);
}

int GlInit(HINSTANCE hInstance)
{
	GuiLock __;
	
	Ctrl::InitGl();
	
	WNDCLASSW  wc;
	Zero(wc);
	wc.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)glWindowProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)NULL;
	wc.lpszClassName = L"UPP-FB-CLASS";
	RegisterClassW(&wc);
	glHWND = CreateWindowW(
		L"UPP-FB-CLASS", L"",
		WS_OVERLAPPED | WS_VISIBLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
	   	CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	    NULL, NULL, hInstance, NULL);
	    
	if(!glHWND)
		return -1;
	                       
	hDC = ::GetDC(glHWND);
	if(!hDC)
		return -2;
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
		return -3;
	}
	if(!SetPixelFormat(hDC, pf, &pfd)) {
		RLOG("OpenGL: SetPixelFormat error");
		DestroyGL();
		return -4;
	}
	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	hRC = wglCreateContext(hDC);
	if(!hRC)
	{
		RLOG("OpenGL: wglCreateContext error");
		DestroyGL();
		return -5;
	}
	if(!wglMakeCurrent(hDC, hRC))
	{
		RLOG("OpenGL: wglMakeCurrent error");
		DestroyGL();
		return -6;
	}
	//ActivateGLContext();
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		RLOG("OpenGL: Glew library initialization error: " + String((const char*) glewGetErrorString(err)));
		DestroyGL();
		return -7;
	}
	
	//InitializeShaders();
	wglSwapIntervalEXT(0);
	//SetTimeCallback(-10, THISBACK(Repaint), 1);
	                       
	SetTimer(glHWND, 1, 10, NULL);
	return 1;
}

Vector<WString>& coreCmdLine__();
Vector<WString> SplitCmdLine__(const char *cmd);

int AppMain(HINSTANCE hInstance, LPSTR lpCmdLine)
{
	UPP::coreCmdLine__() = UPP::SplitCmdLine__(UPP::FromSystemCharset(lpCmdLine));
	UPP::AppInitEnvironment__();
	int r = UPP::GlInit(hInstance);
	if(r > 0) 
	{
		GuiMainFn_();
		UPP::Ctrl::CloseTopCtrls();
		UPP::UsrLog("---------- About to delete this log of WinGL...");
		UPP::DeleteUsrLog();
		UPP::DestroyGL();
		return UPP::GetExitCode();
	} else {
		::MessageBox(NULL, Format("OpenGL window could not be initialized: %d", r), NULL, MB_ICONEXCLAMATION | MB_OK);
		return r;
	}
}

END_UPP_NAMESPACE

#endif