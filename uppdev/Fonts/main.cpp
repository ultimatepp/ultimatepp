#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	DDUMP(GetGlyphInfo(Font().FaceName("Courier New"), 35798).IsNormal());
	DDUMP(GetGlyphInfo(Font().FaceName("Verdana"), 35798).IsNormal());
	DDUMP(GetGlyphInfo(Font().FaceName("Symbol"), 35798).IsNormal());
}
