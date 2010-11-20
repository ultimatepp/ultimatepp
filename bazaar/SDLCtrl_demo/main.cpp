#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <SDL/SDLCtrl.h>
#include <SDLCtrl_demo/demo.h>

#define LAYOUTFILE <SDLCtrl_demo/demo.lay>
#include <CtrlCore/lay.h>

void DemoThread(SDLExample *sdlCtrl) {
	sdlCtrl->Demo();
}

struct SDLCtrl_Demo : public WithMain<TopWindow> {
	bool isfullscreen;
typedef SDLCtrl_Demo CLASSNAME;	
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
	SDLCtrl_Demo(bool fsc) {
		CtrlLayout(*this, "SDLCtrl Example");
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

GUI_APP_MAIN
{
	SDLCtrl_Demo *win;
	bool fullscreen = false;
	
	for (;;) {
		win = new SDLCtrl_Demo(fullscreen);	
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
