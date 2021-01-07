#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow  app;
	app.SetRect(0, 0, Zx(200), Zy(60));
	EditDate  date1, date2, date3;
	date1 <<= date2 <<= date3 <<= GetSysDate();
	date1.ClearModify();
	app.Add(date1.TopPosZ(0, 20).HSizePos());
	date2.Disable();
	app.Add(date2.TopPosZ(20, 20).HSizePos());
	date3.SetReadOnly();
	app.Add(date3.TopPosZ(40, 20).HSizePos());
	app.Run();
	if(date1.IsModified())
		PromptOK("Date was modified!");
}
