#ifndef _slidertest_slidertest_h
#define _slidertest_slidertest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <slidertest/slidertest.lay>
#include <CtrlCore/lay.h>



class slidertest : public WithslidertestLayout<TopWindow> {
public:
	typedef slidertest CLASSNAME;
	slidertest();

	void OnSlider( SliderCtrl* sld, StaticText* txt );

};

#endif

