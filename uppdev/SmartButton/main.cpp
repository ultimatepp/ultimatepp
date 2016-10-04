#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class MyApp : public TopWindow {
	Button button;

	void Click() {
		if(PromptYesNo("Button was clicked. Do you want to quit?"))
			Break();
	}

public:
	typedef MyApp CLASSNAME;

	MyApp() {
		Title("Hello world");
		button.SetLabel("Hello world!");
		button <<= THISBACK(Click);
		Add(button.HSizePos(100, 100).VSizePos(100, 100));
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
