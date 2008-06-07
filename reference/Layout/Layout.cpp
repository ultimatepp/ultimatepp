#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Layout/Layout.lay>
#include <CtrlCore/lay.h>

GUI_APP_MAIN
{
	WithDialogLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Enter a number");
	dlg.number <<= 10;
	for(;;)
		switch(dlg.Execute()) {
		case IDOK:
			PromptOK(AsString(~dlg.number));
			break;
		case IDCANCEL:
			return;
		}
}
