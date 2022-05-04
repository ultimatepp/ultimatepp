#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <Gui16b/dlg.lay>
#include <CtrlCore/lay.h>

struct MyAppWindow : public WithDlgLayout<Upp::TopWindow> {
	MyAppWindow() {
		CtrlLayout(*this, "MyDialog");
	}
};

GUI_APP_MAIN
{
	MyAppWindow().Run();
}
