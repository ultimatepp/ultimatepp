#include <CtrlLib/CtrlLib.h>

using namespace Upp;

void Fn(Color c, ImageBuffer& ib)
{
	DUMP(c);
}

GUI_APP_MAIN
{
	Tuple<int, Color> c;
	
	c.b = Cyan();

//	ImageBuffer ib = Rescale(CtrlImg::exclamation(), 500, 500);
	Image h = CtrlImg::exclamation();
	ImageBuffer ib(h);
	Fn(Red(), ib);
	Image img = CtrlImg::exclamation();
	Value v = img;

	LOG("Here");
}
