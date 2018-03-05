#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	FileSel fs;
	fs.Types("*.*");
	
	DropList dl;
	dl.Add("TEST");
	fs.FileCtrl(dl, Zx(100));
	
	fs.ExecuteSaveAs();
}
