#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct Dbg : Ctrl {
	Vector<String>     regname;
	Vector<Label *>    reglbl;

	EditString         watchedit;
	DropList           frame;
	TabCtrl            tab;
	ArrayCtrl          locals;
	ArrayCtrl          watches;
	ArrayCtrl          autos;
	Label              dlock;

	typedef Dbg CLASSNAME;

	Dbg();
};

Dbg::Dbg()
{
	locals.NoHeader();
	locals.AddColumn("", 1);
	locals.AddColumn("", 6);
	watches.NoHeader();
	watches.AddColumn("", 1).Edit(watchedit);
	watches.AddColumn("", 6);
	watches.Inserting().Removing();
	autos.NoHeader();
	autos.AddColumn("", 1);
	autos.AddColumn("", 6);
	Add(tab.SizePos());
	tab.Add(watches.SizePos(), "Watches");
	tab.Add(locals.SizePos(), "Locals");
	tab.Add(autos.SizePos(), "Autos");
//	Add(frame.HSizePos(200, 0).TopPos(2, EditField::GetStdHeight()));
	frame.Ctrl::Add(dlock.SizePos());
	dlock = "  Running..";
	dlock.SetFrame(BlackFrame());
	dlock.SetInk(Red);
	dlock.NoTransparent();
	dlock.Hide();

//	BackPaint();

//	log.Open(ConfigFile("debug.log"));
}

GUI_APP_MAIN
{
	TopWindow app;
	Dbg widget;
	app.Add(widget.SizePos());
	app.Run();
}
