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
	for(int i = 0; i < 10; i++) {
		int a, b;
		do {
			a = Random(35) + 1;
			b = Random(35) + 1;
		}
		while(a + b < 0 || a + b > 20);
		r.DrawText(10, 10 + isz.cy * 2 * i, Format("%d + %d = ", a, b), fnt);
		do {
			a = Random(35) + 1;
			b = Random(35) + 1;
		}
		while(a - b < 0);
		r.DrawText(2000, 10 + isz.cy * 2 * i, Format("%d - %d = ", a, b), fnt);
	}
	Perform(r);
}
