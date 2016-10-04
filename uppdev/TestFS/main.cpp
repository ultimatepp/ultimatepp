#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	FileSel fs;
	while(fs.ExecuteOpen() != IDOK)
		DUMP(fs.GetRect());
}
