#ifndef _LinuxGl_LinuxGl_h_
#define _LinuxGl_LinuxGl_h_

#define GUI_LINUXGL

#include <CoreGl/CoreGl.h>
#include <LinuxGl/stdids.h>

NAMESPACE_UPP

extern bool  glEndSession;
extern XDisplay* dpy;
extern GLXContext glc;
extern XWindow root;
extern XWindow win;
extern XVisualInfo* vi;
extern int Xeventtime;
extern int Xbuttontime;
extern Point Xbuttonpos;
extern int Xbuttons;

extern Shader alphaMagProg;
extern Shader blurProg;
extern Fbo screenFbo0;
extern Fbo screenFbo1;
extern bool glReady;
extern String error;
extern int glDrawMode;

int CreateGlWindow();
int CreateGlContext();
void ActivateGlContext();
void DestroyGl();
void SerializeGl(Stream& s); 
void glWindowProc(XWindow win, XEvent* event);
int64 GetHighTickCount();

extern bool controlPanelActive;
extern bool consoleActive;

bool GlIsWaitingEvent();
bool GlProcessEvent(bool *quit);
void GlSleep(int ms);
bool GlReady();
bool GlEndSession();
void GlQuitSession();

END_UPP_NAMESPACE

#endif
