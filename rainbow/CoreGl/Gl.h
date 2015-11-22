#ifndef _CoreGl_Gl_h_
#define _CoreGl_Gl_h_

#ifdef flagLINUXGL
#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture
#define Screen  XScreen
#define Window  XWindow
#define Complex XComplex
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#endif

#include <CoreGl/glew.h>

#ifdef flagWINGL
#include <CoreGl/wglew.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#ifdef flagLINUXGL
#include <GL/glx.h>
#endif

#ifdef flagLINUXGL
#undef Picture
#undef Time
#undef Font
#undef Display
#undef Screen
#undef Window
#undef Complex
#endif

#endif
