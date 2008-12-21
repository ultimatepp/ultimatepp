#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LDUMP(x)
#define LLOG(x);
#define LTIMING(x) RTIMING(x)

class AABuffer {
	struct Pixel {
		RGBA  simple;
		RGBA *complex;
	};

	Buffer<Pixel> pixel;
	Size          size;
	int           length;

	void Free();

public:
	void Create(Size sz);
	int  GetLength();
	Pixel *Line(int i)         { return pixel + size.cx * i; }
	void Polygon(const Vector<Point>& p, RGBA color);
	
	Image Result();
	
	AABuffer()                 { size.cx = size.cy = length = 0; }
	~AABuffer()                { Free(); }
};

void AABuffer::Free()
{
	for(int i = 0; i < length; i++)
		if(pixel[i].complex)
			delete pixel[i].complex;
}

void AABuffer::Create(Size sz)
{
	Free();
	size = sz;
	length = sz.cx * sz.cy;
	pixel.Alloc(length);
	memset(pixel, 0, sizeof(Pixel) * length);
}

struct PolygonOutline : Vector< Vector<int> > {
	void AddPoint(int x, int y)
	{
		LDUMP(Point(x, y));
		if(y >= 0 && y < GetCount()) {
			Vector<int>& v = (*this)[y];
			int i;
			for(i = 0; i < v.GetCount(); i++)
				if(x < v[i])
					break;
			v.Insert(i, x);
		}
	}

	void RenderPolygon(int cy, const Vector<Point>& p)
	{
		LTIMING("RenderPolygon");
		if(p.GetCount() < 2)
			return;
		SetCount(cy);
		for(int i = 0; i < p.GetCount(); i++) {
			Point p1 = p[i];
			Point p2 = p[i > 0 ? i - 1 : p.GetCount() - 1];
			LLOG(p1 << " - " << p2);
			if(p1.y > p2.y)
				UPP::Swap(p1, p2);
			int delta = ((p2.x - p1.x) << 16) / (p2.y - p1.y + 1);
			int x = p1.x << 16;
			AddPoint(p1.x, p1.y);
			for(int y = p1.y; y < p2.y; y++) {
				x += delta;
				AddPoint(x >> 16, y);
			}
			AddPoint(p2.x, p2.y);
		}
	}
};


void AABuffer::Polygon(const Vector<Point>& p, RGBA color)
{
	PolygonOutline ln;
	ln.RenderPolygon(size.cy * 4, p);
	for(int y = 0; y < ln.GetCount(); y += 4) {
		int xl = 0;
		int xh = size.cx;
		for(int i = 0; i < 4; i++) {
			Vector<int>& v = ln[y + i];
			if(v.GetCount() >= 2) {
				xl = max(xl, max(v[0] >> 2, 0));
				xh = min(xh, min(v[1] >> 2, size.cx));
			}
			else {
				xl = INT_MAX;
				xh = 0;
			}
		}
		Pixel *l = Line(y >> 2);
		for(int i = xl; i < xh; i++) {
			if(l[i].complex)
				delete[] l[i].complex;
			l[i].simple = color;
		}
		for(int i = 0; i < 4; i++) {
			Pixel *l = Line((y >> 2) + i);
			Vector<int>& v = ln[y + i];
			if(v.GetCount() >= 2) {
				int x = v[0];
				Pixel *p = l + (x >> 2);
				while(x < v[1]) {
					int n = min(3 - (x & 3) + 1, 4);
					if(!p->complex)
						p->complex = new RGBA[16];
					Fill(p->complex + 4 * i, color, n);
					x += n;
					p++;
				}
			}
		}
	}
}

struct RGBAi {
	dword r;
	dword g;
	dword b;
	dword a;

	void Write(RGBA& t) {
		t.r = byte(r >> 4);
		t.g = byte(g >> 4);
		t.b = byte(b >> 4);
		t.a = byte(a >> 4);
	}

	void Put(const RGBA& s) {
		r += s.r;
		g += s.g;
		b += s.b;
		a += s.a;
	}
	
	RGBAi() { r = g = b = a = 0; }
};

Image AABuffer::Result()
{
	LTIMING("Downscale");
	ImageBuffer tb(size);
	const Pixel *s = pixel;
	RGBA *t = tb;
	for(int y = 0; y < size.cy; y++) {
		for(int x = 0; x < size.cx; x++) {
			if(s->complex) {
				RGBAi m;
				const RGBA *c = s->complex;
				m.Put(c[0]); m.Put(c[1]); m.Put(c[2]); m.Put(c[3]);
				m.Put(c[4]); m.Put(c[5]); m.Put(c[6]); m.Put(c[7]);
				m.Put(c[8]); m.Put(c[9]); m.Put(c[10]); m.Put(c[11]);
				m.Put(c[12]); m.Put(c[13]); m.Put(c[14]); m.Put(c[15]);
				m.Write(*t++);
			}
			else
				*t++ = s->simple;
			s++;
		}
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
		w.DrawRect(GetSize(), LtGray);
		AABuffer b;
		b.Create(GetSize());
		for(int i = 0; i < 1; i++) {
			b.Polygon(p, Red());
/*			for(int i = 0; i < p.GetCount(); i++) {
				Point q = p[i];
				Plot(b, q.x, q.y);
				Plot(b, q.x - 1, q.y);
				Plot(b, q.x + 1, q.y);
				Plot(b, q.x, q.y + 1);
				Plot(b, q.x, q.y - 1);
			}*/
//			m = b.Result();
		}
		w.DrawImage(0, 0, b.Result());
	}
	
	App() {
		p << Point(4 * 20, 4 * 20) << Point(4 * 200, 4 * 20) << Point(4 * 230, 4 * 150)/* << Point(4 * 120, 4 * 200)*/;
	}
};

GUI_APP_MAIN
{
	App().Run();
}
