#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	Ctrl::SetUHDEnabled();
	PromptOK("This is a very long line in PromptOK, line1 line2 line3 line4 line5 line6 line7 "
	         "line8 line9 line10 eol11 line12 line13");
}
