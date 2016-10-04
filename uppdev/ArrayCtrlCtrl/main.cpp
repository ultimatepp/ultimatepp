#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	Array<EditInt> option;
	ArrayCtrl a;

	void Do(int ii)
	{
		option[ii].SetFocus();
		for(int i = 0; i < option.GetCount(); i++)
			option[i].Enable(i == ii || option[ii]);
	}

	bool CheckRow()
	{
	//	if((int)a.Get(0) <= 0) {
	//		Exclamation("Chyba");
	//		return false;
	//	}
		return true;
	}
	
	void Enter()
	{
		PromptOK("Enter!");
	}

	typedef App CLASSNAME;

	App() {
		a.AddColumn("Option");
		for(int i = 0; i < 300; i++) {
			a.Add(i & 4);
			a.SetCtrl(i, 0, option.Add());
			option.Top() <<= i;
//			option.Top() <<= THISBACK1(Do, i);
		}
		a.SetLineCy(10, 40);
		option[10].SizePos();
		option[10].WhenEnter = THISBACK(Enter);
		a.WhenAcceptRow = THISBACK(CheckRow);
		Add(a.SizePos());
		Sizeable();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
