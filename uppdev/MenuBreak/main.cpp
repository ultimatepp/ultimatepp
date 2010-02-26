#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class MenuTest : public TopWindow {
public:
	MenuBar menu;

	typedef MenuTest CLASSNAME;
	MenuTest()
	{
		ef.WhenEnter = THISBACK(EnterPressed);
	}
	void ThisMenu(Bar& bar)
	{
		bar.Gap(10);
		bar.Add(ef.SizePos(), Size(200, 20));
		bar.Gap(10);
		bar.Add("One", THISBACK(Action));
		bar.Add("Two", THISBACK(Action));
	}
	
	void EnterPressed()
	{
		menu.EndLoop();
	}
	void RightDown(Point p, dword d)
	{
		menu.Set(THISBACK(ThisMenu));
		menu.Execute();
	}
	void Action() {}
	EditField ef;	
};

GUI_APP_MAIN
{
	MenuTest().Run();
}
