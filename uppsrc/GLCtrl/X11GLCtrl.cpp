#include "GLCtrl.h"

NAMESPACE_UPP

#ifdef PLATFORM_X11

/////////////////////////////////////////////////////////////////////////////////////////
// Static members initialization
int GLCtrl::Instances = 0;
int GLCtrl::ContextActivated = 0;

/////////////////////////////////////////////////////////////////////////////////////////
// Constructor
GLCtrl::GLCtrl( int depthsize, int stencilsize, bool doublebuffer,
				bool multisamplebuffering, int numberofsamples )
{
	// Sets the current instance number and updates total instances
	InstanceNum = ++Instances;
	
	WindowContext = NULL;

	DepthSize        = depthsize;
	StencilSize      = stencilsize;
	DoubleBuffering  = doublebuffer;
	NumberOfSamples  = numberofsamples;
	
} // END Constructor class GLCtrl

/////////////////////////////////////////////////////////////////////////////////////////
// Destructor
GLCtrl::~GLCtrl()
{
	// If glx context is still there, destroy it
	// That can happen on unclean exit
	if(WindowContext)
	{
		glXDestroyContext( (Display *)Xdisplay, WindowContext );
		WindowContext = NULL;
	}
	
} // END Destructor class GLCtrl

/////////////////////////////////////////////////////////////////////////////////////////
// Ovverridden method to choose the correct visual
XVisualInfo *GLCtrl::CreateVisual(void)
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
		SetErrorMessage("GLCtrl : Impossible to find a suitable visual.");
	}
	
	return visualInfo;

} // END GLCtrl::CreateVisual()

	
/////////////////////////////////////////////////////////////////////////////////////////
// Overridden method for attribute setting
void GLCtrl::SetAttributes(unsigned long &ValueMask, XSetWindowAttributes &winAttributes)
{
	ValueMask |=
		CWBorderPixel
		| CWEventMask
		| CWSaveUnder
	;
	winAttributes.border_pixel = 0;
	winAttributes.event_mask   = ExposureMask;
	winAttributes.save_under   = XFalse;

} // END GLCtrl::SetAttributes()


/////////////////////////////////////////////////////////////////////////////////////////
// Activates current OpenGL context
void GLCtrl::ActivateContext()
{
	if( Instances > 0 && ContextActivated != InstanceNum )
	{
		glXMakeCurrent( (Display*)Xdisplay, GetWindow(), WindowContext );
		ContextActivated = InstanceNum;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// Overridden method to create OpenGL context
void GLCtrl::AfterInit(bool Error)
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
		SetErrorMessage("GLCtrl : Error creating GLX context.");
		SetError(true);
	}
	
	// Activate the created glxwindow
	glXMakeCurrent( (Display*)Xdisplay, GetWindow(), WindowContext );
	
	// Call user init function
	GLInit();

	// Calls resize and paint functions
	GLResize( GetSize().cx, GetSize().cy );
	GLPaint();

} // END GLCtrl::AfterInit()


/////////////////////////////////////////////////////////////////////////////////////////
// Overridden method to destroy OpenGL context
void GLCtrl::BeforeTerminate(void)
{
	// Calls user terminate function
	GLDone();
	
	// Resets context and destroys it
	glXMakeCurrent( (Display*)Xdisplay, None, NULL );
	glXDestroyContext( (Display *)Xdisplay, WindowContext );
	WindowContext = NULL;

} // END GLCtrl::BeforeTerminate()


/////////////////////////////////////////////////////////////////////////////////////////
// Overridden method to resize GL windows
void GLCtrl::Resize(int x, int y)
{
	// Activates the current context
	ActivateContext();
	
	// Calls user resize hook
	GLResize(x, y);

} // END GLCtrl::Resize()


/////////////////////////////////////////////////////////////////////////////////////////
// Internal OpenGL Paint method
void GLCtrl::doPaint(void)
{
	// Activates the current context
	ActivateContext();

	// Calls user paint hook
	GLPaint();

	// Swap buffers or flush as needed
	if( DoubleBuffering )
		glXSwapBuffers( (Display*)Xdisplay, GetWindow() ); // Buffer swap does implicit glFlush
	else
		glFlush();

} // END GLCtrl::doPaint()


/////////////////////////////////////////////////////////////////////////////////////////
// Paint method - with graphic context
void GLCtrl::Paint(Draw &draw)
{
	// Calls internal OpenGL Paint method
	doPaint();
	
} // END GLCtrl::Paint()


/////////////////////////////////////////////////////////////////////////////////////////
// Forces control resize
void GLCtrl::PostResizeGL()
{
	// Gets sizes from Control
	int w = GetSize().cx,
		h = GetSize().cy;
		
	// Calls Resize method
	Resize(w, h);

}


/////////////////////////////////////////////////////////////////////////////////////////
// Forces control repaint
void GLCtrl::PostPaintGL()
{
	// Calls internal OpenGL Paint method
	doPaint();

} // END GLCtrl::PostPaintGL()


/////////////////////////////////////////////////////////////////////////////////////////
// Initializes OpenGL context to a standard view
void GLCtrl::StdView()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	Size sz = GetSize();
	glViewport(0, 0, (GLsizei)sz.cx, (GLsizei)sz.cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)(sz.cx)/(GLfloat)(sz.cy), 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

} // END GLCtrl::StdView()

#endif

END_UPP_NAMESPACE
