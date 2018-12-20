#include <CtrlLib/CtrlLib.h>

using namespace Upp;

Color MakeDark0(Color c)
{
	int n = Grayscale(c);
	if(n < 70)
		return Color(min(c.GetR() + 128, 255), min(c.GetG() + 128, 255), min(c.GetB() + 128, 255));
	if(n > 190)
		return Color(max(c.GetR() - 128, 0), max(c.GetG() - 128, 0), max(c.GetB() - 128, 0));
	return c;
}

Color MakeDark(Color c)
{
	double h, s, v;
	const double m = 1/255.0;
	RGBtoHSV(c.GetR() * m, c.GetG() * m, c.GetB() * m, h, s, v);
	DDUMP(c);
	DDUMP(h);
	DDUMP(s);
	DDUMP(v);
	if(s > v)
		return HsvColorf(h, 1 - s, v);
	else
		return HsvColorf(h, s, 1 - v);
}

struct ColorTest : TopWindow {
	ColorSelector color;
	
	void Paint(Draw& w) override {
		Size sz = GetSize();
		w.DrawRect(0, 0, sz.cx, sz.cy / 2, ~color);
		w.DrawRect(0, sz.cy / 2, sz.cx, sz.cy / 2 + 1, MakeDark(~color));
	}
	
	ColorTest() {
		Add(color.LeftPosZ(0, 300).TopPosZ(0, 300));
		color << [=] { Refresh(); };
	}
};

GUI_APP_MAIN
{
	ColorTest().Run();
}
