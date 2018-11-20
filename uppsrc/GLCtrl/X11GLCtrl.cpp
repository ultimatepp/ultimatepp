#include "GLCtrl.h"

namespace Upp {

#ifdef GUI_X11

GLXContext   s_GLXContext;
XVisualInfo *s_XVisualInfo;

void GLCtrl::MakeGLContext()
{
	ONCELOCK {
		Vector<int> visual;
		visual << GLX_RGBA << GLX_DEPTH_SIZE << depthSize;
		if(stencilSize > 0)
			visual << GLX_STENCIL_SIZE << stencilSize;
		if(doubleBuffering)
			visual << GLX_DOUBLEBUFFER;
		if(numberOfSamples > 1)
			visual << GLX_SAMPLE_BUFFERS_ARB << 1 << GLX_SAMPLES_ARB << numberOfSamples;
		visual << None;
		s_XVisualInfo = glXChooseVisual( (XDisplay*)Xdisplay, DefaultScreen(Xdisplay), visual);
		if(!s_XVisualInfo)
			return;
		s_GLXContext = glXCreateContext((XDisplay *)Xdisplay, s_XVisualInfo, NULL, GL_TRUE);
	}
}

void GLCtrl::GLPane::SetAttributes(unsigned long &ValueMask, XSetWindowAttributes &winAttributes)
{
	ValueMask |=
		CWBorderPixel
		| CWEventMask
		| CWSaveUnder
	;
	winAttributes.border_pixel = 0;
	winAttributes.event_mask   = ExposureMask;
	winAttributes.save_under   = XFalse;
}

void GLCtrl::GLPane::Paint(Draw &draw)
{
	if(!s_GLXContext)
		return;

	glXMakeCurrent( (XDisplay*)Xdisplay, GetWindow(), s_GLXContext);
	
	ctrl->DoGLPaint();

	if( ctrl->doubleBuffering )
		glXSwapBuffers((XDisplay*)Xdisplay, GetWindow());
	else
		glFlush();

	glXMakeCurrent( (XDisplay*)Xdisplay, GetWindow(), NULL);
}

#endif

}
