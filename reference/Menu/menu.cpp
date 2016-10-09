#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow {
	bool numbers_enabled;

	void Menu(Bar& bar)
	{
		bar.Add("Enable numbers", [=] { numbers_enabled = !numbers_enabled; })
		   .Check(numbers_enabled);
		bar.Sub(numbers_enabled, "Numbers", [=](Bar& bar) {
			for(int i = 0; i < 10; i++)
				bar.Add(~AsString(i), [=] { PromptOK(AsString(i)); });
		});
		bar.Add("Exit", [=] { Close(); })
		   .Key(K_CTRL_E);
	}

	void MainBar(Bar& bar)
	{
		bar.Sub("Numbers", THISFN(Menu));
		bar.Sub("Items", [=](Bar& bar) {
			bar.Add("Item 1", [&] { Exclamation("Item 1 invoked"); });
			bar.Add("Item 2", [&] { Exclamation("Item 2 invoked"); });
		});
	}

	MenuBar menu;

	typedef App CLASSNAME;

	App()
	{
		numbers_enabled = false;
		AddFrame(menu);
		menu.Set(THISFN(MainBar));
	}
};

GUI_APP_MAIN
{
	App().Run();
}
