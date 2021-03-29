#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	EditDouble q, q1;
	q.AlignRight();
	
	q <<= 1.234;
	
	TopWindow win;
	win.Add(q.LeftPosZ(10, 40).TopPosZ(10));
	win.Add(q1.LeftPosZ(10, 100).TopPosZ(40));
	win.Run();
}
