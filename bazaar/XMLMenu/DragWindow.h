#ifndef _DragWindow_h_
#define _DragWindow_h_

#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

// a window that can react to dragging events
// i.e. report moves of itself to an handler
class DragWindow : public TopWindow
{
	public:

		typedef enum { DRAG_START, DRAG_DRAG, DRAG_END } DragEvent;
		
	private:
	
		// dragging flag
		bool dragging;
	
#ifdef PLATFORM_WIN32
		bool sizeMoving;
#endif
		// position and size of dragged window
		int x, y;
		int width, height;
		
	protected:

#ifdef PLATFORM_WIN32

		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

#elif defined(flagX11)

		virtual void    EventProc(XWindow& w, XEvent *event);

#elif defined(flagGTK)

		gulong handlerId;
		GtkWindow *gtkWindow;
		void connectInternalHandler(void);
		static gboolean DragWindowGtkEvent(GtkWidget *widget, GdkEvent *event, gpointer user_data);
		virtual void EventProc(GdkEvent *event);
		
#endif
	
		// handler for window drag events
		virtual void WindowDragged(DragEvent e, Point p);
	
	public:
	
		DragWindow();
		~DragWindow();
};

END_UPP_NAMESPACE

#endif
