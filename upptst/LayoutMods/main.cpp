#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <LayoutMods/Layout.lay>
#include <CtrlCore/lay.h>

GUI_APP_MAIN
{
	WithTestLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Test");
	dlg.Execute();
}
