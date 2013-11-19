#include "colorpopup.h"



colorpopup::colorpopup()
{
	CtrlLayout(*this, "Window title");
	c0.SColors(false);
	c1.SColors(true);
	c2.NotNull();
	c3.NoRampWheel();
	c4.NotNull().NoRampWheel();
	c5.WithVoid();
}

GUI_APP_MAIN
{
	DUMP(VoidColor());
	colorpopup().Run();
}
