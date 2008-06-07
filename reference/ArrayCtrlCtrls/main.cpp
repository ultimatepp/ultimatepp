#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl a;
	a.AddColumn("Text").Ctrls<EditString>();
	for(int i = 0; i < 300; i++)
		a.Add(AsString(i));
	a.SetLineCy(Draw::GetStdFontCy() + 8);

	TopWindow app;
	app.Add(a.SizePos());
	app.Sizeable();
	app.Run();
}
