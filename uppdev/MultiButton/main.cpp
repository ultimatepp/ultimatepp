#include "CtrlLib/CtrlLib.h"

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;

	MultiButton b2;
	win.Add(b2.LeftPos(10, 400).TopPos(50, 100));
	b2.AddButton().Width(100).SetMonoImage(CtrlsImg::DA());
	b2.AddButton().Width(100).SetImage(CtrlImg::smallleft()).Left();
	b2.AddButton().Width(100).SetMonoImage(CtrlImg::smallleft());
	b2.Set("Hello!");

	ScrollBar sb;
	sb.Horz();
	sb.Set(0, 100, 1000);
	win.Add(sb.HSizePos().BottomPos(0, 100));

	ScrollBar sb2;
	sb2.Vert();
	sb2.Set(0, 100, 1000);
	win.Add(sb2.RightPos(0, 100).VSizePos(0, 100));

	Button b1;
	win.Add(b1.EdgeStyle().LeftPos(400, 100).TopPos(200, 100));

	Button bb;
	win.Add(bb.ScrollStyle().LeftPos(400, 100).TopPos(310, 100));

	win.Run();
}
