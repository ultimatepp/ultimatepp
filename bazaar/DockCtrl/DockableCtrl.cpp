#include "DockCtrl/DockCtrl.h"

#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#define IMAGECLASS DockableCtrlImages
#include <Draw/iml.h>

//===============================================
// DockableCtrl class:
// Base class of all DockCtrl widgets.
//===============================================

DockableCtrl::DockableCtrl()
{
	DockingStyle(DOCK_NONE, STATE_SHUT, 0);
	PermitDragAndDrop();
	SetDropTarget();	
	docksize.Clear();
	hasdragbar	= false;
	haschildbar	= false;
	isdragged	= false;
	isdraggable = true;
	dockbase 	= NULL;
	style		= NULL;
	WhenClose	= THISBACK(OnShutWindow);
	NoCenter().BackPaint();
}

DockableCtrl::~DockableCtrl()
{
}

DockableCtrl& DockableCtrl::DockingStyle(int alignment, int state, int position)
{
	dockalignment 	= alignment;
	dockstate		= state;
	dockposition	= position;
	return *this;
}

DockableCtrl& DockableCtrl::SetLabel(String title)
{
	IsNull(title) || title == t_("") ? dockname = t_("Untitled") :  dockname = title;
	Title(title);
	if(IsVisible()) Refresh();
	return *this;
}

DockableCtrl& DockableCtrl::SetIcon(Image icon)
{
	dockicon = icon;
	Icon(dockicon);
	if(IsVisible()) Refresh();
	return *this;
}

DockableCtrl& DockableCtrl::SetSize(Size sz)
{
	SetSizeHint(sz);
	Rect r = GetRect();
	SetRect(r.left, r.top, r.left + sz.cx, r.top + sz.cy);
	if(IsFloating()) Refresh();
	else RefreshParentLayout();
	return *this;
}

void DockableCtrl::ReSize()
{
	docksize = Ctrl::GetSize();
	for(int i = 0; i < GetFrameCount(); i++) Ctrl::GetFrame(i).FrameAddSize(docksize);	
}

void DockableCtrl::Layout()
{
	TopWindow::Layout(); 
	if(IsFloating() && IsOpen()) ReSize();
}

void DockableCtrl::OpenWindow(Rect& r)
{
	if(IsPopUp()) Close();
	if(IsOpen()) return;
	ReSize();
	r.right  = r.left + docksize.cx;
	r.bottom = r.top  + docksize.cy;
	HideDragBar();
	Title(GetLabel());
	SetRect(r);
	Ctrl::Hide();
	Open(GetOwner());
	Ctrl::Show();
	Refresh();
}

void DockableCtrl::OpenWindow(int x, int y, int cx, int cy)
{
	if(IsPopUp()) Close();
	if(IsOpen()) return;
	HideDragBar();
	Title(GetLabel());
	SetRect(x, y, cx, cy);
	Ctrl::Hide();
	Open(GetOwner());
	Ctrl::Show();
	Refresh();	
}

void DockableCtrl::ShutWindow()
{
	if(IsPopUp()) Close();
	ReSize();
	if(!IsOpen() || !IsFloating()) return;
	Rect r 	 = GetRect();
	r.right  = r.left + docksize.cx;
	r.bottom = r.top  + docksize.cy;
	Close();
	Remove();
	SetRect(r);  
	ShowDragBar();
}

void DockableCtrl::OnShutWindow()
{
	Shut();
}

void DockableCtrl::ContextMenu()
{
	MenuBar::Execute(WhenContextMenu); 
}

void DockableCtrl::Settings()
{
	GetBase().ControlPanel();
}

void DockableCtrl::Menu()
{
	ContextMenu();
}

void DockableCtrl::StartWindowDrag()
{
	if(!isdraggable) return;
	Point pt = GetMousePos();
	Float();

// Thanks to mrjt (James Thomas) for the X11 window drag code.	
#if	defined(PLATFORM_WIN32)
	SendMessage(GetHWND(), WM_NCLBUTTONDOWN, 2, MAKELONG(pt.x, pt.y));
#elif defined(PLATFORM_X11)
	Atom xwndDrag = XAtom("_NET_WM_MOVERESIZE");
	XEvent e;
	Zero(e);
	e.xclient.type = ClientMessage;
	e.xclient.message_type = xwndDrag;
	e.xclient.window = GetWindow();
	e.xclient.format = 32;
	e.xclient.display = Xdisplay;
	e.xclient.send_event = XTrue;
	e.xclient.data.l[0] = pt.x;
	e.xclient.data.l[1] = pt.y;
	e.xclient.data.l[2] = 8;
	e.xclient.data.l[3] = 1;
	e.xclient.data.l[4] = 0;	
	
	XUngrabPointer(Xdisplay, CurrentTime);
	XSendEvent(Xdisplay, RootWindow(Xdisplay, Xscreenno), XFalse, SubstructureNotifyMask, &e);
	XFlush(Xdisplay);
#endif
}


#if defined(PLATFORM_WIN32)
LRESULT  DockableCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(IsFloating())
	{
		switch(message)
		{
			case WM_MOVING:
					if(!isdraggable) break;
					isdragged = true;
					SetDropTarget(DOCK_NONE, GetDropState());
					GetBase().DoDragAndDrop(*this, GetMousePos(), GetCtrlSize());
					break;
		
			case WM_EXITSIZEMOVE:
					if(!isdraggable) break;
					if(!GetMouseLeft() && HasDropTarget() && isdragged) 
					{
						Dock(GetDropTarget(), GetDropState(), Position());
						Refresh();
						SetDropTarget();
						isdragged = false;
					}
				break;
			
			case WM_NCRBUTTONDOWN:
					ContextMenu();
				return 1L;
				
			default:
				break;
		}
	}
	return TopWindow::WindowProc(message, wParam, lParam);
}
#elif defined(PLATFORM_X11)
void DockableCtrl::EventProc(XWindow& w, XEvent *event)
{
	if (IsOpen()) 
	{
		switch(event->type) {
		case ConfigureNotify:
		{
			if(!isdraggable) break;
			XConfigureEvent& e = event->xconfigure;
			if (Point(e.x, e.y) != GetScreenRect().TopLeft()) 
			{
				SetDropTarget(DOCK_NONE, GetDropState());
				if(!isdragged);
				{	
					isdragged = true;
					GetBase().DoDragAndDrop(*this, GetMousePos(), GetCtrlSize());
					SetFocus();
				}
				break;
			}
			break;
		}
		case FocusIn:
			if(!isdraggable) break;
			XFocusChangeEvent &e = event->xfocus;
			if (e.mode == NotifyUngrab) 
			{
				Dock(GetDropTarget(), GetDropState(), Position());
				SetDropTarget();
				isdragged = false;
				return;
			}
			break;
		}
	}
	TopWindow::EventProc(w, event);	
}
#endif
