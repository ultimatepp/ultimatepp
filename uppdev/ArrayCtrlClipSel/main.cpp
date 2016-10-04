#include <CtrlLib/CtrlLib.h>
#include <Report/Report.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl a;
	a.AddColumn("First");
	a.AddColumn("Second");
	a.AddColumn("Third");
	for(int i = 0; i < 100; i++) {
		a.Add(i, GetSysDate() + i, FormatIntRoman(i));
		if(i % 3 == 0)
			a.Select(i);
	}
	a.MultiSelect();
	a.SetClipboard(true, false);
	
	TopWindow win;
	win.Add(a.SizePos());
	win.Run();
	
	QtfReport(a.AsQtf(true) + "&&" + a.AsQtf(false, false));
	SaveFile("u:/ac1.csv", a.AsCsv());
	SaveFile("u:/ac2.csv", a.AsCsv(true, ',', false));
}
