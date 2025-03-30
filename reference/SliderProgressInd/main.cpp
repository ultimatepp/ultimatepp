#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	ProgressIndicator progress;
	SliderCtrl        slider;
	Label             text;

	typedef App CLASSNAME;

	App() {
		*this << slider.BottomPosZ(5, 30).HSizePos(10, 10)
		      << progress.VSizePosZ(10, 40).HCenterPos(40)
		      << text.LeftPosZ(5, 200).TopPosZ(5, 40);
		slider << [=] {
			progress.Set(~slider, 100);
			text = "\1[C6*/@b " + AsString(~slider);
		};
		slider.Range(100);
		slider <<= 50;
		slider.WhenAction();
		Sizeable().Zoomable();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
