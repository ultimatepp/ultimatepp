#ifdef flagLINUXGL

#include <CtrlCore/CtrlCore.h>

extern void GuiMainFn_();

NAMESPACE_UPP

XDisplay* dpy = NULL;
GLXContext glc = NULL;
XWindow root = 0;
XWindow win = 0;
XVisualInfo* vi = NULL;
int Xeventtime = 0;
int Xbuttontime = 0;
Point Xbuttonpos;
int Xbuttons = 0;

Shader alphaMagProg;
Shader blurProg;
Fbo    screenFbo0;
Fbo    screenFbo1;
String error;

#ifdef flagDEBUG
bool controlPanelActive = true;
bool consoleActive = true;
#else
bool controlPanelActive = false;
bool consoleActive = false;
#endif

bool glEndSession = false;
bool glReady = false;

int glDrawMode = DRAW_ON_IDLE;

bool GlEndSession()
{
	return glEndSession;
}

bool GlReady()
{
	return glReady;
}

void GlQuitSession()
{
//	::PostQuitMessage(0);
}

bool GlIsWaitingEvent()
{
	GuiLock __;
	return XPending(dpy) > 0;
}

bool GlProcessEvent(bool *quit)
{
	GuiLock __;
	if(!GlIsWaitingEvent()) return false;
	XEvent event;
	XNextEvent(dpy, &event);
	glWindowProc(win, &event);
	return true;
}

void GlSleep(int ms)
{
	//MsgWaitForMultipleObjects(0, NULL, FALSE, ms, QS_ALLINPUT);
}

void ActivateGlContext()
{
	//if(hRC != NULL && wglGetCurrentContext() != hRC)
	//	wglMakeCurrent(hDC, hRC);
}

void DestroyGl()
{
	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

int CreateGlWindow()
{
	dpy = XOpenDisplay(NULL);
	
	if(dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
	    return -1;
	}

	root = DefaultRootWindow(dpy);
	GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
	vi = glXChooseVisual(dpy, 0, att);
	
	if(vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		return -1;
	} 
	else {
		printf("\n\tvisual %p selected\n", (void *)vi->visualid);
	}
	
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask;	
	
	Ctrl::screenRect.Set(0, 0, 600, 600);
	
	win = XCreateWindow(dpy, root, 0, 0, Ctrl::screenRect.GetWidth(), Ctrl::screenRect.GetHeight(), 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "LinuxGL Window");
	
	return 1;
}

int CreateGlContext()
{
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	
	RLOG("OpenGL: glxCreateContext ok..");

	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		error = "Glew library initialization error: " + String((const char*) glewGetErrorString(err));
		return -6;
	}
	RLOG("OpenGL: glewInit ok..");
	
	Size sz = Ctrl::GetScreenSize();
	if(!screenFbo0.Create(sz.cx, sz.cy))
	{
		error = "Creating screen 0 fbo error: " + screenFbo0.GetError();		
		return -7;
	}

	if(!screenFbo1.Create(sz.cx, sz.cy))
	{
		error = "Creating screen 1 fbo error: " + screenFbo1.GetError();
		return -7;
	}
	
	RLOG("OpenGL: Creating fbo ok..");
	
	alphaMagProg.CompileProgram(alphaMagVert, alphaMagFrag);
	
	if(alphaMagProg.GetProgram() < 0)
	{
		error = alphaMagProg.GetError();
		return -8;
	}

	blurProg.CompileProgram(blurVert, blurFrag);
	
	if(blurProg.GetProgram() < 0)
	{
		error = blurProg.GetError();
		return -8;
	}
	
	RLOG("OpenGL: CompileProgram ok..");
	
	resources.GetFont(Arial(), true);
	resources.GetFont(Arial().Bold(), true);
	
	RLOG("OpenGL: Preloading fonts ok..");
	
	if(glDrawMode == DRAW_ON_TIMER)
	{
		//SetTimer(glHwnd, 1, 10, NULL);
		RLOG("OpenGL: SetTimer ok..");
	}
	
	glReady = true;
	return 1;
}

void SerializeGl(Stream& s)
{
	s % controlPanelActive;
	s % consoleActive;
}
 
int AppMain(int argc, const char **argv, const char **envptr)
{
	UPP::AppInit__(argc, argv, envptr);
	Ctrl::InitGl();
	int r = UPP::CreateGlWindow();
	if(r < 0)
	{
		error = Format("OpenGL window could not be created: %d\n\n%s", r, GetLastErrorMessage());
		RLOG(error);
	}
	else
	{
		r = UPP::CreateGlContext();
	}
	
	if(r < 0)
	{
		error = Format("OpenGL context could not be created: %d\n\n%s", r, error);
		RLOG(error);
	}

	if(r > 0) 
	{
		GuiMainFn_();
		UPP::Ctrl::ExitGl();
		USRLOG("---------- About to delete this log of WinGL...");
		UPP::AppExit__();
		return UPP::GetExitCode();
	}
	else
	{
		return r;
	}
}

END_UPP_NAMESPACE

#endif