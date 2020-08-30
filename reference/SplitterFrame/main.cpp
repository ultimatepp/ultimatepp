#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	SplitterFrame sf;
	ArrayCtrl list;

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), SColorPaper());
		w.DrawText(10, 10, "This is view area", Arial(30));
	}

	MyApp() {
		AddFrame(sf.Left(list, 100));
		list.AddColumn("List");
		for(int i = 0; i < 100; i++)
			list.Add(FormatIntRoman(i, true));
		AddFrame(InsetFrame());
		sf.SizeMin(40).MinSize(40);
		Sizeable();
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
