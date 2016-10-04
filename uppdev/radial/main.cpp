#include "radial.h"

RadialDemo::RadialDemo()
{
	Sizeable().Zoomable();
	Title("Radial basis transform demo");
	BackPaint();

	Add(array.VSizePos(0, 30).LeftPos(0, 200));
	Add(recalc.BottomPos(0, 22).LeftPos(0, 80));
	recalc.SetLabel("Calculate");
	recalc <<= THISBACK(Recalc);
	array.AutoHideSb();
	array.Inserting().Removing().Duplicating().NoAskRemove();
	array.AddColumn("Source").SetDisplay(ColorDisplay()).Edit(source);
	array.AddColumn("Target").SetDisplay(ColorDisplay()).Edit(target);
	array.Add(White(), Yellow());
	array.Add(LtRed(), LtBlue());
	array.Add(Black(), LtGreen());
	array.Add(LtMagenta(), White());
	array.Add(Gray(), LtRed());

	Recalc();
}

void RadialDemo::Recalc()
{
	Vector<Color> sc, tc;
	int n = array.GetCount();
	sc.SetCount(n);
	tc.SetCount(n);
	for(int i = 0; i < n; i++) {
		sc[i] = array.Get(i, 0);
		tc[i] = array.Get(i, 1);
	}
	transform = CalcTransform(sc, tc);
	Refresh();
}

void RadialDemo::Paint(Draw& draw)
{
	draw.DrawRect(draw.GetClip(), SWhite());
	PaintSection(draw, Point(220, 10), Point(220, 230), Size(200, 200), LtRed(), LtGreen(), LtCyan());
	PaintSection(draw, Point(440, 10), Point(440, 230), Size(200, 200), Black(), LtGray(), White());
	PaintSection(draw, Point(660, 10), Point(660, 230), Size(200, 200), LtBlue(), LtMagenta(), White());
}

void RadialDemo::PaintSection(Draw& draw, Point src, Point dst, Size sz, Color lt, Color rt, Color rb)
{
	ImageBuffer sbuf(sz), dbuf(sz);
	for(int x = 0; x < sz.cx; x++)
		for(int y = 0; y < sz.cy; y++) {
			int r = lt.GetR() + iscale(rt.GetR() - lt.GetR(), x, sz.cx)
				+ iscale(rb.GetR() - rt.GetR(), y, sz.cy);
			int g = lt.GetG() + iscale(rt.GetG() - lt.GetG(), x, sz.cx)
				+ iscale(rb.GetG() - rt.GetG(), y, sz.cy);
			int b = lt.GetB() + iscale(rt.GetB() - lt.GetB(), x, sz.cx)
				+ iscale(rb.GetB() - rt.GetB(), y, sz.cy);
			Color c(minmax(r, 0, 255), minmax(g, 0, 255), minmax(b, 0, 255));
			sbuf[y][x] = RGBA(c);
			dbuf[y][x] = RGBA(transform.Transform(c));
		}
	draw.DrawImage(src.x, src.y, sbuf);
	draw.DrawImage(dst.x, dst.y, dbuf);
}

Image RadialRecolor(const Image& m)
{
	Vector<Color> src;
	Vector<Color> tgt;
	src.Add(Black); tgt.Add(SColorText);
	src.Add(Gray); tgt.Add(SColorShadow);
	src.Add(LtGray); tgt.Add(SColorFace);
	src.Add(White); tgt.Add(SColorLight);
	src.Add(Yellow); tgt.Add(SColorPaper);
	src.Add(LtBlue); tgt.Add(SColorHighlight);
	RadialBasisTransform rbt = CalcTransform(src, tgt);
	ImageBuffer b(m.GetSize());
	const RGBA *s = m;
	const RGBA *e = s + b.GetLength();
	RGBA *t = b;
	while(s < e) {
		*t = rbt.Transform(*s);
		t->a = s->a;
		t++;
		s++;
	}
	b.SetHotSpot(m.GetHotSpot());
	return b;
}

struct ChApp : TopWindow {
	virtual void Paint(Draw& w)
	{
		static Image m = RadialRecolor(CtrlsImg::OkB());
		ChPaint(w, GetSize(), m);
	}

	ChApp() { Sizeable().Zoomable(); SetRect(0, 0, 50, 20); }
};

GUI_APP_MAIN
{
	ChApp().Run();
//	RadialDemo().Run();
}
