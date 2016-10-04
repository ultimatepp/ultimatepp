#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	WithDropChoice<EditString> edit;
	
	win.Add(edit.TopPos(0, 20).LeftPos(0, 100));
	edit.AddList("Hello there!");
	edit.DropWidth(500);
	
	win.Run();
	
	StringStream ss;
	ss % edit;
	String s = ss;
	LOGHEXDUMP(s, s.GetCount());
}
