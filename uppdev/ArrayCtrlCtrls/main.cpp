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
	ArrayCtrl a;
	a.Appending().Removing();
	a.AddColumn("Text").Ctrls(WidgetFactory);
	a.AddColumn("Text");
	for(int i = 0; i < 10000; i++)
		a.Add(AsString(i));
//	for(int i = 0; i < 300; i += 3)
//		a.GetCtrl(i, 0)->Disable();

	a.SetLineCy(Draw::GetStdFontCy() + 8);

	TopWindow app;
	app.Add(a.SizePos());
	app.Sizeable();
	app.Run();
}
