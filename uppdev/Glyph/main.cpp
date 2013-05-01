#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

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



struct MyApp : TopWindow {
	Point p;
	
	virtual void Paint(Draw& w);
	virtual void MouseMove(Point p, dword keyflags);

};

void MyApp::MouseMove(Point p_, dword keyflags)
{
	p = p_;
	Refresh();
}

void DrawLine(Draw& w, Point p1, Point p2)
{
	if(p1.y == p2.y) {
		return;
	}
	if(p2.y < p1.y)
		Swap(p1, p2);
	
	int dy = p2.y - p1.y;
	int dx = abs(p2.x - p1.x);
	int dir = sgn(p2.x - p1.x);

	int dda = dy / 2;
	int x = p1.x;
	
	for(int y = p1.y; y < p2.y; y++) {
		w.DrawRect(x, y, 1, 1, LtBlue());
		dda -= dx;
		while(dy > 0 && dda < 0) {
			dda += dy;
			x += dir;
		}
	}
}

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White());
	DWORD gdiCount = GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS); 
	w.DrawText(400, 0, AsString(gdiCount));

	w.DrawImage(0, 0, RenderGlyph(Arial(20), 'e'));

	String g = CompressGlyph(AutoCrop(RenderGlyph(Arial(20), 'e'), RGBAZero()));
	w.DrawImage(20, 0, DecompressGlyph(g, White()));
	w.DrawImage(0, 50, DecompressGlyph(g, Black()));

	Point p0(300, 300);

	w.DrawLine(p0, p, 0, LtGray());
	
	DrawLine(w, p0, p);
	
	__BREAK__;
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
			Size sz = AutoCrop(RenderGlyph(Tahoma(n), *s), RGBAZero()).GetSize();
			RDUMP(sz);
			RDUMP(sz.cx * sz.cy);
			RDUMP(CompressGlyph(AutoCrop(RenderGlyph(Tahoma(n), *s), RGBAZero())).GetLength());
		}
	MyApp().Run();
}
