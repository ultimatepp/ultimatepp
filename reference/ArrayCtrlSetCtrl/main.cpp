#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	Array<Option> option;
	ArrayCtrl a;
	
	void Do(int ii)
	{
		option[ii].SetFocus();
		for(int i = 0; i < option.GetCount(); i++)
			option[i].Enable(i == ii || option[ii]);
	}
	
	App() {
		a.AddColumn("Option");
		for(int i = 0; i < 300; i++) {
			a.Add(bool(i & 4));
			a.SetCtrl(i, 0, option.Add().SetLabel("Option " + AsString(i)));
			option.Top() << [=] { Do(i); };
		}
		a.SetLineCy(Draw::GetStdFontCy() + 8);
		Add(a.SizePos());
		Sizeable();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
