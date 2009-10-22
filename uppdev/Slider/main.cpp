#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	StaticRect        dynarect;
	SliderCtrl        slider, slider2;
	Label             text;

	void Slider() {
		dynarect.SetRectX(50, ~slider);
		text = "\1[C6*/@b " + AsString(~slider);
	}

	typedef App CLASSNAME;

	App() {
		Sizeable().Zoomable();

		slider <<= THISBACK(Slider);
		slider.Range(700);
		slider2.Range(600);
		slider <<= 250;
		Add(slider.RightPosZ(5, 30).VSizePos(10, 10));
		slider.WantFocus();
		slider2.WantFocus();
		Add(slider2.BottomPosZ(5, 30).HSizePos(10, 10));
		Slider();

		dynarect.Color(SRed);
		Add(dynarect.VSizePos(40, 40));

		Add(text.LeftPos(0, 200).TopPos(0, 40));
	}
};

GUI_APP_MAIN
{
	App().Run();
}
