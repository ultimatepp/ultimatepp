#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow {
	bool numbers_enabled;
	EditString test;

	void Action2(int a, int b) { PromptOK(AsString(a) + AsString(b)); }

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
			bar.Add("xxxxxx " + AsString(i), THISBACK1(ShowNumber, i));
	}

	void Menu(Bar& bar)
	{
		bar.Add("THISBACK2", THISBACK2(Action2, 10, 11));
		bar.Add("Enable numbers", THISBACK(EnableNumbers))
		   .Check(numbers_enabled);
		bar.Add(numbers_enabled, "Numbers", CtrlImg::cut(), THISBACK(SubMenu));
		bar.Add(numbers_enabled, "Numbers", CtrlImg::information(), THISBACK(SubMenu));
		bar.Separator();
		bar.Add("Zoom &Out", CtrlImg::error(), THISBACK(Exit));
		bar.Add("Exit", THISBACK(Exit))
		   .Key(K_CTRL_E);
	}

	void BigMenu(Bar& bar)
	{
		for(int i = 0; i < 100; i++)
			bar.Add(AsString(i), THISBACK1(ShowNumber, i));
	}

	void MainBar(Bar& bar)
	{
		bar.Add("Problem", THISBACK(Menu));
		bar.Add("Menu", THISBACK(Menu));
		bar.Add("Menu", THISBACK(Menu));
		bar.Add("BigMenu", THISBACK(BigMenu));
		bar.AddNC(test);
	}

	virtual void RightDown(Point p, dword)
	{
		MenuBar bar;
		MainBar(bar);
		bar.Execute();
	}

	virtual void LeftDown(Point p, dword)
	{
		MenuBar bar;
		BigMenu(bar);
		bar.Execute();
	}

	MenuBar menu;

	typedef App CLASSNAME;

	App()
	{
		numbers_enabled = true;
		AddFrame(menu);
		menu.Set(THISBACK(MainBar));
	}
};

GUI_APP_MAIN
{
	App().Run();
}
