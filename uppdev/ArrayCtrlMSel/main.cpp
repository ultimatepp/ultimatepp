#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl a;
	a.AddColumn("Alfa\rbeta");
	for(int i = 0; i < 10; i++) {
		a.Add(i);
	}
	a.MultiSelect().Removing();
	TopWindow app;
	app.Add(a.SizePos());
	app.Run();
}
