#include "DragWindow.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

LRESULT DragWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LPRECT lpRect = (LPRECT)lParam;
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
	return TopWindow::WindowProc(message, wParam, lParam);
}

#else

void DragWindow::EventProc(XWindow& w, XEvent *event)
{
	if(event->type == FocusOut && event->xfocus.mode == NotifyGrab)
	{
		Rect r = GetScreenRect();
		x = r.left;
		y = r.top;
		width = r.Width();
		height = r.Height();
		TopWindow::EventProc(w, event);
	}
	else if(event->type == FocusIn && event->xfocus.mode == NotifyUngrab)
	{
		if(dragging)
		{
			WindowDragged(DRAG_END, Point(x + width / 2, y));
			dragging = false;
			x = y = width = height = -1;
		}
		TopWindow::EventProc(w, event);
	}
	else if(event->type == ConfigureNotify)
	{
		// give time to eventually resize the window rect
		TopWindow::EventProc(w, event);
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
	else
		TopWindow::EventProc(w, event);
}

#endif

DragWindow::DragWindow()
{
	dragging = false;
#ifdef PLATFORM_WIN32
	sizeMoving = false;
#endif
	x = y = width = height = -1;
}

DragWindow::~DragWindow()
{
}

// handler for window drag events
void DragWindow::WindowDragged(DragEvent e, Point p)
{
}


END_UPP_NAMESPACE
