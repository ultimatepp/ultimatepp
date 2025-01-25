#ifndef _Slider_Slider_h
#define _Slider_Slider_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Slider/Slider.lay>
#include <CtrlCore/lay.h>

class Slider : public WithSliderLayout<TopWindow> {
public:
	Slider();
};

#endif
