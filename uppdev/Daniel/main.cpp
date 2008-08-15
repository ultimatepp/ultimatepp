#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	TopWindow win;
	ArrayCtrl a;
	for(int i = 0; i < 20; i++)
		a.AddColumn("Alfa", 100);
	a.HeaderObject().Absolute();
	for(int i = 0; i < 100; i++)
		a.Add(i);
//	win.Add(a.SizePos());
	TabCtrl tb;
	tb.Add("Aaklsjdf asdfjkasdf");
	tb.Add("Aaklsjdf asdfjkasdf");
	tb.Add("Aaklsjdf asdfjkasdf");
	tb.Add("Aaklsjdf asdfjkasdf");
//	win.Add(tb.SizePos());

	Button x;
	x = ":asdasdfasdf";
	x.SetAlign(ALIGN_CENTER);
	win.Add(x.SizePos());
	win.Sizeable();
	win.Run();

	x.SetAlign(ALIGN_RIGHT);
	win.Run();

}
