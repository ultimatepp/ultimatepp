#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct EditStringSpecial : EditString {
	ArrayCtrl *GetArrayCtrl() {
		for(Ctrl *q = GetParent(); q; q = q->GetParent())
			if(ArrayCtrl *a = dynamic_cast<ArrayCtrl *>(q))
				return a;
		return NULL;
	}
	
	static void DoMenu(EditStringSpecial *x) {
		MenuBar bar;
		x->StdBar(bar);
		ArrayCtrl *a = x->GetArrayCtrl();
		if(a) {
			bar.Separator();
			a->WhenBar(bar);
		}
		bar.Execute();
	}

	void MyBar(Bar& bar)
	{
		PostCallback(callback1(DoMenu, this));
	}

	typedef EditStringSpecial CLASSNAME;

	EditStringSpecial() {
		WhenBar = THISBACK(MyBar);		
	}
};

GUI_APP_MAIN
{
	ArrayCtrl a;
	a.Appending().Removing();
	a.AddColumn("Text").Ctrls<EditStringSpecial>();
	for(int i = 0; i < 300; i++)
		a.Add(AsString(i));
	a.SetLineCy(Draw::GetStdFontCy() + 8);

	TopWindow app;
	app.Add(a.SizePos());
	app.Sizeable();
	app.Run();
}
