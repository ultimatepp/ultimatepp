#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl a;
	a.AddColumn("Font");
	a.AddColumn("View");
	for(int i = 0; i < Font::GetFaceCount(); i++)
		a.Add(Font::GetFaceName(i), AttrText("Just a test!").SetFont(Font(i, 20)));
	a.SetLineCy(24);
	TopWindow app;
	app.Add(a.SizePos());
	app.Run();
}
