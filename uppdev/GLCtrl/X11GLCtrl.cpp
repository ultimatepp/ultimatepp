#include "GLCtrl.h"

NAMESPACE_UPP

#ifdef PLATFORM_X11

/////////////////////////////////////////////////////////////////////////////////////////
// Static members initialization
int GLCtrl::GLPane::Instances = 0;
int GLCtrl::GLPane::ContextActivated = 0;

/////////////////////////////////////////////////////////////////////////////////////////
// Constructor
GLCtrl::GLPane::GLPane(int depthsize, int stencilsize, bool doublebuffer,
                       bool multisamplebuffering, int numberofsamples )
{
	// Sets the current instance number and updates total instances
	InstanceNum = ++Instances;
	
	WindowContext = NULL;

	DepthSize        = depthsize;
	StencilSize      = stencilsize;
	DoubleBuffering  = doublebuffer;
	NumberOfSamples  = numberofsamples;
} // END Constructor class GLCtrl::GLPane

/////////////////////////////////////////////////////////////////////////////////////////
// Destructor
GLCtrl::GLPane::~GLPane()
{
	// If glx context is still there, destroy it
	// That can happen on unclean exit
	if(WindowContext)
	{
		glXDestroyContext( (Display *)Xdisplay, WindowContext );
		WindowContext = NULL;
	}
} // END Destructor class GLCtrl::GLPane

/////////////////////////////////////////////////////////////////////////////////////////
// Ovverridden method to choose the correct visual
XVisualInfo *GLCtrl::GLPane::CreateVisual(void)
{
	Vector<int> visual;
	visual << GLX_RGBA << GLX_DEPTH_SIZE << DepthSize;

	if( StencilSize > 0 )
		visual << GLX_STENCIL_SIZE << StencilSize;

	if( DoubleBuffering )
		visual << GLX_DOUBLEBUFFER;

	if( MultiSampleBuffering && NumberOfSamples > 1 )
	{
		visual << GLX_SAMPLE_BUFFERS_ARB << 1 << GLX_SAMPLES_ARB << NumberOfSamples;
	}

	visual << None;

	// Try to find a visual
	XVisualInfo *visualInfo = NULL;
	visualInfo = glXChooseVisual( (Display*)Xdisplay, DefaultScreen(Xdisplay), visual );

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
	if( Instances > 0 && ContextActivated != InstanceNum )
	{
		glXMakeCurrent( (Display*)Xdisplay, GetWindow(), WindowContext );
		ContextActivated = InstanceNum;
	}
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
		(Display *)Xdisplay,
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
	glXMakeCurrent( (Display*)Xdisplay, GetWindow(), WindowContext );
	
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
	glXMakeCurrent( (Display*)Xdisplay, None, NULL );
	glXDestroyContext( (Display *)Xdisplay, WindowContext );
	WindowContext = NULL;
} // END GLCtrl::GLPane::BeforeTerminate()

/////////////////////////////////////////////////////////////////////////////////////////
// Overridden method to resize GL windows
void GLCtrl::GLPane::Resize(int x, int y)
{
	// Activates the current context
	ActivateContext();
	
	// Calls user resize hook
	ctrl->GLResize(x, y);
} // END GLCtrl::GLPane::Resize()

/////////////////////////////////////////////////////////////////////////////////////////
// Internal OpenGL Paint method
void GLCtrl::GLPane::doPaint(void)
{
	// Activates the current context
	ActivateContext();

	// Calls user paint hook
	ctrl->GLPaint();

	// Swap buffers or flush as needed
	if( DoubleBuffering )
		glXSwapBuffers( (Display*)Xdisplay, GetWindow() ); // Buffer swap does implicit glFlush
	else
		glFlush();
} // END GLCtrl::GLPane::doPaint()

/////////////////////////////////////////////////////////////////////////////////////////
// Paint method - with graphic context
void GLCtrl::GLPane::Paint(Draw &draw)
{
	// Calls internal OpenGL Paint method
	doPaint();
} // END GLCtrl::GLPane::Paint()

Vector<int> GLCtrl::GLPane::Pick(int x, int y)
{
	ActivateContext();
	
	Vector<int> result = _picking.Pick(x, y,
	                                  callback2(ctrl, &GLCtrl::GLResize, GetSize().cx, GetSize().cy), 
	                                  callback(ctrl, &GLCtrl::GLPickingPaint));
	
	return result;
}

GLCtrl::GLCtrl(int depthsize, int stencilsize, bool doublebuffer, bool multisamplebuffering, int numberofsamples)
:	pane(depthsize, stencilsize, doublebuffer, multisamplebuffering, numberofsamples)
{
	pane.ctrl = this;
	Add(pane.SizePos());
}

#endif

END_UPP_NAMESPACE
