#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	DDUMP(GetDeviceCaps(GetDC(NULL), SHADEBLENDCAPS) & SB_PIXEL_ALPHA);
}
