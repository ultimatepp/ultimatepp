#include "colorpopup.h"



colorpopup::colorpopup()
{
	CtrlLayout(*this, "Window title");
	c0.SColors(false);
	c1.SColors(true);
	c2.NotNull();
	c3.NoRampWheel();
	c4.NotNull().NoRampWheel();
}

GUI_APP_MAIN
{
	colorpopup().Run();
}
