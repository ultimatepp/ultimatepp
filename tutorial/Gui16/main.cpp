#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Gui16/dlg.lay>
#include <CtrlCore/lay.h>

struct MyAppWindow : public WithDlgLayout<TopWindow> {
	MyAppWindow() {
		CtrlLayout(*this, "MyDialog");
	}
};

GUI_APP_MAIN
{
	MyAppWindow().Run();
}
