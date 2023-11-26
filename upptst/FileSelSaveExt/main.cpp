#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	FileSel sel;
	sel.ActiveDir(GetExeFolder());
	sel.Type("files", "*.txt *.cpp");
	sel.Type("files", "*.c");
	sel.ExecuteSaveAs();
	DDUMP(~sel);
	sel.Type("all", "*.*");
	sel.ExecuteSaveAs();
	DDUMP(~sel);
}
