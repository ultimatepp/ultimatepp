#include "GLCtrl.h"

NAMESPACE_UPP

#ifdef GUI_X11

/////////////////////////////////////////////////////////////////////////////////////////
// Destructor
GLCtrl::GLPane::~GLPane()
{
	// If glx context is still there, destroy it
	// That can happen on unclean exit
	if(WindowContext)
	{
		glXDestroyContext( (XDisplay *)Xdisplay, WindowContext );
		WindowContext = NULL;
	}
} // END Destructor class GLCtrl::GLPane

/////////////////////////////////////////////////////////////////////////////////////////
// Ovverridden method to choose the correct visual
XVisualInfo *GLCtrl::GLPane::CreateVisual(void)
{
	Vector<int> visual;
	visual << GLX_RGBA << GLX_DEPTH_SIZE << ctrl->depthSize;

	if( ctrl->stencilSize > 0 )
		visual << GLX_STENCIL_SIZE << ctrl->stencilSize;

	if( ctrl->doubleBuffering )
		visual << GLX_DOUBLEBUFFER;

	if( ctrl->multiSampleBuffering && ctrl->numberOfSamples > 1 )
	{
		visual << GLX_SAMPLE_BUFFERS_ARB << 1 << GLX_SAMPLES_ARB << ctrl->numberOfSamples;
	}

	visual << None;

	// Try to find a visual
	XVisualInfo *visualInfo = NULL;
	visualInfo = glXChooseVisual( (XDisplay*)Xdisplay, DefaultScreen(Xdisplay), visual );

	if( visualInfo == NULL )
	{
		SetError(true);
		SetErrorMessage("GLCtrl::GLPane : Impossible to find a suitable visual.");
	}
	
	return visualInfo;
} // END GLCtrl::GLPane::CreateVisual()

/////////////////////////////////////////////////////////////////////////////////////////
// Overridden method for attribute setting
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
} // END GLCtrl::GLPane::SetAttributes()


/////////////////////////////////////////////////////////////////////////////////////////
// Activates current OpenGL context
void GLCtrl::GLPane::ActivateContext()
{
	if (WindowContext != NULL && glXGetCurrentContext() != WindowContext)
		glXMakeCurrent( (XDisplay*)Xdisplay, GetWindow(), WindowContext );
}

/////////////////////////////////////////////////////////////////////////////////////////
// Overridden method to create OpenGL context
void GLCtrl::GLPane::AfterInit(bool Error)
{
	// Gets the activw XVisualInfo
	XVisualInfo visualInfo = GetVisualInfo();
	
	// Create an OpenGL rendering context
	WindowContext = glXCreateContext
	(
		(XDisplay *)Xdisplay,
		&visualInfo,
		NULL,		// No sharing of display lists
		GL_TRUE		// Direct rendering if possible
	);

	if( WindowContext == NULL )
	{
		SetErrorMessage("GLCtrl::GLPane : Error creating GLX context.");
		SetError(true);
	}
	
	// Activate the created glxwindow
	glXMakeCurrent( (XDisplay*)Xdisplay, GetWindow(), WindowContext );
	
	// Call user init function
	ctrl->GLInit();

	// Calls resize and paint functions
	ctrl->GLResize( GetSize().cx, GetSize().cy );
	ctrl->GLPaint();
} // END GLCtrl::GLPane::AfterInit()

/////////////////////////////////////////////////////////////////////////////////////////
// Overridden method to destroy OpenGL context
void GLCtrl::GLPane::BeforeTerminate(void)
{
	// Calls user terminate function
	ctrl->GLDone();
	
	// Resets context and destroys it
	glXMakeCurrent( (XDisplay*)Xdisplay, None, NULL );
	glXDestroyContext( (XDisplay *)Xdisplay, WindowContext );
	WindowContext = NULL;
} // END GLCtrl::GLPane::BeforeTerminate()

/////////////////////////////////////////////////////////////////////////////////////////
// Overridden method to resize GL windows
void GLCtrl::GLPane::Layout()
{
	// Activates the current context
	ActivateContext();
	
	if (glXGetCurrentContext() == NULL)
		return;
	
	// Calls user resize hook
	ctrl->GLResize(GetSize().cx, GetSize().cy);
} // END GLCtrl::GLPane::Resize()

/////////////////////////////////////////////////////////////////////////////////////////
// Paint method - with graphic context
void GLCtrl::GLPane::Paint(Draw &draw)
{
	// Activates the current context
	ActivateContext();

	// Calls user paint hook
	ctrl->GLPaint();

	// Swap buffers or flush as needed
	if( ctrl->doubleBuffering )
		glXSwapBuffers( (XDisplay*)Xdisplay, GetWindow() ); // Buffer swap does implicit glFlush
	else
		glFlush();
} // END GLCtrl::GLPane::Paint()

#endif

END_UPP_NAMESPACE
