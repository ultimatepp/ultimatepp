#include "Slider.h"

Slider::Slider()
{
	CtrlLayout(*this, "Slider example");
	
	auto Show = [=] {
		vslider_val = ~~vslider;
		vslider_val2 = ~~vslider2;
		hslider_val = ~~hslider;
		hslider_val2 = ~~hslider2;
	};

	vslider.Range(100);
	vslider << [=] { Show(); };
	vslider <<= 0;

	vslider2.MinMax(100, 0);
	vslider2 << [=] { Show(); };
	vslider2 <<= 0;

	hslider.Range(100);
	hslider << [=] { Show(); };
	hslider <<= 0;

	hslider2.MinMax(100, 0);
	hslider2 << [=] { Show(); };
	hslider2 <<= 0;
	
	Show();
}

GUI_APP_MAIN
{
	Slider().Run();
}
