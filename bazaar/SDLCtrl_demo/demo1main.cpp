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
		isfullscreen = !isfullscreen;
		Close();
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
	SDLCtrl_Demo1(bool fsc) {
		CtrlLayout(*this, "Unodgs example in a SDLCtrl");
		Zoomable().Sizeable();
		
		butRun.WhenAction = THISBACK(Demo);
		butStop.WhenAction = THISBACK(Stop);
		butStop.Disable();
		butFullScreen.WhenAction = THISBACK(SetFullScreen);
		if (fsc)
			butFullScreen.SetLabel("Window");
		else	
			butFullScreen.SetLabel("Fullscreen");
		isfullscreen = fsc;
	}
};

void Run_Demo1() {
	SDLCtrl_Demo1 *win;
	bool fullscreen = true;
	
	for (;;) {
		win = new SDLCtrl_Demo1(fullscreen);	
		if (!fullscreen)
			win->Run();
		else
			win->FullScreen().Run();
		bool isfullscreen = win->isfullscreen;
		delete win;
		if (isfullscreen == fullscreen)
			break;
		else
			fullscreen = isfullscreen;
	}
}