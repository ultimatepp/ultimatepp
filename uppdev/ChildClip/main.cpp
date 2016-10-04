#include "ChildClip.h"



ChildClip::ChildClip()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	TopWindow app;
	DocEdit edit;
	app.Add(edit.HSizePos(100, 100).VSizePos(100, 100));
	Button b;
	edit.Add(b.TopPos(-20, 50).LeftPos(20, 50));
	app.NoBackPaint();
	app.Sizeable().Run();
}
