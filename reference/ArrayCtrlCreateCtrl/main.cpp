#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl list;
	list.Appending().Removing();
	list.AddColumn("Text");
	list.AddColumn("Option");

	for(int i = 0; i < 300; i++) {
		list.Add(AsString(i));
		list.CreateCtrl<EditString>(i, 0);
		list.CreateCtrl<Option>(i, 1, false).SetLabel("Option " + AsString(i));
	}

	list.SetLineCy(Draw::GetStdFontCy() + 8);

	TopWindow app;
	app.Add(list.SizePos());
	app.Sizeable();
	app.Run();
}
