#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	TopWindow app;
	Button b;
	app << b.LeftPos(10, 12).TopPos(10, 22);
//	b.Disable();
//	b.ScrollStyle();
	b.Ok();
	app.Run();
	return;
	Option o;
	app.Add(o.SizePos());
	o = Null;
	app.Run();
	o.ThreeState();
	app.Run();
}
