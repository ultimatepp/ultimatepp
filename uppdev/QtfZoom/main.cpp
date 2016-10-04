#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	RichTextView rv;
	rv.AutoHideSb();
	rv.SetQTF("[4 memento ipso mores]");
	TopWindow app;
	app.Add(rv.SizePos());
	app.SetRect(100, 100, 400, 200);
	app.Run();
	app.Hide();

	RichTextView rv1;
	rv1.AutoHideSb();
	rv1.SetQTF("[4 memento ipso mores]");
	TopWindow app1;
	app1.Add(rv1.SizePos());
	app1.SetRect(300, 300, 600, 300);
	//app1.SetFrameRect(300, 300, 600, 300);
	app1.Run();
}
