#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) {
		ImagePainter ip(GetSize());
		ip.DrawRect(GetSize(), White());

		String s = "Абвгдежз";				// some cyrillic text
		ip.DrawText(10, 10, s);				// OK

		String s2 = ToCharset(CHARSET_WIN1251, s);
		ip.DrawText(10, 50, s2, CHARSET_WIN1251);	// draws rectangles instead of letters

		Image im = ip;
		w.DrawImage(0, 0, im);
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
