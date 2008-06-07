#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow {
	bool numbers_enabled;

	void Exit()
	{
		Close();
	}

	void EnableNumbers()
	{
		numbers_enabled = !numbers_enabled;
	}

	void ShowNumber(int n)
	{
		PromptOK(AsString(n));
	}

	void SubMenu(Bar& bar)
	{
		for(int i = 0; i < 10; i++)
			bar.Add(AsString(i), THISBACK1(ShowNumber, i));
	}

	void Menu(Bar& bar)
	{
		bar.Add("Enable numbers", THISBACK(EnableNumbers))
		   .Check(numbers_enabled);
		bar.Add(numbers_enabled, "Numbers", THISBACK(SubMenu));
		bar.Add("Exit", THISBACK(Exit))
		   .Key(K_CTRL_E);
	}

	void MainBar(Bar& bar)
	{
		bar.Add("Menu", THISBACK(Menu));
	}

	MenuBar menu;

	typedef App CLASSNAME;

	App()
	{
		numbers_enabled = false;
		AddFrame(menu);
		menu.Set(THISBACK(MainBar));
	}
};

GUI_APP_MAIN
{
	App().Run();
}
