#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	ArrayCtrl a;
	a.HeaderObject().Absolute();
	Array<EditString> es;
	for(int i = 0; i < 30; i++)
		a.AddColumn(FormatIntAlpha(i), 120).Edit(es.Add());
	for(int i = 0; i < 300; i++) {
		Vector<Value> x;
		for(int i = 0; i < 30; i++)
			x.Add(FormatIntAlpha(rand()));
		a.Add(x);
	}
	TopWindow app;
	app.Add(a.HSizePos(10, 10).VSizePos(20, 20));
	app.SetRect(0, 0, 1000, 600);
	app.Open();
	Ctrl::AutoBackPaintAreaSize = 0;
	Ctrl::ProcessEvents();
	app.NoBackPaint();
	{
		TimeStop s;
		for(int i = 0; i < 200; i++) {
			app.Refresh();
			app.Sync();
			Draw::Flush();
		}
		RLOG("Direct paint time: " << s);
	}
    {
		TimeStop s;
		for(int i = 0; i < 200; i++) {
			a.SetCursor(i);
			app.Sync();
			Draw::Flush();
		}
		RLOG("Direct scroll time: " << s);
	}
	a.SetCursor(0);
	app.BackPaint();
	{
		TimeStop s;
		for(int i = 0; i < 200; i++) {
			app.Refresh();
			app.Sync();
			Draw::Flush();
		}
		RLOG("Back paint time: " << s);
	}
    {
		TimeStop s;
		for(int i = 0; i < 200; i++) {
			a.SetCursor(i);
			app.Sync();
			Draw::Flush();
		}
		RLOG("Back scroll time: " << s);
	}
	PromptOK("Finished");
}
