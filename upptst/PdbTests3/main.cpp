#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	Tuple<int, Color> c;
	
	c.b = Cyan();

//	ImageBuffer ib = Rescale(CtrlImg::exclamation(), 500, 500);
	ImageBuffer ib = CtrlImg::exclamation();
	Image img = CtrlImg::exclamation();
	Value v = img;

	LOG("Here");
}
