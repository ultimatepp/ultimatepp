#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	ProgressIndicator progress;
	SliderCtrl        slider;
	Label             text;

	void Slider() {
		progress.Set(~slider, 100);
		text = "\1[C6*/@b " + AsString(~slider);
	}

	typedef App CLASSNAME;

	App() {
		Add(slider.BottomPosZ(5, 30).HSizePos(10, 10));
		Add(progress.VSizePos(10, 40).HCenterPos(40));
		Add(text.LeftPos(5, 200).TopPos(5, 40));
		slider <<= THISBACK(Slider);
		slider.Range(100);
		slider <<= 50;
		Slider();
		Sizeable().Zoomable();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
