#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl a;
	a.AddColumn("Alfa\rbeta");
	for(int i = 0; i < 100; i++) {
		a.Add(i);
		if(i % 3 == 0)
			a.Select(i);
	}
	a.Select(70, 10);
	a.MultiSelect();
	TopWindow app;
	app.Add(a.SizePos());
	app.Run();
}
