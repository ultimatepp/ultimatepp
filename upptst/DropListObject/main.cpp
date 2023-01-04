#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	DropList dl, dl2;
	
	MyApp() {
		dl2.ListObject().EvenRowColor();
		Add(dl.TopPosZ(0).LeftPosZ(0, 200));
		Add(dl2.TopPosZ(50).LeftPosZ(0, 200));
		for(int i = 0; i < 20; i++) {
			dl.Add(i);
			dl2.Add(i);
		}
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
