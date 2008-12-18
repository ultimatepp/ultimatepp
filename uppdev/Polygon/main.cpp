#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LDUMP(x)
#define LLOG(x);
#define LTIMING(x) RTIMING(x)

struct PolygonRenderer {
	Buffer< Vector<int> > ln;
	int y0, ymax, lnc;

	void AddPoint(int x, int y)
	{
		LDUMP(Point(x, y));
		if(y >= y0 && y <= ymax) {
			Vector<int>& v = ln[y - y0];
			int i;
			for(i = 0; i < v.GetCount(); i++)
				if(x < v[i])
					break;
			v.Insert(i, x);
		}
	}

	void RenderPolygon(ImageBuffer& b, const Vector<Point>& p, RGBA color)
	{
		LTIMING("RenderPolygon");
		if(p.GetCount() < 2)
			return;
		ymax = y0 = p[0].y;
		for(int i = 0; i < p.GetCount(); i++) {
			int y = p[i].y;
			y0 = min(y0, y);
			ymax = max(ymax, y);
		}
		y0 = minmax(y0, 0, b.GetHeight() - 1);
		ymax = minmax(ymax, 0, b.GetHeight() - 1);
		int lines = ymax - y0 + 1;
		ln.Alloc(lines);
		for(int i = 0; i < p.GetCount(); i++) {
			Point p1 = p[i];
			Point p2 = p[i > 0 ? i - 1 : p.GetCount() - 1];
			LLOG(p1 << " - " << p2);
			if(p1.y > p2.y)
				Swap(p1, p2);
			int delta = ((p2.x - p1.x) << 16) / (p2.y - p1.y + 1);
			int x = p1.x << 16;
			AddPoint(p1.x, p1.y);
			for(int y = p1.y; y < p2.y; y++) {
				x += delta;
				AddPoint(x >> 16, y);
			}
			AddPoint(p2.x, p2.y);
		}
		RGBA *t = b[y0];
		int cx = b.GetWidth();
		for(int i = 0; i < lines; i++) {
			LDUMP(y0 + i);
			Vector<int>& v = ln[i];
			int j = 0;
			while(j < v.GetCount() - 1) {
				LLOG(v[j] << ' ' << v[j + 1]);
				if(v[j] < cx && v[j + 1] >= 0) {
					int x1 = max(v[j], 0);
					int x2 = min(v[j + 1], cx - 1);
					Fill(t + x1, color, x2 - x1 + 1);
				}
				j += 2;
			}
			t += cx;
		}
	}
};

void RenderPolygon(ImageBuffer& b, const Vector<Point>& p, RGBA color)
{
	PolygonRenderer r;
	r.RenderPolygon(b, p, color);
}

struct RGBAi {
	dword r;
	dword g;
	dword b;
	dword a;

	void Get(RGBA& t) {
		t.r = r >> 4;
		t.g = g >> 4;
		t.b = b >> 4;
		t.a = a >> 4;
	}

	void Put(const RGBA& s) {
		r += s.r;
		g += s.g;
		b += s.b;
		a += s.a;
	}
	
	RGBAi() { r = g = b = a = 0; }
};

Image Downscale(const ImageBuffer& b)
{
	LTIMING("Downscale");
	Size tsz = b.GetSize() / 4;
	ImageBuffer tb(tsz);
	int cx = b.GetWidth();
	const RGBA *bs = ~b;
	RGBA *t = tb;
	for(int y = 0; y < tsz.cy; y++) {
		const RGBA *q = bs;
		for(int x = 0; x < tsz.cx; x++) {
			RGBAi m;
			const RGBA *s = q;
			m.Put(s[0]); m.Put(s[1]); m.Put(s[2]); m.Put(s[3]);
			s += cx;
			m.Put(s[0]); m.Put(s[1]); m.Put(s[2]); m.Put(s[3]);
			s += cx;
			m.Put(s[0]); m.Put(s[1]); m.Put(s[2]); m.Put(s[3]);
			s += cx;
			m.Put(s[0]); m.Put(s[1]); m.Put(s[2]); m.Put(s[3]);
			q += 4;
			m.Get(*t++);
		}
		bs += 4 * cx;
	}
	return tb;
}

struct App : TopWindow {
	Vector<Point> p;

	virtual void LeftDown(Point, dword) {
		Size sz = GetSize() * 4;
		int n = Random(10) + 3;
		p.SetCount(n);
		for(int i = 0; i < n; i++)
			p[i] = Point(Random(sz.cx), Random(sz.cy));
//			p[i] = Point(Random(sz.cx + 30) - 15, Random(sz.cy + 30) - 15);
		Refresh();
	}

	void Plot(ImageBuffer& b, int x, int y) {
		if(Rect(b.GetSize()).Contains(Point(x, y)))
			b[y][x] = Yellow();
	}

	virtual void Paint(Draw& w) {
		ImageBuffer b(GetSize() * 4);
		Fill(b, LtGray(), b.GetLength());
		Image m;
		for(int i = 0; i < 10; i++) {
			RenderPolygon(b, p, Red());
			for(int i = 0; i < p.GetCount(); i++) {
				Point q = p[i];
				Plot(b, q.x, q.y);
				Plot(b, q.x - 1, q.y);
				Plot(b, q.x + 1, q.y);
				Plot(b, q.x, q.y + 1);
				Plot(b, q.x, q.y - 1);
			}
			m = Downscale(b);
		}
		w.DrawImage(0, 0, m);
	}
	
	App() {
		p << Point(4 * 20, 4 * 20) << Point(4 * 200, 4 * 20) << Point(4 * 230, 4 * 150) << Point(4 * 120, 4 * 200);
	}
};

GUI_APP_MAIN
{
	App().Run();
}
