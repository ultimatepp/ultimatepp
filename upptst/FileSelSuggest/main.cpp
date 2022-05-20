#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	FileSel sel;
	sel.Type("all", "*.*");
	sel.DefaultName("test");
	sel.ExecuteSaveAs();
}
