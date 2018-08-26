#include "DragWindow.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

LRESULT DragWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// first, send event to TopWindow handler
	LRESULT lresult = TopWindow::WindowProc(message, wParam, lParam);
	switch(message)
	{
		case WM_SIZE:
		case WM_SIZING:
			sizeMoving = false;
			dragging = false;
			break;
			
		case WM_ENTERSIZEMOVE:
			sizeMoving = 1;
			break;
			
		case WM_MOVING:
			if(sizeMoving)
			{
				LPRECT lpRect = (LPRECT)lParam;
				x = lpRect->left;
				y = lpRect->top;
				width = lpRect->right - lpRect->left;
				height = lpRect->bottom - lpRect->top;
				if(!dragging)
				{
					dragging = true;
					WindowDragged(DRAG_START, Point(x + width / 2, y));
				}
				else
					WindowDragged(DRAG_DRAG, Point(x + width / 2, y));
			}
			else
				dragging = false;
			break;
			
		case WM_EXITSIZEMOVE:
			if(dragging)
				WindowDragged(DRAG_END, Point(x + width / 2, y));
			sizeMoving = false;
			dragging = false;
			break;
	}
	return lresult;
}

#elif defined(flagGTK)

void DragWindow::connectInternalHandler(void)
{
	gtkWindow = gtk();
	handlerId = g_signal_connect(gtkWindow, "event", G_CALLBACK(DragWindowGtkEvent), (gpointer)this);
}

gboolean DragWindow::DragWindowGtkEvent(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	// static event handler -- forward to me
	GuiLock __;

	Ctrl *p = (Ctrl *)user_data;
	DragWindow *me = dynamic_cast<DragWindow *>(p);
	if(me)
		me->EventProc(event);
	
	return true;
}

void DragWindow::EventProc(GdkEvent *event)
{
	if(event->type == GDK_FOCUS_CHANGE)
	{
		GdkEventFocus *eventFocus = (GdkEventFocus *)event;

		if(eventFocus->in)
		{
			if(dragging)
			{
				WindowDragged(DRAG_END, Point(x + width / 2, y));
				dragging = false;
				x = y = width = height = -1;
			}
		}
		else
		{
			Rect r = GetScreenRect();
			x = r.left;
			y = r.top;
			width = r.Width();
			height = r.Height();
		}

	}
	else if(event->type == GDK_CONFIGURE)
	{
		GdkEventConfigure *eventConfigure = (GdkEventConfigure *)event;

		// give time to eventually resize the window rect
		Ctrl::ProcessEvents();

		// check if we're really dragging the window or just
		// resizing it; in latter case, just ignore the event
		if(!dragging)
		{
			Rect r = GetScreenRect();
			if(r.Width() == width && r.Height() == height)
			{
				x = eventConfigure->x;
				y = eventConfigure->y;
				width = eventConfigure->width;
				height = eventConfigure->height;
				
				WindowDragged(DRAG_START, Point(x + width / 2, y));
				dragging = true;
			}
		}
		if(dragging)
		{
			x = eventConfigure->x;
			y = eventConfigure->y;
			width = eventConfigure->width;
			height = eventConfigure->height;
			WindowDragged(DRAG_DRAG, Point(x + width / 2, y));
		}
	}
}

#elif defined(flagX11)

void DragWindow::EventProc(XWindow& w, XEvent *event)
{
	// first, send event to TopWindow
	TopWindow::EventProc(w, event);
	
	if(event->type == FocusOut && event->xfocus.mode == NotifyGrab)
	{
		Rect r = GetScreenRect();
		x = r.left;
		y = r.top;
		width = r.Width();
		height = r.Height();
	}
	else if(event->type == FocusIn && event->xfocus.mode == NotifyUngrab)
	{
		if(dragging)
		{
			WindowDragged(DRAG_END, Point(x + width / 2, y));
			dragging = false;
			x = y = width = height = -1;
		}
	}
	else if(event->type == ConfigureNotify)
	{
		// give time to eventually resize the window rect
		Ctrl::ProcessEvents();

		// check if we're really dragging the window or just
		// resizing it; in latter case, just ignore the event
		if(!dragging)
		{
			Rect r = GetScreenRect();
			if(r.Width() == width && r.Height() == height)
			{
				x = event->xconfigure.x;
				y = event->xconfigure.y;
				width = event->xconfigure.width;
				height = event->xconfigure.height;
				WindowDragged(DRAG_START, Point(x + width / 2, y));
				dragging = true;
			}
		}
		if(dragging)
		{
			x = event->xconfigure.x;
			y = event->xconfigure.y;
			width = event->xconfigure.width;
			height = event->xconfigure.height;
			WindowDragged(DRAG_DRAG, Point(x + width / 2, y));
		}
	}
}

#else

#error("Unknown platform not supported")

#endif

DragWindow::DragWindow()
{
	dragging = false;
#ifdef PLATFORM_WIN32
	sizeMoving = false;
#endif
	x = y = width = height = -1;

#ifdef flagGTK
	// as widget is still not ready now, we must connect the event from a callback
	PostCallback([&]() { connectInternalHandler(); });
#endif
}

DragWindow::~DragWindow()
{
#ifdef flagGTK
	if(g_signal_handler_is_connected(gtkWindow, handlerId))
		g_signal_handler_disconnect(gtkWindow, handlerId);
#endif
}

// handler for window drag events
void DragWindow::WindowDragged(DragEvent e, Point p)
{
}


END_UPP_NAMESPACE
