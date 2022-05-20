#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	FileSel sel;
	sel.Type("all", "*.*");
	sel.NoExeIcons();
	sel.ExecuteSaveAs();
	sel.NoExeIcons(false);
	sel.ExecuteSaveAs();
}
