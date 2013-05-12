#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>
#include "glyph.h"

using namespace Upp;

#define IMAGECLASS GlyphImg
#define IMAGEFILE <Glyph/glyph.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS GlyphImg
#define IMAGEFILE <Glyph/glyph.iml>
#include <Draw/iml_source.h>

struct ColorRenderer : MiniRenderer {
	Draw *draw;
	Color color;

	virtual void PutHorz(int x, int y, int cx) {
		draw->DrawRect(x, y, cx, 1, color);
	}
	virtual void PutVert(int x, int y, int cy) {
		draw->DrawRect(x, y, 1, cy, color);
	}
};

struct MyApp : TopWindow {
	Point p;
	int   width;
	
	virtual void Paint(Draw& w);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);
	
	MyApp() { width = 2; p = Point(0, 0); }
};

Image MyApp::CursorImage(Point p, dword keyflags)
{
	return GlyphImg::cursor1();
}

void MyApp::MouseMove(Point p_, dword keyflags)
{
	p = p_;
	Refresh();
}

void MyApp::MouseWheel(Point p, int zdelta, dword keyflags)
{
	width = minmax(1, 1000, width + sgn(zdelta));
	Title(AsString(width));
	Refresh();
}

void MyApp::Paint(Draw& w)
{
	Size sz = GetSize();

	w.DrawRect(GetSize(), Gray());
/*
//	DWORD gdiCount = GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS); 
//	w.DrawText(400, 0, AsString(gdiCount));

	w.DrawImage(0, 0, RenderGlyph(Arial(20), 'e'));
*/
	String g = CompressGlyph(AutoCrop(RenderGlyph(Arial(20), 'e'), RGBAZero()));
	w.DrawImage(20, 0, DecompressGlyph(g, White()));
	w.DrawImage(0, 50, DecompressGlyph(g, Black()));

	Point p0(300, 300);

	Vector< Vector<Point> > pgs;
	
	ColorRenderer r;
	r.Cy(sz.cy);

	r.draw = &w;
	r.color = Green();
#if 1
	DLOG("------------");
//	p = p0 + Size(16, 16);
	DrawFrame(w, Rect(p0, p), LtRed());
	r.Polygon();
	r.Ellipse(Rect(p0, p));
//	r.Ellipse(p0, p - p0 - width, -1);
	r.Fill();
	return;

	if(0) {
		Point center = p0;
		Size radius = p - p0;
		int n = max(abs(radius.cx), abs(radius.cy));
		for(int i = 0; i < n; i++) {
			Point p = center + Point(int(sin(M_2PI * i / n) * radius.cx), int(cos(M_2PI * i / n) * radius.cy));
			w.DrawRect(p.x, p.y, 1, 1, White());
		}
	}

	r.color = Green();
#endif

	if(0) {
		Sizef r = Size(abs(p.x - p0.x), abs(p.y - p0.y));
		double rr = r.cx * r.cx;
		for(int y = 0; y <= r.cy; y++) {
			double y2 = r.cx * y / r.cy;
			int x = (int)(sqrt(rr - (double)y2 * y2) + 0.5);
			w.DrawRect(p0.x - x, p0.y + y, 2 * x, 1, Black());
			w.DrawRect(p0.x - x, p0.y - y, 2 * x, 1, Black());
		}
	}

	r.color = Black();
	for(int i = 0; i < 1; i++) {
		RTIMING("RenderTriangle");
		r.Polygon();
		r.Move(p0);
		r.Line(p);
		r.Line(Point(700, 400));
		r.Fill();
	}
	
	w.DrawRect(p.x, p.y, 1, 1, White());
//	w.DrawRect(700, 400, 1, 1, White());
	
/*
	for(int i = 0; i < 0; i++) {
		r.color = Color(Color(Random(256), Random(256), Random(256)));
		r.Move(Point(Random(sz.cx), Random(sz.cy)));
		r.Line(Point(Random(sz.cx), Random(sz.cy)));
		r.Line(Point(Random(sz.cx), Random(sz.cy)));
		r.Fill();
	}
*/
	r.color = LtRed();
	r.Width(width);
	r.Move(p0);
	r.Line(p);
	
	r.FatLine(p0, p, width);

	w.DrawRect(p0.x, p0.y, 1, 1, White());
	w.DrawRect(700, 400, 1, 1, White());

//	w.DrawRect(p0.x - 1, p0.y - 1, 3, 3, LtGray());

	r.color = LtBlue();
//	r.Line(p0, p);

//	DrawLine(w, p0, p);

//	__BREAK__;
}

GUI_APP_MAIN
{
	#if 0
	{
		for(int i = 0; i < 10000; i++) {
			RTIMING("RenderGlyph");
			RenderGlyph(Arial(20), 'e');
		}
		String g = CompressGlyph(AutoCrop(RenderGlyph(Arial(20), 'e'), RGBAZero()));
		RDUMPHEX(g);
		for(int i = 0; i < 10000; i++) {
			RTIMING("DecompressGlyph");
			DecompressGlyph(g, White());
		}
	}
	for(const char *s = "compres"; *s; s++)
		for(int n = 10; n <= 100; n += 10) {
			RLOG("-------------");
			RLOG(n << ' ' << *s);
			Size sz = AutoCrop(RenderGlyph(StdFont(n), *s), RGBAZero()).GetSize();
			RDUMP(sz);
			RDUMP(sz.cx * sz.cy);
			RDUMP(CompressGlyph(AutoCrop(RenderGlyph(StdFont(n), *s), RGBAZero())).GetLength());
		}
	#endif
	MyApp().Run();
}
