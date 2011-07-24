#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	int q = PromptYesNo("Just a test");
	DDUMP(q);
	PromptOK(AsString(q));
}
