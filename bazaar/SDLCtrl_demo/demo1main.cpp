#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <SDL/SDLCtrl.h>
#include <SDLCtrl_demo/demo1.h>

#define LAYOUTFILE <SDLCtrl_demo/demo1.lay>
#include <CtrlCore/lay.h>

struct SDLCtrl_Demo1 : public WithDemo1<TopWindow> {
	bool isfullscreen;
typedef SDLCtrl_Demo1 CLASSNAME;	
	void Demo() {
		butRun.Disable();
		butStop.Enable();
		
		sdl.Demo();
		
		butRun.Enable();
		butStop.Disable();
	}
	void Stop() {
		sdl.done = true;
	}
	virtual void Close() {
		sdl.done = true;
		TopWindow::Close();
	}
	void SetFullScreen() {
// Alendar code 
#ifdef PLATFORM_WIN32	
		static dword style;
		static Rect normalwindowrect;

		if (!isfullscreen) {
			// Get the current Top window's style bits
			style = GetStyle();
			// Strips the caption bit, which is the title bar
			style = (style & ~WS_CAPTION);
			// Save the "overlapped" or normal window shape
			normalwindowrect = GetRect(); 
			SetWindowLong(GetHWND(), GWL_STYLE, style);  
				
			// Get the full size of the screen
			long cx = GetSystemMetrics(SM_CXSCREEN);
   			long cy = GetSystemMetrics(SM_CYSCREEN);
   			// Expand the window to full size
	   		SetWindowPos(GetHWND(),HWND_TOP,0,0,cx,cy,SWP_SHOWWINDOW); 
			//or use Maximize(false) if you like a 
			//delayed resizing, even with zoom effects turned off
		} else {
			// Set the caption bit back so we can get our title bar back
			style |= WS_CAPTION;
			// Pass to windows (Style() method gets confused on SyncCaption0)
			SetWindowLong(GetHWND(), GWL_STYLE, style);
			// Return size to normal; key flag is SWP_FRAMECHANGED
			SetWindowPos(GetHWND(), HWND_TOP, 
						normalwindowrect.left, 
						normalwindowrect.top, 
						normalwindowrect.Width(), 
						normalwindowrect.Height(), SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		}
#else	
// Oan1971 code
		Sizeable(!isfullscreen);
		FullScreen(isfullscreen);
		
		XEvent event;
		event.xclient.type = ClientMessage;
		event.xclient.serial = 0;
		event.xclient.send_event = true;
		event.xclient.message_type = XAtom("_NET_WM_STATE");
		event.xclient.window = GetWindow();
		event.xclient.format = 32;
		event.xclient.data.l[0] = isfullscreen;
		event.xclient.data.l[1] = XAtom("_NET_WM_STATE_FULLSCREEN");
		event.xclient.data.l[2] = 0;
		event.xclient.data.l[3] = 0;
		event.xclient.data.l[4] = 0;
		
		XSendEvent(Xdisplay, Xroot, false, SubstructureRedirectMask | SubstructureNotifyMask, &event);
		
		if (isfullscreen)
			SetRect(0, 0, Xwidth, Xheight);
#endif	
		isfullscreen = !isfullscreen;
		butFullScreen.SetLabel(!isfullscreen ? "Full Screen" : "Window");
	}
	
	virtual void MouseMove(Point p, dword flags) {
		static bool fill = false;
		if (isfullscreen && !sdl.done) {
			if (p.y < GetSize().cy-50) {
				if (!fill) {
					sdl.SetFrame(NullFrame()).SizePos();
					fill = true;
				}
			} else {
				if (fill) {
					sdl.SetFrame(InsetFrame()).HSizePosZ(4, 4).VSizePosZ(4, 28);
					fill = false;
				}
			}
		}
	}
	SDLCtrl_Demo1() {
		CtrlLayout(*this, "Unodgs example in a SDLCtrl");
		Zoomable().Sizeable();
		
		butRun.WhenAction = THISBACK(Demo);
		butStop.WhenAction = THISBACK(Stop);
		butStop.Disable();
		butFullScreen.WhenAction = THISBACK(SetFullScreen);
		butFullScreen.SetLabel("Full Screen");
		isfullscreen = false;
	}
};

void Run_Demo1() {
	SDLCtrl_Demo1().Run();
}