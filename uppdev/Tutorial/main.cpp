#include <CtrlLib/CtrlLib.h>

struct MyAppWindow : TopWindow {
	Button button;

	typedef MyAppWindow CLASSNAME;

	MyAppWindow() {
		Title("My application with font-zoomed button").Sizeable();
		*this << button.SetLabel("Button").LeftPosZ(10, 64).TopPosZ(10, 24);
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
