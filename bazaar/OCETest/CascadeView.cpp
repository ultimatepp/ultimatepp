
#include "CascadeView.h"


using namespace Upp;

/////////////////////////////////////////////////////////////////////////////////////////
// Constructor
CascadeView::CascadeView() : OCECtrl()
{
	// Resets panning and rotate flag
	isPanning = false;
	isRotating = false;
	
	// Resets Pan start point to a default value
	panStartPoint.SetNull();
	
} // END Constructor class CascadeView

/////////////////////////////////////////////////////////////////////////////////////////
// Destructor
CascadeView::~CascadeView()
{
} // END Destructor class CascadeView


///////////////////////////////////////////////////////////////////////////////////////////////
// middle up - resets pan/3dorbit behaviour
void CascadeView::MiddleUp(Point p, dword keyflags)
{
	isPanning = false;
	panStartPoint.SetNull();
	
	isRotating = false;
	
} // END CascadeView::MiddleUp()


///////////////////////////////////////////////////////////////////////////////////////////////
// mouse move handler
void CascadeView::MouseMove(Point p, dword keyflags)
{
	/////////////////// DISPLAY HANDLING (PAN / ROTATE VIEW  /////////////////////
	if(isPanning)
	{
		GetViewHandle()->Pan(p.x - panPrevPoint.x, panPrevPoint.y - p.y);
		panPrevPoint = p;
	}
	
	if(isRotating)
		GetViewHandle()->Rotation(p.x, p.y);
	
	///////////////////   /////////////////////

} // END CascadeView::MiddleDown()

		
///////////////////////////////////////////////////////////////////////////////////////////////
// middle drag - pan or 3dorbit if in addition to ctrl-shift keys
void CascadeView::MiddleDrag(Point p, dword keyflags)
{
	panStartPoint = p;
	panPrevPoint = p;
	
	// if both CTRL and SHIFT keys are pressed, the view is rotated (3dorbit)
	// if not, the pan function is activated
	if( (keyflags & K_SHIFT) && (keyflags & K_CTRL) )
	{
		isRotating = true;
		GetViewHandle()->StartRotation(p.x, p.y, 0.4);
	}
	else
		isPanning = true;
	
} // END CascadeView::MiddleDrag()
		

///////////////////////////////////////////////////////////////////////////////////////////////
// mouse wheel - zoom
void CascadeView::MouseWheel(Point p, int zdelta, dword keyflags)
{
	Quantity_Factor	CurScale = GetViewHandle()->Scale();
	if(zdelta > 0)
		CurScale *= (1.0 + zdelta / 200.0);
	else
		CurScale /= (1.0 - zdelta / 200.0);
	GetViewHandle()->SetScale(CurScale);
	
	GetViewHandle()->Redraw();
	
} // END CascadeView::MouseWheel()
		

///////////////////////////////////////////////////////////////////////////////////////////////
// double middle click - zoom extents
void CascadeView::MiddleDouble(Point p, dword keyflags)
{
  GetViewHandle()->FitAll();	
  GetViewHandle()->Redraw();
	
} // END CascadeView::MiddleDouble()


///////////////////////////////////////////////////////////////////////////////////////////////
// mouse enter handler
void   CascadeView::MouseEnter(Point p, dword keyflags)
{
	// if mouse middle button is pressed on enter, resume panning or rotate
	if(GetMouseMiddle())
	{
		// sets starting and current pan points if needed
		if(panStartPoint.IsNullInstance())
		{
			panStartPoint = p;
			panPrevPoint = p;
		}

		// pans or rotates depending on CTRL-SHIFT keys
		if( (keyflags & K_SHIFT) && (keyflags & K_CTRL) )
		{
			isRotating = true;
			GetViewHandle()->StartRotation(p.x, p.y, 0.4);
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
