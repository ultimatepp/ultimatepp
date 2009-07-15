#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	Size sz(10, 10);
	ImageDraw id(sz);
	id.DrawRect(sz, Blue);
	ImageBuffer ib(id);	// I beleive this is the problem right here
	Image m_view = ib;
}
