#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	EditString text, text2;
	String h;
	for(int i = 0; i < 20000; i++)
		h << FormatIntRoman(i);
	DUMP(h.GetCount());
	text <<= h;
	text2 <<= h;
	win.Add(text.HSizePos().TopPos(0));
	win.Add(text2.Password().HSizePos().TopPos(50));
	win.Execute();
}
