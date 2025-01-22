#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	StaticRect        dynarect;
	SliderCtrl        slider;
	Label             text;

	typedef App CLASSNAME;

	App() {
		Sizeable().Zoomable();

		slider << [=] {
			dynarect.SetRectX(50, ~slider);
			text = "\1[C6*/@b " + AsString(~slider);
		};

		slider.Range(700);
		slider <<= 250;

		Add(slider.BottomPosZ(5, 30).HSizePos(10, 10));

		dynarect.Color(SRed);
		Add(dynarect.VSizePosZ(40, 40));

		Add(text.LeftPosZ(0, 200).TopPosZ(0, 40));

		slider.WhenAction();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
