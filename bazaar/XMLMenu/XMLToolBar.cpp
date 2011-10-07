#include "XMLToolBar.h"
#include "XMLToolBarFrame.h"

#include "WithXMLMenu.h"

NAMESPACE_UPP

XMLToolBar::XMLToolBar(XMLMenuInterface *_iFace)
{
	// stores interface to menu handler
	iFace = _iFace;
	
	// adds the toolbar
	Add(toolBar.SizePos());
	
	// initialize current/previous state
	SetState(TOOLBAR_CLOSED);
	prevState = TOOLBAR_TOP;
	
	// undocked, at screen center
	Rect r = GetScreenRect();
	undockedX = r.left + r.Width() / 2;
	undockedY = r.top + r.Height() / 2;
	
	// docked, after last line, at left
	dockedRow = 1000;
	dockedCol = 0;
}

XMLToolBar::~XMLToolBar()
{
}

// sets toolbar state
XMLToolBar &XMLToolBar::SetState(XMLToolBarState state)
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
	if(GetIsDocked() || (GetIsPopUp() && toolBarState != TOOLBAR_SQUARE_POPUP))
	{
		dragFrame.Align(GetAlign());
		AddFrame(dragFrame);
	}
	return *this;
}

// gets toolbar align
int XMLToolBar::GetAlign(void)
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
			return ToolBar::BAR_TOP;
			
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
bool XMLToolBar::GetIsDocked()
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
bool XMLToolBar::GetIsFloating()
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
bool XMLToolBar::GetIsOpened()
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
bool XMLToolBar::GetIsPopUp()
{
	return IsPopUp();
}
		
// lays toolbar and handle inside control
void XMLToolBar::Layout(void)
{
	ParentCtrl::Layout();
}

// gets toolbar size when completely inlined horizontally
Size XMLToolBar::GetHorzSize(void)
{
	SizeToolBar *t = (SizeToolBar *)&toolBar;
	Size sz = t->GetPaneSize(true);
	dragFrame.Align(ToolBar::BAR_TOP);
	dragFrame.FrameAddSize(sz);
	dragFrame.Align(GetAlign());
	return sz;
}

// gets toolbar size when completely inlined vertically
Size XMLToolBar::GetVertSize(void)
{
	SizeToolBar *t = (SizeToolBar *)&toolBar;
	Size sz = t->GetPaneSize(false);
	dragFrame.Align(ToolBar::BAR_LEFT);
	dragFrame.FrameAddSize(sz);
	dragFrame.Align(GetAlign());
	return sz;
}

// gets toolbar size when squared
Size XMLToolBar::GetSquaredSize(void)
{
	SizeToolBar *t = (SizeToolBar *)&toolBar;

	Size hSz = t->GetPaneSize(true);
	Size vSz = t->GetPaneSize(false);
	double area = max(hSz.cx * hSz.cy, vSz.cx * vSz.cy);
	int hor = (int)sqrt(area);
	if(hor < vSz.cx)
		hor = vSz.cx;
	return t->GetPaneSize(true, hor);
}

// floats the toolbar at a given position
XMLToolBarFloating &XMLToolBar::Float(Point p)
{
	// close bar
	CloseBar();

	// sets current state
	SetState(TOOLBAR_FLOATING);
	
	// create the floating window
	floating = new XMLToolBarFloating(*this, p);
	
	return *floating;
}

// popups the toolbar at a given position
XMLToolBar &XMLToolBar::Popup(Point p)
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
	
	PopUp();

	return *this;
}

XMLToolBar &XMLToolBar::PopHorz(Point p)
{
	if(!GetIsPopUp())
		Popup(p);
	SetState(TOOLBAR_HORZ_POPUP);
	Layout();
	Size sz = GetHorzSize();
	SetRect(p.x, p.y - sz.cy / 2, sz.cx, sz.cy);
	return *this;
}

XMLToolBar &XMLToolBar::PopVert(Point p)
{
	if(!GetIsPopUp())
		Popup(p);
	SetState(TOOLBAR_VERT_POPUP);
	Layout();
	Size sz = GetVertSize();
	SetRect(p.x - sz.cx / 2, p.y, sz.cx, sz.cy);
	return *this;
}

XMLToolBar &XMLToolBar::PopSquare(Point p)
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
XMLToolBar &XMLToolBar::Dock(XMLToolBarFrame &f, int row, int col)
{
	// close bar
	CloseBar();

	// dock into given frame and set docked state
	f.Dock(*this, row, col);
	toolBarFrame = &f;
	SetState(f.GetToolBarState());
	prevState = toolBarState;
	return *this;
}

XMLToolBar &XMLToolBar::Dock(XMLToolBarFrame &f, Point p)
{
	// close bar
	CloseBar();

	// dock into given frame and set docked state
	f.Dock(*this, p);
	toolBarFrame = &f;
	SetState(f.GetToolBarState());
	prevState = toolBarState;
	return *this;
}

// closes the toolbar
XMLToolBar &XMLToolBar::CloseBar(void)
{
	// saves last state before closing an oriented toolbar
	if(toolBarState != TOOLBAR_CLOSED && toolBarState != TOOLBAR_FLOATING)
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
	{
		floating->Close();
	}
	SetState(TOOLBAR_CLOSED);
	return *this;
}

// pre-Dock and pre-Undock a toolbar inside frame
XMLToolBar &XMLToolBar::PreDock(XMLToolBarFrame &f, Point p)
{
	ASSERT(toolBarState == XMLToolBar::TOOLBAR_HORZ_POPUP || toolBarState == XMLToolBar::TOOLBAR_VERT_POPUP);
	
	// dock into given frame and set docked state
	f.PreDock(*this, p);
	toolBarFrame = &f;
	switch(f.GetToolBarState())
	{
		case XMLToolBar::TOOLBAR_LEFT :
		case XMLToolBar::TOOLBAR_RIGHT :
			SetState(XMLToolBar::TOOLBAR_VERT_POPUP);
			break;
			
		case XMLToolBar::TOOLBAR_TOP :
		case XMLToolBar::TOOLBAR_BOTTOM :
			SetState(XMLToolBar::TOOLBAR_HORZ_POPUP);
			break;
		default:
			NEVER();
			break;
	}
	Layout();
	return *this;
}

XMLToolBar &XMLToolBar::UnPreDock(XMLToolBarFrame &f)
{
	// dock into given frame and set docked state
	f.UnPreDock(*this);
	toolBarFrame = NULL;
	return *this;
}

// mouse handler -- handles moves when ctrl is in popup state
Image XMLToolBar::MouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	// handles events JUST when ctl is in popup state
	if(!IsPopUp())
		return Image::Arrow();

	// query the controller interface
	return iFace->PopupMouseEvent(event, p, zdelta, keyflags);
}

END_UPP_NAMESPACE
