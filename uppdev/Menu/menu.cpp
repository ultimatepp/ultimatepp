#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow {
	bool numbers_enabled;
	EditString test;
	Point p;
	ArrayCtrl list;
	
	virtual void MouseMove(Point p, dword keyflags);
	virtual void Paint(Draw& w);

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
		bar.Add("Zoom &Out", CtrlImg::error(), THISBACK(Exit)).Bold();
		bar.Add("Zoom Aut", CtrlImg::error(), THISBACK(Exit));
		bar.Add("Zoom Aut", CtrlImg::error(), THISBACK(Exit));
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
	
	void CloseSubMenu()
	{
		list.Add("Close");
	}
	
	void OpenSubMenu()
	{
		list.Add("Open");
	}

	MenuBar menu;

	typedef App CLASSNAME;

	App()
	{
		numbers_enabled = true;
		AddFrame(menu);
		menu.Set(THISBACK(MainBar));
		menu.WhenSubMenuClose = THISBACK(CloseSubMenu);
		menu.WhenSubMenuOpen = THISBACK(OpenSubMenu);
		list.AddColumn();
		Add(list.RightPos(0, 200).VSizePos());
	}
};

void App::MouseMove(Point p, dword keyflags)
{
	this->p = p;
	Refresh();
}

void App::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White());
	w.DrawText(100, 100, AsString(p));
}


GUI_APP_MAIN
{
	App().Run();
}
