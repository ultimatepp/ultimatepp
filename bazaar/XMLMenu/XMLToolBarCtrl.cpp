#include "XMLToolBarCtrl.h"
#include "XMLToolBarFrame.h"

#include "WithXMLMenu.h"

NAMESPACE_UPP

XMLToolBarCtrl::XMLToolBarCtrl(XMLMenuInterface *_iFace)
{
	// stores interface to menu handler
	iFace = _iFace;

	// adds the toolbar
	Add(toolBar.SizePos());
	
	// initialize current/previous state
	SetState(TOOLBAR_CLOSED);
	prevState = TOOLBAR_TOP;
	
	// initialize position
	toolBarPos = Point(0, 100);
}

XMLToolBarCtrl::~XMLToolBarCtrl()
{
}

// sets toolbar state
XMLToolBarCtrl &XMLToolBarCtrl::SetState(XMLToolBarState state)
{
	// sets orientation
	toolBarState = state;
	switch(state)
	{
		case TOOLBAR_CLOSED :
			toolBar.Align(ToolBar::BAR_TOP);
			toolBar.Wrap();
			break;
			
		case TOOLBAR_FLOATING :
			toolBar.Align(ToolBar::BAR_TOP);
			toolBar.Wrap();
			break;
			
		case TOOLBAR_HORZ_POPUP :
			toolBar.Align(ToolBar::BAR_TOP);
			toolBar.NoWrap();
			break;
			
		case TOOLBAR_VERT_POPUP :
			toolBar.Align(ToolBar::BAR_LEFT);
			toolBar.NoWrap();
			break;
			
		case TOOLBAR_SQUARE_POPUP :
			toolBar.Align(ToolBar::BAR_TOP);
			toolBar.Wrap();
			break;
			
		case TOOLBAR_TOP :
			toolBar.Align(ToolBar::BAR_TOP);
			toolBar.NoWrap();
			break;
			
		case TOOLBAR_BOTTOM :
			toolBar.Align(ToolBar::BAR_TOP);
			toolBar.NoWrap();
			break;
			
		case TOOLBAR_LEFT :
			toolBar.Align(ToolBar::BAR_LEFT);
			toolBar.NoWrap();
			break;
			
		case TOOLBAR_RIGHT :
			toolBar.Align(ToolBar::BAR_LEFT);
			toolBar.NoWrap();
			break;

		default:
			NEVER();
	}
	// remove all frames
	ClearFrames();
	
	// if docked or popup, add the dragging frame
//	if(GetIsDocked() || (GetIsPopUp()  && toolBarState != TOOLBAR_SQUARE_POPUP))
	if(!GetIsFloating())
	{
		dragFrame.Align(GetAlign());
		AddFrame(dragFrame);
	}
	return *this;
}

// gets bar position
Point XMLToolBarCtrl::GetPosition(void) const
{
	return toolBarPos;
}


// gets toolbar align
int XMLToolBarCtrl::GetAlign(void) const
{
	switch(toolBarState)
	{
		case TOOLBAR_CLOSED :
			return ToolBar::BAR_TOP;
			
		case TOOLBAR_FLOATING :
			return ToolBar::BAR_TOP;
			
		case TOOLBAR_HORZ_POPUP :
			return ToolBar::BAR_TOP;
			
		case TOOLBAR_VERT_POPUP :
			return ToolBar::BAR_LEFT;
			
		case TOOLBAR_SQUARE_POPUP :
			return ToolBar::BAR_LEFT;
			
		case TOOLBAR_TOP :
			return ToolBar::BAR_TOP;
			
		case TOOLBAR_BOTTOM :
			return ToolBar::BAR_TOP;
			
		case TOOLBAR_LEFT :
			return ToolBar::BAR_LEFT;
			
		case TOOLBAR_RIGHT :
			return ToolBar::BAR_LEFT;

		default:
			NEVER();
			return ToolBar::BAR_TOP;
	}
}

// get docked state
bool XMLToolBarCtrl::GetIsDocked() const
{
	switch(toolBarState)
	{
		case TOOLBAR_CLOSED :
		case TOOLBAR_FLOATING :
		case TOOLBAR_HORZ_POPUP :
		case TOOLBAR_VERT_POPUP :
		case TOOLBAR_SQUARE_POPUP :
			return false;

		case TOOLBAR_TOP :
		case TOOLBAR_BOTTOM :
		case TOOLBAR_LEFT :
		case TOOLBAR_RIGHT :
			return true;
			
		default:
			NEVER();
			return false;
	}
}

