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
	r.DrawText(0, 0, letters, fnt);
	for(int y = isz.cy * 2; y + isz.cy < page.cy; y += isz.cy * 2)
		for(int x = 0; x < page.cx; x += isz.cx * 2) {
			int ch = letters[rand() % letters.GetCount()];
			PaintingPainter gw(isz.cx, isz.cy);
			gw.Begin();
			if((rand() & 3) == 0) {
				gw.Translate(isz.cx, 0);
				gw.Scale(-1, 1);
			}
			gw.Character((isz.cx - fnt.Info()[ch]) / 2, 0, ch, fnt).Fill(Black());
//			gw.Rectangle(0, 0, isz.cx, isz.cy).Stroke(5, LtGray());
			gw.End();
			r.DrawPainting(x, y, isz.cx, isz.cy, gw);
		}
	Perform(r);
}

