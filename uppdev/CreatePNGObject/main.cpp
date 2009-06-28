#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	String chk0 = AsQTF(CreatePNGObject(CtrlImg::exclamation(), 400, 400));
	PromptOK(chk0);
}

