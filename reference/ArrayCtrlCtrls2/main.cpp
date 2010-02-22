#include <CtrlLib/CtrlLib.h>

using namespace Upp;

void WidgetFactory(int line, One<Ctrl>& x)
{
	if(line & 1)
		x.Create<EditString>();
	else
		x.Create<Option>().SetLabel("Line " + AsString(line));
}

GUI_APP_MAIN
{
	ArrayCtrl list;
	list.Appending().Removing();
	list.AddColumn("Text").Ctrls(WidgetFactory);

	for(int i = 0; i < 300; i++)
		list.Add(AsString(i));
	for(int i = 1; i < 300; i += 3)
		list.GetCtrl(i, 0)->Disable();

	list.SetLineCy(Draw::GetStdFontCy() + 8);

	TopWindow app;
	app.Add(list.SizePos());
	app.Sizeable();
	app.Run();
}
