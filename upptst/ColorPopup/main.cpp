#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ColorPopup/colorpopup.lay>
#include <CtrlCore/lay.h>

class MyApp : public WithMyAppLayout<TopWindow> {
public:
	typedef MyApp CLASSNAME;
	MyApp();
};

MyApp::MyApp()
{
	CtrlLayout(*this, "Window title");
	c0.SColors(false);
	c1.WithText().SColors(true);
	c2.NotNull();
	c3.NoRampWheel();
	c4.NotNull().NoRampWheel();
	c5.WithVoid();
}

GUI_APP_MAIN
{
//	SetStdFont(Arial(25));
	SyncUHDMode();
	MyApp().Run();
}
