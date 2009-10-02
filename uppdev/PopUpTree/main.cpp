#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	PopUpTree tree;
	
	tree.SetRoot(CtrlImg::cut(), "Root");
	tree.Add(0, Null, "Test");
	
	win.Add(tree.HSizePos().VSizePos(0, Ctrl::STDSIZE));
	win.Run();
}

