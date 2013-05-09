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

bool IsUniform(const RGBA *s, RGBA c, int add, int n)
{
	while(n-- > 0) {
		if(*s != c)
			return false;
		s += add;
	}
	return true;
}

Image AutoCrop(const Image& m, RGBA c)
{
	Size isz = m.GetSize();
	Rect r = isz;
	for(r.top = 0; r.top < isz.cy && IsUniform(m[r.top], c, 1, isz.cx); r.top++)
		;
	for(r.bottom = isz.cy - 1; r.bottom >= r.top && IsUniform(m[r.bottom], c, 1, isz.cx); r.bottom--)
		;
	if(r.bottom <= r.top)
		return Null;
	int h = r.GetHeight();
	const RGBA *p = m[r.top];
	for(r.left = 0; r.left < isz.cy && IsUniform(p + r.left, c, isz.cx, h); r.left++)
		;
	for(r.right = isz.cx; r.right >= r.left && IsUniform(p + r.right, c, isz.cx, h); r.right--)
		;
	r.right++;
	r.bottom++;
	return WithHotSpot(Crop(m, r), 20, 20);
}

Image RenderGlyph(Font fnt, int chr)
{
	int cx = fnt[chr];
	int cy = fnt.GetLineHeight();
	ImageBuffer ib(2 * cx, 2 * cy);
	BufferPainter sw(ib, MODE_ANTIALIASED);
	sw.Clear(RGBAZero());
	sw.DrawText(cx / 2, cy / 2, WString(chr, 1), fnt, Black());
	ib.SetHotSpot(Point(20, 20));
	return ib;
}

String CompressGlyph(const Image& m)
{
	const RGBA *s = m;
	const RGBA *e = s + m.GetLength();
	StringStream r;
	Size sz = m.GetSize();
	r / sz.cx / sz.cy;
	Point p = m.GetHotSpot();
	r / p.x / p.y;
	while(s < e) {
		const RGBA *b = s;
		if(s->a == 0 || s->a == 255) {
			byte a = s->a;
			s++;
			while(s < e && s - b < 63 && s->a == a)
				s++;
			if(a == 0 && s >= e)
				return r;
			r.Put((a & 0x40) | (s - b));
		}
		else {
			r.Put((s->a >> 1) | 0x80);
			s++;
		}
	}
	return r;
}

Image DecompressGlyph(const String& g, Color c)
{
	StringStream r(g);
	Size sz;
	r / sz.cx / sz.cy;
	ImageBuffer ib(sz);
	Point p;
	ib.SetHotSpot(p);
	r / p.x / p.y;
	RGBA *t = ib;
	RGBA *te = ib.End();
	RGBA full = c;
	while(!r.IsEof()) {
		byte b = r.Get();
		ASSERT(t < te);
		if(b & 0x80)
			*t++ = (((b & 0x7f) << 1) | ((b & 0x40) >> 6)) * c;
		else {
			int n = b & 63;
			ASSERT(t + n <= te);
			Fill(t, b & 0x40 ? full : RGBAZero(), n);
			t += n;
		}
	}
	Fill(t, RGBAZero(), te - t);
	return ib;
}

struct ColorRenderer : MiniRenderer {
	Draw *draw;
	Color color;

	virtual void PutHorz(int x, int y, int cx) {
		draw->DrawRect(x, y, cx, 1, color);
	}
	virtual void PutVert(int x, int y, int cy) {
		draw->DrawRect(x, y, 1, cy, color);
	}
	
	ColorRenderer(int cy) : MiniRenderer(cy) {}
};

struct MyApp : TopWindow {
	Point p;
	
	virtual void Paint(Draw& w);
	virtual void MouseMove(Point p, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);
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


void DrawLine(Draw& w, Point p1, Point p2)
{
	DLOG("----------------");
	int dirx = sgn(p2.x - p1.x);
	int diry = sgn(p2.y - p1.y);
	int dx = abs(p2.x - p1.x);
	int dy = abs(p2.y - p1.y);
	int x = p1.x;
	int y = p1.y;
	int x0 = x;
	int y0 = y;
	if(dx < dy) {
		int dda = dy >> 1;
		int n = dy;
		for(;;) {
			if(x != x0) {
				if(y0 < y)
					w.DrawRect(x0, y0, 1, y - y0, LtBlue());
				else
					w.DrawRect(x0, y + 1, 1, y0 - y, LtBlue());
				x0 = x;
				y0 = y;
			}
			if(n-- <= 0)
				break;
			y += diry;
			dda -= dx;
			if(dda < 0) {
				dda += dy;
				x += dirx;
			}
		}
		if(y0 < y)
			w.DrawRect(x0, y0, 1, y - y0, LtBlue());
		else
			w.DrawRect(x0, y + 1, 1, y0 - y, LtBlue());
	}
	else {
		int dda = dx >> 1;
		int n = dx;
		for(;;) {
			if(y != y0) {
				if(x0 < x)
					w.DrawRect(x0, y0, x - x0, 1, LtBlue());
				else
					w.DrawRect(x + 1, y0, x0 - x, 1, LtBlue());
				x0 = x;
				y0 = y;
			}
			if(n-- <= 0)
				break;
			x += dirx;
			dda -= dy;
			if(dda < 0) {
				dda += dx;
				y += diry;
			}
		}
		if(x0 < x)
			w.DrawRect(x0, y0, x - x0, 1, LtBlue());
		else
			w.DrawRect(x + 1, y0, x0 - x, 1, LtBlue());
	}
}


void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), Gray());
//	DWORD gdiCount = GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS); 
//	w.DrawText(400, 0, AsString(gdiCount));

	w.DrawImage(0, 0, RenderGlyph(Arial(20), 'e'));

	String g = CompressGlyph(AutoCrop(RenderGlyph(Arial(20), 'e'), RGBAZero()));
	w.DrawImage(20, 0, DecompressGlyph(g, White()));
	w.DrawImage(0, 50, DecompressGlyph(g, Black()));

	Point p0(300, 300);

	Vector< Vector<Point> > pgs;
	
	ColorRenderer r(GetSize().cy);

	r.draw = &w;
	r.color = Black();
#if 0	
	DLOG("########## Ellipse");
	r.Ellipse(p0, p - p0/*Size(100, 100)*/);
	r.Render();

	{
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
	r.Move(p0);
	r.Line(p);
	r.Line(Point(700, 400));
	r.Render();
	
	w.DrawRect(p.x, p.y, 1, 1, White());
	w.DrawRect(700, 400, 1, 1, White());
	

//	w.DrawLine(p0, p, 0, LtGray());
	w.DrawRect(p0.x - 1, p0.y - 1, 3, 3, LtGray());

	r.color = LtBlue();
//	r.Line(p0, p);

//	DrawLine(w, p0, p);

//	__BREAK__;
}

GUI_APP_MAIN
{
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
	MyApp().Run();
}
