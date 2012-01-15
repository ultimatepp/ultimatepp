
#include "CascadeView.h"

#ifdef WIN32
#include <WNT_Window.hxx>
#include <Graphic3d_WNTGraphicDevice.hxx>
#else
#include <Xw_Window.hxx>
#include <Graphic3d_GraphicDevice.hxx>
#endif

using namespace Upp;

/////////////////////////////////////////////////////////////////////////////////////////
// Constructor
CascadeView::CascadeView(CascadeDocument *Doc)
{
	// Stores the document
	Document = Doc;
	
	// Resets view pointer
	View.Nullify();
	
	// Resets panning and rotate flag
	isPanning = false;
	isRotating = false;
	
	// Resets Pan start point to a default value
	PanStartPoint.SetNull();
	
} // END Constructor class CascadeView

/////////////////////////////////////////////////////////////////////////////////////////
// Destructor
CascadeView::~CascadeView()
{
	// Resets document handle
	Document = 0;
	
	// Resets view handle
	View.Nullify();
	
} // END Destructor class CascadeView

#ifdef PLATFORM_X11

/////////////////////////////////////////////////////////////////////////////////////////
// Method to choose the correct visual
XVisualInfo *CascadeView::CreateVisual(void)
{
	int visualAttr[] =
	{
		GLX_RGBA,
		GLX_DEPTH_SIZE,		1,
		GLX_RED_SIZE,		1,
		GLX_GREEN_SIZE,		1,
		GLX_BLUE_SIZE,		1,
		GLX_DOUBLEBUFFER,	None
	};
    XVisualInfo *pVisualInfo = ::glXChooseVisual( Xdisplay, DefaultScreen(Xdisplay), visualAttr );
    
    return pVisualInfo;
	
} // END CascadeView::CreateVisual()
		
/////////////////////////////////////////////////////////////////////////////////////////
// Method for attribute setting
void CascadeView::SetAttributes(unsigned long &ValueMask, XSetWindowAttributes &attr)
{
	ValueMask |=
			CWBackPixel
		|	CWBorderPixel
	;
	attr.background_pixel = 0;
	attr.border_pixel = 0;
	
} // END CascadeView::SetAttributes()
		

/////////////////////////////////////////////////////////////////////////////////////////
// GLInit method
void CascadeView::AfterInit(bool isError)
{
	if(isError)
		return;
	
	// Gets the window handle
	Window WindowHandle = GetWindow();

	// Creates the view object
	View = Document->GetViewer()->CreateView();
	
	// Creates the OpenCascade window handle
    short lo = (short) WindowHandle;
    short hi = (short) (WindowHandle >> 16);
    Handle(Xw_Window) hWnd = new Xw_Window(Handle(Graphic3d_GraphicDevice)::DownCast(Document->GetViewer()->Device()),(int) hi,(int) lo,Xw_WQ_SAMEQUALITY);
//    Handle(Xw_Window) hWnd = new Xw_Window(Document->getGraphicDevice(), WindowHandle);

	// Sets window handle in view
    View->SetWindow(hWnd);
    
    // Maps the view if needed
    if ( !hWnd->IsMapped() )
        hWnd->Map();
    
	// platform intependent part
	InitView();
    
} // END CascadeView::AfterInit()


/////////////////////////////////////////////////////////////////////////////////////////
// These is called just before termination
void CascadeView::BeforeTerminate(void)
{
	if(!View.IsNull())
		View.Nullify();

} // END CascadeView::BeforeTerminate()
		
#else
void CascadeView::State(int reason)
{
	if (reason == CLOSE)
	{
		if(!View.IsNull())
			View.Nullify();
	}

	DHCtrl::State(reason);
	
	if (reason == OPEN)
	{
		// platform dependent part

		// Gets the window handle
		HWND windowHandle = GetHWND();
	
		// Creates the view object
		View = Document->GetViewer()->CreateView();
		
		// Creates the OpenCascade window handle
		Handle(WNT_Window) hWnd = new WNT_Window(Document->GetGraphicDevice(), windowHandle);
	
		// Sets window handle in view
	    View->SetWindow(hWnd);
	    
	    // Maps the view if needed
	    if ( !hWnd->IsMapped() )
	        hWnd->Map();

		// platform intependent part
		InitView();
	};
	
	
}

#endif


