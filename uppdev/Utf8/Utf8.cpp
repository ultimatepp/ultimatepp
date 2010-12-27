#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct TestWindow : TopWindow {
	WString wtext;
	String text;

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White);
		w.DrawText(10, 10, wtext);
		w.DrawText(10, 50, text);
		w.DrawText(100, 100, "Hello!");
	}
};

GUI_APP_MAIN
{
	WString x = FromUtf8(LoadFile(GetHomeDirFile("aaa")));
	DDUMP(FormatIntHex(x[0]));
	DDUMP(x.GetCount());
	LOGHEXDUMP(~x, 2 * x.GetCount());
	String h = ToUtf8(x);
	LOGHEXDUMP(~h, h.GetCount());
	SaveFile(GetHomeDirFile("aaa.1"), h);
	
	TestWindow w;
	w.text = h;
	w.wtext = x;
	
//	w.Run();
	
	LineEdit le;
	le.Set(x);
	w.Add(le.SizePos());
	w.Run();
}

