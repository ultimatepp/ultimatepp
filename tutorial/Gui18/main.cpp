#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow  app;
	app.SetRect(0, 0, Zx(200), Zy(20));
	EditDate  date;
	app.Add(date.TopPosZ(0, 20).HSizePos());
	app.Run();
	if(app.Accept())
		PromptOK("Correct date.&[* " + AsString(~date));
}
