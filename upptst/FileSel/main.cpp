#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	FileSelector sel;
	sel.AllFilesType();
	sel.Type("Test2", "*.tst");
	sel.ActiveType(1);
	for(int i = 0; i < sel.GetCount(); i++)
		DDUMP(sel[i]);
	sel.Multi();
	if(!sel.ExecuteSaveAs("Test!"))
		return;
	for(int i = 0; i < sel.GetCount(); i++)
		DDUMP(sel[i]);
	sel = "/home/cxl/Temp/Gui2/main.cpp";
	if(!sel.ExecuteSaveAs("as"))
		return;
	DDUMP(~sel);
	sel.NoAsking();
	if(!sel.ExecuteSaveAs("as"))
		return;
	return;
/*
	DocEdit a;
	TopWindow w1, w2;
	w1.Add(a.SizePos());
	w1.OpenMain();
	Button b;
	b <<= callback(Sel);
	w2.Add(b.SizePos());
	w2.Open(&w1);
	w2.Run();
*/
}
