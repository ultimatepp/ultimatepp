#include "GLCtrl.h"

#ifdef PLATFORM_POSIX

#include <GL/glx.h>
#include <GL/gl.h>

#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture
#define Status  int

#ifdef GUI_GTK
#include <gdk/gdkx.h>
#endif

#undef Time
#undef Font
#undef Display
#undef Picture
#undef Status

namespace Upp {
	
static XVisualInfo *s_XVisualInfo;
static Colormap     s_Colormap;
static GLXContext   s_GLXContext;
static ::XDisplay  *s_Display;

void GLCtrl::Create()
{
	MemoryIgnoreLeaksBlock __;

	Ctrl *top = GetTopCtrl();
	if(!top)
		return;

#ifdef GUI_GTK
	GdkWindow *gdk = top->gdk();
	if(!gdk)
		return;

	Window w = gdk_x11_window_get_xid(gdk);
#else
	Window w = top->GetWindow();
#endif

	ONCELOCK {
	#ifdef GUI_GTK
		s_Display = GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
	#else
		s_Display = Xdisplay;
	#endif
		int samples = numberOfSamples;

		do {
			Vector<int> attr;
			attr << GLX_RGBA << GLX_DEPTH_SIZE << depthSize
			     << GLX_STENCIL_SIZE << stencilSize;
			if(doubleBuffering)
				attr << GLX_DOUBLEBUFFER;
			if(samples > 1)
				attr << GLX_SAMPLE_BUFFERS_ARB << 1 << GLX_SAMPLES_ARB << samples;
			attr << 0;
			samples >>= 1;
			s_XVisualInfo = glXChooseVisual(s_Display, DefaultScreen(s_Display), attr);
		}
		while(!s_XVisualInfo && samples > 0);
		if(!s_XVisualInfo)
			return;
		s_Colormap = XCreateColormap(s_Display, RootWindow(s_Display, s_XVisualInfo->screen), s_XVisualInfo->visual, AllocNone);
		s_GLXContext = glXCreateContext(s_Display, s_XVisualInfo, NULL, GL_TRUE);
	}
	
	if(!s_GLXContext)
		return;

	XSetWindowAttributes swa;
	swa.colormap = s_Colormap;
	swa.border_pixel = 0;
	swa.event_mask = 0;
 
	win = XCreateWindow(s_Display, w, 0, 0, 1, 1, 0,
                        s_XVisualInfo->depth, InputOutput, s_XVisualInfo->visual,
                        CWBorderPixel|CWColormap|CWEventMask, &swa);
	visible = false;
	position = Null;
}

void GLCtrl::Sync()
{
	MemoryIgnoreLeaksBlock __;

	if(win) {
		Rect r = GetScreenView() - GetTopCtrl()->GetScreenRect().TopLeft();
		bool b = IsVisible() && r.GetWidth() > 0 && r.GetHeight() > 0;
		if(b != visible) {
			visible = b;
			position = Null;
			if(b)
			    XMapWindow(s_Display, win);
			else
			    XUnmapWindow(s_Display, win);
		}
		if(r != position && visible) {
			position = r;
			XMoveResizeWindow(s_Display, win, r.left, r.top, r.Width(), r.Height());
		}
	}
}

void GLCtrl::State(int reason)
{
	switch(reason) {
	case CLOSE:
		XDestroyWindow(s_Display, win);
		break;
	case OPEN:
		Create();
	default:
		Sync();
		break;
	}
}

void GLCtrl::ExecuteGL(Event<> paint, bool swap_buffers)
{
	MemoryIgnoreLeaksBlock __;

	glXMakeCurrent(s_Display, win, s_GLXContext);

	ONCELOCK {
		glewInit();
	}

	paint();

	if(swap_buffers)
		glXSwapBuffers(s_Display, win);
	else
		glFlush();

	glXMakeCurrent(s_Display, None, NULL);
}

void GLCtrl::Paint(Draw& w)
{
	Size sz = GetSize();
	if(!s_GLXContext || sz.cx == 0 || sz.cy == 0)
		return;

	ExecuteGL([&] { DoGLPaint(); }, doubleBuffering);
}

}

#endif
