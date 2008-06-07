#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Gui16/dlg.lay>
#include <CtrlCore/lay.h>

struct MyApp : public WithDlgLayout<TopWindow> {
	MyApp() {
		CtrlLayout(*this, "MyDialog");
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
