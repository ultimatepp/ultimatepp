#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct Test : TopWindow {
	ArrayCtrl ar;
	Test() {
		SetRect(0, 0, 640, 480);
		Add(ar.SizePos());
		ar.MultiSelect();
		ar.AddColumn("Col 1", 1);
		for (int i=0; i<20; ++i) ar.Add(i);
		ar.WhenSel = [this] {
			RDUMP(ar.GetCursor());
		};
	}
};

GUI_APP_MAIN {
	Test().Run();
}
