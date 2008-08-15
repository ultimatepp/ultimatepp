#include "EditTime.h"

EditTimeDlg::EditTimeDlg()
{
	a <<= GetSysDate();
	CtrlLayoutOK(*this, "Window title");
}

GUI_APP_MAIN
{
	DUMP(GetSysTime());
	DUMP(GetSysTime() + 60);
	DUMP(GetSysTime() + 60);
	DUMP(GetSysTime() + 6000);
	DUMP(GetSysTime() + 24 * 3600);
	DUMP(GetSysTime() + 24 * 3600 * 100);
	DUMP((GetSysTime() + 24 * 3600 * 100) - GetSysTime());
	DUMP((GetSysTime() + 24 * 3600 * 100 + 20) - GetSysTime());
	EditTimeDlg().Run();
}
