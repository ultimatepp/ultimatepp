#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	ArrayCtrl list;
	TimeCallback tm;

	MyApp() {
		list.AddColumn("Test");
		list.Add("Simple");
		for(int i = 0; i < 100; i++)
			list.Add("Long " + String('X', i * 100));
		Sizeable().Zoomable();
		Add(list.VSizePosZ(Zx(20), 0).LeftPosZ(0, 100));
		
		tm.Set(-500, [this] { list.Add("Long " + String('X', Random(20))); list.GoEnd(); });
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
