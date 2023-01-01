#include "CtrlCore.h"

#ifdef GUI_X11

namespace Upp {

/////////////////////////////////////////////////////////////////////////////////////////
// Constructor
DHCtrl::DHCtrl()
{
	// Sets control NOT initialized
	isInitialized = false;

	// Sets control NOT mapped
	isMapped = false;

	// Resets error contition
	isError = false;

	// Sets the user visual to null
	UserVisualInfo = 0;

	// No background painting
	backpaint = NOBACKPAINT;
//	transparent = true;
	hwnd = 0;
} // END Constructor class DHCtrl

/////////////////////////////////////////////////////////////////////////////////////////
// Destructor
DHCtrl::~DHCtrl()
{
	// Destroys the associated window and clean up stuffs
	Terminate();

} // END Destructor class DHCtrl

/////////////////////////////////////////////////////////////////////////////////////////
// Maps/unmaps the window
void DHCtrl::MapWindow(bool map)
{
	GuiLock __; 
	// no action if not initialized
	if(!isInitialized)
		return;

	if(map && !isMapped)
		XMapWindow(Xdisplay, GetWindow());
	else if(!map && isMapped)
		XUnmapWindow(Xdisplay, GetWindow());

	isMapped = map;

} // END DHCtrl::MapWndow()


/////////////////////////////////////////////////////////////////////////////////////////
// Initializes the view
bool DHCtrl::Init()
{
	GuiLock __; 
	static bool isInitializing = false;

	// Just for security sakes...
	if(isInitialized)
		return true;

	// Prevents reentrant call....
	if(isInitializing)
		return false;
	isInitializing = true;

	// Call BeforeInit user func...
	BeforeInit();

	// if display is null, error
	if(!Xdisplay)
	{
		// Call AfterInit user func...
		AfterInit(true);

		// Sets the appropriate error message
		SetErrorMessage("DHCtrl : Bad display");

		isError = true;
		isInitializing = false;
		return false;
	}

	// Calls the user visual function
	UserVisualInfo = CreateVisual();

	// If error, returns
	if(isError)
	{
		isInitializing = false;
		return false;
	}

	// Gets the default visual, if none is given
	Visual *visual;
	int Depth;
	if(UserVisualInfo)
	{
		visual = UserVisualInfo->visual;
		Depth = UserVisualInfo->depth;
	}
	else
	{
		visual = DefaultVisual(Xdisplay, DefaultScreen(Xdisplay));
		Depth = DefaultDepth(Xdisplay, DefaultScreen(Xdisplay));
	}

	// Initializes attribute setting flags
	unsigned long ValueMask =
		CWBorderPixel
		| CWColormap
		| CWSaveUnder
//		| CWBackPixel
//		| CWBorderPixel
		| CWColormap
//		| CWEventMask
//		| CWWinGravity
//		| CWBitGravity
	; // END ValueMask

	// Initializes attribute structure
	XSetWindowAttributes winAttributes;
	// creates a ColorMap, in case we're not using default visual
	winAttributes.colormap = XCreateColormap(Xdisplay, GetTopWindow()->GetWindow(), visual, AllocNone);
	winAttributes.border_pixel = 0;
	winAttributes.save_under = XFalse;
//	winAttributes.win_gravity = StaticGravity;
//	winAttributes.bit_gravity = ForgetGravity;

	// Calls the attributes user setting routine
	SetAttributes(ValueMask, winAttributes);

	// Creates the X11 window
	Rect r = GetRectInParentWindow();
	if (!hwnd) {
		hwnd = XCreateWindow
		(
			Xdisplay,									// display
	//		GetTopWindow()->GetWindow(), 				// parent
			GetParentWindow(),
	
			r.left, r.top, r.Width(), r.Height(),		// x, y, width, height
			0,											// border width
			Depth,										// depth
			InputOutput,								// class
			visual,										// visual
			ValueMask,									// value mask
			&winAttributes								// attributes
		);
	} else {
		XReparentWindow(Xdisplay, hwnd, GetParentWindow(), r.left, r.top);
		XResizeWindow(Xdisplay, hwnd, r.Width(), r.Height());
		XChangeWindowAttributes(Xdisplay, hwnd, ValueMask, &winAttributes);
	}

/*
	// Frees VisualInfo
    if (UserVisualInfo)
    {
        XFree( (char *)UserVisualInfo);
        UserVisualInfo = 0;
    }
*/
    // If problem creating window, error
    if(!hwnd)
    {
		// Call AfterInit user func...
		AfterInit(true);

		// Sets the appropriate error message
		SetErrorMessage("DHCtrl : Can't create window");

        isError = true;
		isInitializing = false;
        return false;
    }

	// Adds window to UPP managed windows
	XWindow *cw = AddXWindow(hwnd);

	cw->xic = xim ? XCreateIC
					(
						xim,
						XNInputStyle,
						XIMPreeditNothing | XIMStatusNothing,
						XNClientWindow,
						hwnd,
						XNFocusWindow,
						hwnd,
	    				NULL
	    			)
	    : NULL;

	Top* top = new Top;
	top->window = hwnd;
	SetTop(top);

	long im_event_mask = 0;
	if(cw->xic)
		XGetICValues(cw->xic, XNFilterEvents, &im_event_mask, NULL);
	XSelectInput
	(
		Xdisplay,
		hwnd,
		ExposureMask
//		| StructureNotifyMask		// *very* important, flag MUST NOT be set
		| KeyPressMask
		| FocusChangeMask
		| KeyPressMask
		| KeyReleaseMask
		| PointerMotionMask
		| ButtonPressMask
		| ButtonReleaseMask
		| PropertyChangeMask
		| VisibilityChangeMask
		| im_event_mask
	);

	int version = 5;
	XChangeProperty
	(
		Xdisplay,
		hwnd,
		XAtom("XdndAware"),
		XA_ATOM,
		32,
		0,
		(byte *)&version,
		1
	);

	// Maps the window if needed
	if(IsShown())
		MapWindow(true);

	// Flushes the display
    XFlush(Xdisplay);

    // Stores the initial control size
    CurrentSize = GetSize();

	// Exits from initializing lock
	isInitializing = false;

	// mark control as initialized
	isInitialized = true;

	// Resets the message
	isError = false;
	SetErrorMessage("");

	// Call AfterInit user func...
	AfterInit(false);

	return true;

} // END DHCtrl::Init()


/////////////////////////////////////////////////////////////////////////////////////////
// Terminates the view
void DHCtrl::Terminate(void)
{
	GuiLock __; 
	BeforeTerminate();

	if(!isInitialized)
		return;

	// Unmaps the window
	MapWindow(false);
	Window xwin = GetWindow();
	if(xwin) {
	// gathers data from XWindow (needs Input Context...)
		XWindow *cw = XWindowFromWindow(xwin);
	
		// Frees input context as needed
		if(cw && cw->xic)
		{
			XDestroyIC(cw->xic);
			cw->xic = NULL;
		}
	
		// Removes XWindow from Upp list
		RemoveXWindow(xwin);
	
		// Destroys the window
		// Not to do, it's done destroying the parent window by X11 system
	//	XDestroyWindow(Xdisplay, top->window);
	
		// Destroys created Top struct
		DeleteTop();
	}
	
	// Resets initialization and error flags
	isInitialized = false;
	isError = false;

} // END DHCtrl::Terminate()

/////////////////////////////////////////////////////////////////////////////////////////
// State handler
void DHCtrl::State(int reason)
{
	GuiLock __; 
//	Window dummy;
//	int x, y;
//	unsigned int width, height, border, depth;
	Rect r;

	// No handling if in error state
	if( isError)
		return;

	// Initializes the control if needed (and possible...)
	if(!isInitialized && GetTopWindow() && GetTopWindow()->GetWindow())
		Init();

	if(isInitialized)
	{
		switch( reason )
		{
			case FOCUS      : // = 10,
				break;

			case ACTIVATE   : // = 11,
				break;

			case DEACTIVATE : // = 12,
				break;

			case SHOW       : // = 13,
				MapWindow(IsVisible());
				break;

			case ENABLE     : // = 14,
				break;

			case EDITABLE   : // = 15,
				break;

			case OPEN       : // = 16,
				MapWindow(IsShown());
				break;

			case CLOSE      : // = 17,
				Terminate();
				break;

			case POSITION   : // = 100,
			case LAYOUTPOS  : // = 101,
				SyncNativeWindows();
				break;

			default:
				break;

		} // switch(reason)
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// Property Visual
Visual *DHCtrl::GetVisual(void)
{
	GuiLock __; 
	if(UserVisualInfo)
		return UserVisualInfo->visual;
	else
		return DefaultVisual(Xdisplay, DefaultScreen(Xdisplay));

} // END DHCtrl::getVisual()


/////////////////////////////////////////////////////////////////////////////////////////
// Property VisualInfo
XVisualInfo DHCtrl::GetVisualInfo(void)
{
	GuiLock __; 
	// if present an user visual info, just return it
	if(UserVisualInfo)
		return *UserVisualInfo;

	XVisualInfo visualInfo;
	memset(&visualInfo, 0, sizeof(visualInfo));

	// get the active visual
	Visual *visual = GetVisual();

	// gets a list of all available XVisualinfo
	XVisualInfo *v = 0;
	XVisualInfo vtemplate;
	int nVis;
	XVisualInfo *vlist = XGetVisualInfo(Xdisplay, VisualNoMask, &vtemplate, &nVis);

	// search for current visual inside the list
	if(vlist)
	{
		for (v = vlist; v < vlist + nVis; v++)
		{
			if (v->visual == visual)
			{
				visualInfo = *v;
				break;
			}
		}
	    XFree(vlist);
	}
	else
	{
		isError = true;
		ErrorMessage = "DHCtrl: no XVisualInfo for current Visual";
	}

	// returns the found XVisualInfo struct
	return visualInfo;
}

}

#endif
