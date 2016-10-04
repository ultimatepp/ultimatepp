#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	DUMP(Draw::GetStdFontCy() + 4);
	DUMP(ScreenInfo().GetPixelsPerInch().cy);
}
