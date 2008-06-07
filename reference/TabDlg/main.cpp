#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <TabDlg/TabDlg.lay>
#include <CtrlCore/lay.h>

GUI_APP_MAIN
{
	TabDlg dlg;
	WithTab1Layout<ParentCtrl> tab1;
	WithTab2Layout<ParentCtrl> tab2;
	dlg(tab1, "Tab1")(tab2, "Tab2")
	   .OKCancel()
	   .Title("Tab dialog");
	dlg.Run();
}
