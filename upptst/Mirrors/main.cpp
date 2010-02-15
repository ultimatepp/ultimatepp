#include <CtrlLib/CtrlLib.h>
#include <Report/Report.h>
#include <Painter/Painter.h>

using namespace Upp;

GUI_APP_MAIN
{
	Font fnt = Arial(200);
	Size isz = GetTextSize("M", fnt);
	String letters = "sSzZEeDNB12345679kKrRLCc";
	Report r;
	Size page = r.GetPageSize();
#if 0
//	r.DrawText(0, 0, letters, fnt);
	for(int y = 0; y + isz.cy < page.cy; y += 3 * isz.cy / 2)
		for(int x = 0; x < page.cx; x += 3 * isz.cx / 2) {
			int ch = letters[Random(letters.GetCount())];
			PaintingPainter gw(isz.cx, isz.cy);
			gw.Begin();
			if(Random(4) == 0) {
				gw.Translate(isz.cx, 0);
				gw.Scale(-1, 1);
			}
			gw.Character((isz.cx - fnt.Info()[ch]) / 2, 0, ch, fnt).Fill(Black());
//			gw.Rectangle(0, 0, isz.cx, isz.cy).Stroke(5, LtGray());
			gw.End();
			r.DrawPainting(x, y, isz.cx, isz.cy, gw);
		}
	r.NewPage();
	String pairs = "BNDH";
	for(int y = 0; y + isz.cy < page.cy; y += 3 * isz.cy / 2)
		for(int x = 0; x < page.cx; x += 3 * isz.cx) {
			int ch = pairs[Random(pairs.GetCount())];
			int cl = ToLower(ch);
			if(Random(3) == 0)
				cl = ToLower(pairs[Random(pairs.GetCount())]);
			r.DrawText(x, y, String(ch, 1) + String(cl, 1), fnt);
		}
	r.NewPage();
#endif
#if 0
	for(int i = 0; i < 26; i++) {
		for(int j = 0; j < 3; j++) {
			int a, b, c;
			do {
				a = Random(50) + 1;
				b = Random(50) - 25;
				c = Random(50) - 25;
			}
			while(a + b + c < 1 || a + b + c > 50);
			int q = Random(4);
			String s;
			if(q == 0)
				s = "__";
			else
				s = AsString(a);
			s << (b > 0 ? " + " : " - ");
			if(q == 1)
				s << "__";
			else
				s << AsString(abs(b));
			s << (c > 0 ? " + " : " - ");
			if(q == 2)
				s << "__";
			else
				s << AsString(abs(c));
			s << " = ";
			if(q == 3)
				s << "__";
			else
				s << a + b + c;
			r.DrawText(10 + 1200 * j, 10 + 2 * isz.cy / 2  * i, s, Arial(120));
		}
	}
#endif
	for(int i = 0; i < 50; i++) {
		for(int j = 0; j < 3; j++) {
			int a, b, c;
			a = Random(2) + 5;
			b = Random(10) + 1;
			if(Random(2))
				Swap(a, b);
			String s;
			if(Random(2))
				s << a << " x " << b << " = _____";
			else
				s << a << " x _____ = " << a * b;
			r.DrawText(10 + 1200 * j, 120  * i, s, Arial(100));
		}
	}
	Perform(r);
}
