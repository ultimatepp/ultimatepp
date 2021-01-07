#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow  app;
	app.SetRect(0, 0, Zx(200), Zy(20));
	EditString text;
	app.Add(text.TopPosZ(0, 20).HSizePos());

	text <<= "Some text - operator version";
	app.Run();
	PromptOK((String)~text);
}
