#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	Button button;

	MyAppWindow() {
		Title("My application with button");
		Add(button.LeftPos(10, 200).TopPos(10, 60));
		button.SetLabel("Click me!");
		button << [=] {
			PromptOK("You have clicked the button!");
		};
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
