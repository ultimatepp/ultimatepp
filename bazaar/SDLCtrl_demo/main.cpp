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
	SDLCtrl_Demo() {
		CtrlLayout(*this, "SDLCtrl Example");
		Zoomable().Sizeable();
		
		butRun.WhenAction = THISBACK(Demo);
		butStop.WhenAction = THISBACK(Stop);
		butStop.Disable();
	}
};

GUI_APP_MAIN
{
	SDLCtrl_Demo().Run();
}
