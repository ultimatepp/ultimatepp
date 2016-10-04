#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void RightDown(Point p, dword keyflags) {
		int result = Null;
		MenuBar menu;
		for(int i = 0; i < 10; i++)
			menu.Add(AsString(i), [=, &result] { result = i; });
		menu.Execute();
		if(IsNull(result))
			PromptOK("Menu was cancelled");
		else
			PromptOK("You have selected " + AsString((int)result));
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
