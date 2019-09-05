#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ImageDraw iw(400, 100);
	iw.DrawRect(0, 0, 400, 100, White());
	iw.DrawEllipse(0, 0, 400, 100, Red());
	iw.DrawText(20, 30, AsString(GetSysTime()));
	ClearClipboard();
	AppendClipboardImage(iw);
}
