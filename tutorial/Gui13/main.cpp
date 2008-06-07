#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	Button button;

	void Click() { PromptOK("You have clicked the button!"); }

	typedef MyAppWindow CLASSNAME;

	MyAppWindow() {
		Title("My application with button");
		Add(button.LeftPos(10, 100).TopPos(10, 30));
		button.SetLabel("Click me!");
		button <<= THISBACK(Click);
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
