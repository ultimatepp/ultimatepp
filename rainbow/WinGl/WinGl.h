#ifndef _WinGl_WinGl_h_
#define _WinGl_WinGl_h_

#define GUI_WINGL

#include <CoreGl/CoreGl.h>

NAMESPACE_UPP

extern bool  glEndSession;
extern HWND  hWnd;
extern HWND  glHwnd;
extern HDC   hDC;
extern HGLRC hRC;
extern Shader alphaMagProg;
extern Shader blurProg;
extern Fbo screenFbo0;
extern Fbo screenFbo1;
extern bool glReady;
extern String error;
extern int glDrawMode;

int CreateGlWindow(HINSTANCE hInstance);
int CreateGlContext();
void ActivateGlContext();
void DestroyGl();
void SerializeGl(Stream& s); 
LRESULT CALLBACK glWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
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
