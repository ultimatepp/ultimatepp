#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
#ifdef PLATFORM_WIN32
	DDUMP(GetDeviceCaps(GetDC(NULL), SHADEBLENDCAPS) & SB_PIXEL_ALPHA);
#endif
}
