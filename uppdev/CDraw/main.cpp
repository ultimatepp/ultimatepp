#include "MDraw.h"

struct DrawRectOp {
	void (*op)(PixelBlock& w, int x, int y, int cx, int cy, Color c);
	const char *name;
}
drawrect[] =
{
	DrawRectNOP, "No rect",
	DrawRectBASE, "Base",
	DrawRectF, "MMX",
	DrawRectS, "SMMX",
};

struct DrawTextOp {
	void (*op)(PixelBlock& w, int x, int y, Color c);
	const char *name;
}
drawchar[] =
{
	DrawNOP, "No text",
	DrawBASE, "Base",
	AADrawBASE, "AA-Base",
	DrawMMX1, "AA-MMX",
	DrawAAP, "AA-packed",
	DrawAAPMMX, "AA-packed MMX",
};

void MDraw::Paint(Draw& w)
{
	TimeStop tm;
	Size sz = GetSize();
	String txt('0', 200);
	if(gdi) {
		int ii = posy / 16;
		int y = 16 * ii - posy;
		while(y < sz.cy) {
			w.DrawRect(0, y, sz.cx, 16, (va & 1) ? White : ii & 1 ? Color(10 * ii, 0, 128) : Color(0, 128, 10 * ii));
			w.DrawText(0, y, txt, Arial(13), White);
			ii++;
			y += 16;
		}
	}
	else {
		PixelBlock b(sz.cx, sz.cy, sec);
		int ii = posy / 16;
		int y = 16 * ii - posy;
		if(va & 2)
			(*drawrect[recti].op)(b, 0, 0, sz.cx, sz.cy, White);
		while(y < sz.cy) {
			if(!(va & 2))
				(*drawrect[recti].op)(b, 0, y, sz.cx, 16, (ii & 1) ? Color(10 * ii, 0, 128) : Color(0, 128, 10 * ii));
			int x = 0;
			int qq = 0;
			while(x < sz.cx) {
				if(!(va & 1) || ((qq >> 4) ^ ii) & 1)
					(*drawchar[chari].op)(b, x, y, ((qq ^ ii) & 1) ? Color(10 * ii, 13 * qq, 128) : Color(13 * qq, 255, 10 * ii));
				qq++;
				x += 7;
			}
			ii++;
			y += 16;
		}
		b.Put(w, 0, 0);
	}
	time += tm.Elapsed();
	double tma = (time / ++frames);
	String t;
	t << "| Rect(1):" << drawrect[recti].name;
	t << "| Text(2):" << drawchar[chari].name;
	t << "| GDI(3):" << (gdi ? "*ON*" : "off");
	t << "| Section(4):" << (sec ? "*ON*" : "off");
	t << "| -- result: " << Format("%.2f", tma) << " ms, " << Format("%.2f", 1000.0 / max(tma, 1.0))
	  << " FPS (" << AsString(frames) << " passes)";
	Title(t);
	if(am) {
		posy += 20;
		Refresh();
	}
}

void MDraw::ClearRefresh()
{
	frames = 0;
	time = 0;
	Refresh();
}

bool MDraw::Key(dword key, int)
{
	switch(key) {
	case '0': am = !am; break;
	case '1': recti = (recti + 1) % __countof(drawrect); break;
	case '2': chari = (chari + 1) % __countof(drawchar); break;
	case '3': gdi = !gdi; break;
	case '4': sec = !sec; break;
	case '5': va++; break;
	case K_ENTER:
		va = 0;
		sec = 0;
		am = 0;
		gdi = 0;
		SetRect(30, 30, 800, 600);
		for(va = 0; va < 4; va++) {
			RLOG(((va & 1) ? "Low density" : "High density") << " - " << ((va & 2) ? "Single rect" : "Many rects"));
			for(recti = 0; recti < __countof(drawrect); recti++)
				for(chari = 0; chari < __countof(drawchar); chari++) {
					ClearRefresh();
					for(int i = 0; i < 3; i++) {
						Refresh();
						Sync();
						posy += 10;
					}
					double tma = (time / ++frames);
					RLOG(drawrect[recti].name << "/" << drawchar[chari].name << ": "
					    << Format("%.2f", tma) << " ms, "
					    << Format("%.2f", 1000.0 / max(tma, 1.0)) << " FPS");
				}
			RLOG("-----------------");
		}
		recti = 0;
		chari = 0;
		PromptOK("Test finished, results are in .log file");
		break;
	}
	ClearRefresh();
	return true;
}

void MDraw::MouseMove(Point p, dword)
{
	if(am)
		return;
	posy = p.y;
	Refresh();
}

MDraw::MDraw()
{
	posy = 0;
	am = 0;
	gdi = 0;
	sec = 0;
	va = 0;
	recti = chari = 0;
	Sizeable().Zoomable();
	ClearRefresh();
}

GUI_APP_MAIN
{
	Ctrl::AutoBackPaintAreaSize = 0;
	MDraw t;
//	t.SetRect(0, 0, 1024, 768);
	t.Sizeable().Zoomable();
	t.Run();
}
