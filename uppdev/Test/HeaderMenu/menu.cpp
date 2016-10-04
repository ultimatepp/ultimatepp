#include "menu.h"

class TestWindow : public TopWindow {
public:
	typedef TestWindow CLASSNAME;
	TestWindow();

	void OpenMenu0();
	void OpenMenu(int column);
	void Menu(Bar& bar);
	void MenuFn(int i);

private:
	ArrayCtrl array;
};

TestWindow::TestWindow()
{
	array.AddColumn("Column 1").HeaderTab().WhenAction = THISBACK1(OpenMenu, 0);
	array.AddColumn("Column 2").HeaderTab().WhenAction = THISBACK1(OpenMenu, 1);
	array.AddColumn("Column 3").HeaderTab().WhenAction = THISBACK1(OpenMenu, 2);
	*this << array.HSizePos(4, 4).VSizePos(4, 4);
	array.WhenLeftClick = THISBACK(OpenMenu0);
}

void TestWindow::OpenMenu0()
{
	MenuBar::Execute(THISBACK(Menu));
}

void TestWindow::OpenMenu(int column)
{
	HeaderCtrl& hdr = array.HeaderObject();
	Rect rc = hdr.GetTabRect(column) + hdr.GetScreenView().TopLeft();
	array.SetFocus();
	MenuBar::Execute(THISBACK(Menu));
//	MenuBar::Execute(this, THISBACK(Menu), Point(rc.left, rc.bottom));
}

void TestWindow::Menu(Bar& bar)
{
	bar.Add("Function 1", THISBACK1(MenuFn, 1));
	bar.Add("Function 2", THISBACK1(MenuFn, 2));
	bar.Add("Function 3", THISBACK1(MenuFn, 3));
}

void TestWindow::MenuFn(int i)
{
	PromptOK(NFormat("Menu fn #%d", i));
}

GUI_APP_MAIN {
	TestWindow().Run();
}
