#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl list;
	list.AddColumn("Trivial").Ctrls<EditString>(); // specify type of widget as template argument
	list.AddColumn("Factory").With( // class factory creates widget - allows customization of widget
		[](One<Ctrl>& x) {
			x.Create<Option>().NoWantFocus();
		}
	);
	list.AddColumn("Lined").WithLined( // passes the row index as the first argument
		[](int line, One<Ctrl>& x)
		{
			if(line & 1)
				x.Create<EditString>();
			else
				x.Create<Option>().SetLabel("Line " + AsString(line));
		}
	);

	for(int i = 0; i < 300; i++)
		list.Add(AsString(i), i & 1, AsString(i));
	for(int i = 1; i < 300; i += 3)
		list.GetCtrl(i, 0)->Disable();

	list.SetLineCy(Draw::GetStdFontCy() + DPI(8));

	TopWindow app;
	app.Add(list.SizePos());
	app.Sizeable();
	app.Run();
}