/////////////////////////////////////////////////////////////////////////////////////////
// initializes view after platform-dependent init
void CascadeView::InitView(void)
{
    // Sets the background color
    View->SetBackgroundColor(Quantity_NOC_BLACK);
    
    // Sets up the triedron
    View->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.1, V3d_ZBUFFER);
    View->TriedronEcho(Aspect_TOTE_ORIGIN); // ???

    // Activate the grid
    Document->GetViewer()->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);
    Document->GetViewer()->SetGridEcho(Standard_True);
    
    View->SetTransparency();

	View->SetPlaneOff();
    
    // Signals resize needed
    View->MustBeResized();
    
} // END CascadeView::InitView()

/////////////////////////////////////////////////////////////////////////////////////////
// Paint method
void CascadeView::Paint(Draw &draw)
{
	if(View.IsNull())
		return;
    View->MustBeResized();
	View->Redraw();
	
} // END CascadeView::Paint()


/////////////////////////////////////////////////////////////////////////////////////////
// Handle layout events and propagates to embedded window
void CascadeView::Layout(void)
{
	if(View.IsNull())
		return;
    View->MustBeResized();
    
} // END CascadeView::Resize()


/////////////////////////////////////////////////////////////////////////////////////////
// Fit all in view
void CascadeView::FitAll()
{
	if(View.IsNull())
		return;
    View->FitAll();
    View->ZFitAll();
    View->Redraw();

} // END CascadeView::FitAll()


///////////////////////////////////////////////////////////////////////////////////////////////
// middle up - resets pan/3dorbit behaviour
void CascadeView::MiddleUp(Point p, dword keyflags)
{
	isPanning = false;
	PanStartPoint.SetNull();
	
	isRotating = false;
	
} // END CascadeView::MiddleUp()


///////////////////////////////////////////////////////////////////////////////////////////////
// mouse move handler
void CascadeView::MouseMove(Point p, dword keyflags)
{
	/////////////////// DISPLAY HANDLING (PAN / ROTATE VIEW  /////////////////////
	if(isPanning)
	{
		View->Pan(p.x - PanPrevPoint.x, PanPrevPoint.y - p.y);
		PanPrevPoint = p;
	}
	
	if(isRotating)
		View->Rotation(p.x, p.y);
	
	///////////////////   /////////////////////

} // END CascadeView::MiddleDown()

		
///////////////////////////////////////////////////////////////////////////////////////////////
// middle drag - pan or 3dorbit if in addition to ctrl-shift keys
void CascadeView::MiddleDrag(Point p, dword keyflags)
{
	PanStartPoint = p;
	PanPrevPoint = p;
	
	// if both CTRL and SHIFT keys are pressed, the view is rotated (3dorbit)
	// if not, the pan function is activated
	if( (keyflags & K_SHIFT) && (keyflags & K_CTRL) )
	{
		isRotating = true;
		View->StartRotation(p.x, p.y, 0.4);
	}
	else
		isPanning = true;
	
} // END CascadeView::MiddleDrag()
		

///////////////////////////////////////////////////////////////////////////////////////////////
// mouse wheel - zoom
void CascadeView::MouseWheel(Point p, int zdelta, dword keyflags)
{
	Quantity_Factor	CurScale = View->Scale();
	if(zdelta > 0)
		CurScale *= (1.0 + zdelta / 200.0);
	else
		CurScale /= (1.0 - zdelta / 200.0);
	View->SetScale(CurScale);
	
	View->Redraw();
	
} // END CascadeView::MouseWheel()
		

///////////////////////////////////////////////////////////////////////////////////////////////
// double middle click - zoom extents
void CascadeView::MiddleDouble(Point p, dword keyflags)
{
  View->FitAll();	
  View->Redraw();
	
} // END CascadeView::MiddleDouble()


///////////////////////////////////////////////////////////////////////////////////////////////
// mouse enter handler
void   CascadeView::MouseEnter(Point p, dword keyflags)
{
	// if mouse middle button is pressed on enter, resume panning or rotate
	if(GetMouseMiddle())
	{
		// sets starting and current pan points if needed
		if(PanStartPoint.IsNullInstance())
		{
			PanStartPoint = p;
			PanPrevPoint = p;
		}

		// pans or rotates depending on CTRL-SHIFT keys
		if( (keyflags & K_SHIFT) && (keyflags & K_CTRL) )
		{
			isRotating = true;
			View->StartRotation(p.x, p.y, 0.4);
		}
		else
			isPanning = true;
	}
	
} // END CascadeView::MouseEnter()


///////////////////////////////////////////////////////////////////////////////////////////////
// mouse leave handler
void   CascadeView::MouseLeave()
{
	// resets panning & rotate state on window exit
	isPanning = false;
	isRotating = false;
	
} // END CascadeView::MouseLeave()
