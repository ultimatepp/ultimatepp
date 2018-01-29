#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <OptionBox/test.lay>
#include <CtrlCore/lay.h>

GUI_APP_MAIN
{
	WithTestLayout<TopWindow> dlg;
	CtrlLayoutExit(dlg, "Test");
	dlg.Run();
}