// get floating state
bool XMLToolBarCtrl::GetIsFloating() const
{
	switch(toolBarState)
	{
		case TOOLBAR_FLOATING :
			return true;

		case TOOLBAR_CLOSED :
		case TOOLBAR_HORZ_POPUP :
		case TOOLBAR_VERT_POPUP :
		case TOOLBAR_SQUARE_POPUP :
		case TOOLBAR_TOP :
		case TOOLBAR_BOTTOM :
		case TOOLBAR_LEFT :
		case TOOLBAR_RIGHT :
			return false;
			
		default:
			NEVER();
			return false;
	}
}

// get floating state
bool XMLToolBarCtrl::GetIsOpened() const
{
	switch(toolBarState)
	{
		case TOOLBAR_FLOATING :
		case TOOLBAR_HORZ_POPUP :
		case TOOLBAR_VERT_POPUP :
		case TOOLBAR_SQUARE_POPUP :
		case TOOLBAR_TOP :
		case TOOLBAR_BOTTOM :
		case TOOLBAR_LEFT :
		case TOOLBAR_RIGHT :
			return true;

		case TOOLBAR_CLOSED :
			return false;
			
		default:
			NEVER();
			return false;
	}
}

// get popup state
bool XMLToolBarCtrl::GetIsPopUp() const
{
	return IsPopUp();
}
		
// lays toolbar and handle inside control
void XMLToolBarCtrl::Layout(void)
{
	ParentCtrl::Layout();
}

// gets toolbar size when completely inlined horizontally
Size XMLToolBarCtrl::GetHorzSize(void)
{
	SizeToolBar *t = (SizeToolBar *)&toolBar;
	Size sz = t->GetPaneSize(true);
	dragFrame.Align(ToolBar::BAR_TOP);
	dragFrame.FrameAddSize(sz);
	dragFrame.Align(GetAlign());
	return sz;
}

// gets toolbar size when completely inlined vertically
Size XMLToolBarCtrl::GetVertSize(void)
{
	SizeToolBar *t = (SizeToolBar *)&toolBar;
	Size sz = t->GetPaneSize(false);
	dragFrame.Align(ToolBar::BAR_LEFT);
	dragFrame.FrameAddSize(sz);
	dragFrame.Align(GetAlign());
	return sz;
}

// gets toolbar size when squared
Size XMLToolBarCtrl::GetSquaredSize(void)
{
	SizeToolBar *t = (SizeToolBar *)&toolBar;

	Size hSz = t->GetPaneSize(true);
	Size vSz = t->GetPaneSize(false);
	double area = max(hSz.cx * hSz.cy, vSz.cx * vSz.cy);
	int hor = (int)sqrt(area);
	if(hor < vSz.cx)
		hor = vSz.cx;
	Size sz = t->GetPaneSize(true, hor);
	dragFrame.Align(ToolBar::BAR_LEFT);
	dragFrame.FrameAddSize(sz);
	dragFrame.Align(GetAlign());
	return sz;
}

// floats the toolbar at a given position
XMLToolBarFloating &XMLToolBarCtrl::Float(Point p)
{
	// close bar
	CloseBar();

	// sets current state
	SetState(TOOLBAR_FLOATING);
	
	// create the floating window
	floating = new XMLToolBarFloating(*this, p);
	toolBarPos = p;
	
	return *floating;
}

// popups the toolbar at a given position
XMLToolBarCtrl &XMLToolBarCtrl::Popup(Point p)
{
	// if not already popup, close and pop it up
	CloseBar();

	// sets current state
	Size sz;
	switch(prevState)
	{
		case TOOLBAR_TOP :
		case TOOLBAR_BOTTOM :
		case TOOLBAR_HORZ_POPUP :
			SetState(TOOLBAR_HORZ_POPUP);
			sz = GetHorzSize();
			break;

		case TOOLBAR_LEFT :
		case TOOLBAR_RIGHT :
		case TOOLBAR_VERT_POPUP :
			SetState(TOOLBAR_VERT_POPUP);
			sz = GetVertSize();
			break;

		default:
			SetState(TOOLBAR_SQUARE_POPUP);
			sz = GetSquaredSize();
			p.x -= sz.cx / 2;
			break;
	}
	
	// re-layout it
	Layout();

	// move it at requested point
	SetRect(p.x, p.y, sz.cx, sz.cy);

	// last try.... try to find a suitable owner for popup
	// this one was quite hard
	Ctrl *owner = dynamic_cast<Ctrl *>(iFace);
	Ctrl *topCtrl = owner->GetTopCtrl();
	while(owner && owner->GetParent() && owner->GetParent() != topCtrl)
		owner = owner->GetParent();
	PopUp(owner);

	return *this;
}

