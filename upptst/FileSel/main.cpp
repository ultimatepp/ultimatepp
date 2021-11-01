#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	FileSelNative sel;
	sel.AllFilesType();
	sel.Type("Test2", "*.tst");
	sel.Type("All", "*.*");
	sel.Type("All 2", "*");
	sel.ActiveType(1);
	while(sel.ExecuteSaveAs("Test!")) {
		for(int i = 0; i < sel.GetCount(); i++)
			DDUMP(sel[i]);
		sel.Multi();
		sel.NoAsking();
	}
	sel = GetDataFile("main.cpp");
	if(!sel.ExecuteSaveAs("as"))
		return;
	DDUMP(~sel);
}
