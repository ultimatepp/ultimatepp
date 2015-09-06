#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl list;
	list.AddColumn();
	list.Add("Simple");
	list.Add("Long " + String('X', 200));
	list.Add(AttrText("High " + String('X', 200)).SetFont(StdFont(StdFont().GetHeight() * 6 / 5)));
	String s = String('0', 200);
	for(int i = 0; i < 9; i++) {
		s << "\n" << String('1' + i, 200);
		list.Add(s);
	}
	String t = String('0', 20000);
	for(int i = 0; i < 9; i++) {
		t << "\n" << String('1' + i, 20000);
		list.Add(t);
	}
	for(int i = 0; i < 200; i++)
		s << "\n" << String('1' + i, 200);
	list.Add("Very tall\n" + s);
	TopWindow win;
	win.Add(list.SizePos());
	win.Run();
}