XMLToolBarCtrl &XMLToolBarCtrl::PopHorz(Point p)
{
	if(!GetIsPopUp())
		Popup(p);
	SetState(TOOLBAR_HORZ_POPUP);
	Layout();
	Size sz = GetHorzSize();
	SetRect(p.x, p.y - sz.cy / 2, sz.cx, sz.cy);
	return *this;
}

XMLToolBarCtrl &XMLToolBarCtrl::PopVert(Point p)
{
	if(!GetIsPopUp())
		Popup(p);
	SetState(TOOLBAR_VERT_POPUP);
	Layout();
	Size sz = GetVertSize();
	SetRect(p.x - sz.cx / 2, p.y, sz.cx, sz.cy);
	return *this;
}

XMLToolBarCtrl &XMLToolBarCtrl::PopSquare(Point p)
{
	if(!GetIsPopUp())
		Popup(p);
	SetState(TOOLBAR_SQUARE_POPUP);
	Layout();
	Size sz = GetSquaredSize();
	SetRect(p.x - sz.cx / 2, p.y, sz.cx, sz.cy);
	return *this;
}

// docks the toolbar at a position inside a frame
XMLToolBarCtrl &XMLToolBarCtrl::Dock(XMLToolBarFrame &f, Point p)
{
	// close bar
	CloseBar();

	// dock into given frame and set docked state
	f.Dock(*this, p);
	toolBarFrame = &f;
	SetState(f.GetToolBarState());
	return *this;
}

XMLToolBarCtrl &XMLToolBarCtrl::DockAt(XMLToolBarFrame &f, Point p)
{
	// close bar
	CloseBar();

	// dock into given frame and set docked state
	f.DockAt(*this, p);
	toolBarFrame = &f;
	SetState(f.GetToolBarState());
	return *this;
}

// closes the toolbar
XMLToolBarCtrl &XMLToolBarCtrl::CloseBar(void)
{
	// saves last state before closing a toolbar
	// this is used to toggle it on demand
	if(toolBarState != TOOLBAR_CLOSED && !IsPopUp())
		prevState = toolBarState;
	
	// close if displayed as popup
	if(IsPopUp())
		Close();
	
	// undock if docked somewhere
	if(GetIsDocked() && toolBarFrame)
	{
		toolBarFrame->Undock(*this);
		toolBarFrame = NULL;
	}
	// close floating container, if any
	else if(GetIsFloating() && floating)
		floating.Clear();

	SetState(TOOLBAR_CLOSED);
	return *this;
}

// pre-Dock and pre-Undock a toolbar inside frame
XMLToolBarCtrl &XMLToolBarCtrl::PreDock(XMLToolBarFrame &f, Point p)
{
	ASSERT(toolBarState == TOOLBAR_HORZ_POPUP || toolBarState == TOOLBAR_VERT_POPUP);
	
	// dock into given frame and set docked state
	f.PreDock(*this, p);
	toolBarFrame = &f;
	switch(f.GetToolBarState())
	{
		case TOOLBAR_LEFT :
		case TOOLBAR_RIGHT :
			SetState(TOOLBAR_VERT_POPUP);
			break;
			
		case TOOLBAR_TOP :
		case TOOLBAR_BOTTOM :
			SetState(TOOLBAR_HORZ_POPUP);
			break;
		default:
			NEVER();
			break;
	}
	Layout();
	return *this;
}

XMLToolBarCtrl &XMLToolBarCtrl::UnPreDock(XMLToolBarFrame &f)
{
	// dock into given frame and set docked state
	f.UnPreDock(*this);
	toolBarFrame = NULL;
	return *this;
}

// right mouse click handler -- forwards to main class context menu
void XMLToolBarCtrl::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	// we need to use PostCallback here, otherwise if sending ctrl gets destroyed on the way
	// Upp try to dispatch the mouse event to it, crashing
	if(event == RIGHTDOWN)
		Ctrl::PostCallback(callback1(iFace, &XMLMenuInterface::RightClickEvent, p));
}

END_UPP_NAMESPACE
