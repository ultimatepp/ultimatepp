#include <CtrlLib/CtrlLib.h>

class MyApp : public TopWindow {
	Button button;

	void Click(String s) {
		button.SetLabel("I was clicked, YOHOO!");
		PromptOK(s);
		button <<= THISBACK1(Click, "Button was clicked once again!");
	}

public:
	typedef MyApp CLASSNAME;

	MyApp() {
		Title("Testing of Ultimate++");
		button.SetRect(100, 100, 200, 50);
		button.SetLabel("Click me!");
		button <<= THISBACK1(Click, "Button was clicked for the first time.");
		Add(button);
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
