#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	DropTree dt;
	
	dt->SetRoot(CtrlImg::Home(), "Home");
	for(int i = 0; i < 10; i++) {
		int id = dt->Add(0, CtrlImg::Computer(), "Computer " + AsString(i + 1));
		for(int j = 0; j < i; j++)
			dt->Add(id, CtrlImg::File(), "File " + AsString(j + 1));
	}

	dt.AutoResize();
	
	win.Add(dt.LeftPosZ(10, 200).TopPosZ(10));
	win.Run();
}
