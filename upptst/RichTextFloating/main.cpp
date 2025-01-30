#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	RichEditWithToolBar edit;
	edit.Floating();
	RichEditWithToolBar edit2;
	edit2.Floating();
	win << edit.HSizePos().VSizePos(0, Zx(200));
	win << edit2.HSizePos().BottomPos(0, Zx(200));
	win.Sizeable();
	String fn = ConfigFile("test.qtf");
	edit.SetQTF(LoadFile(fn));
	win.Run();
	SaveFile(fn, edit.GetQTF());
}
