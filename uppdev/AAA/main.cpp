#include <CtrlLib/CtrlLib.h>

#include <vector> 

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <AAA/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <AAA/test.iml>
#include <Draw/iml_source.h>

struct Test : TopWindow {
	virtual void Paint(Draw& w);
	
	EditIntSpin passes, amount;
	
	void DoRefresh() { Refresh(); }

	void PutImage(Draw& w, int& y, const Image& m)
	{
		w.DrawImage(20, y, m);
		y += m.GetSize().cy + 10;
	}
	
	typedef Test CLASSNAME;
	
	Test();
};

int sqr(int x)
{
	return x * x;
}

RGBA Blend(RGBA a, RGBA b)
{
	RGBA r;
	r.a = ((int)a.a + b.a) >> 1;
	r.r = ((int)a.r + b.r) >> 1;
	r.g = ((int)a.g + b.g) >> 1;
	r.b = ((int)a.b + b.b) >> 1;
	return r;
}

RGBA Get(const Image& img, int x, int y)
{
	return Rect(img.GetSize()).Contains(x, y) ? img[y][x] : RGBAZero();
}

bool IsSimilar(RGBA a, RGBA b, int max)
{
	return sqr(77 * ((int)a.r - b.r)) + sqr(151 * ((int)a.g - b.g)) + sqr(28 * ((int)a.b - b.b))
	       <= sqr(max);
}

int GetColorIndex(Vector<RGBA>& colors, Color c, int max_distance)
{
	for(int i = 0; i < colors.GetCount(); i++)
		if(IsSimilar(colors[i], c, max_distance))
			return i;
	colors.Add(c);
	return colors.GetCount() - 1;
}

struct Colors : Vector<RGBA> {
	const Image& m;
	int    x, y;
	
	int  DX(int r) {
		r &= 7;
		return findarg(r, 0, 7, 6) >= 0 ? -1 : findarg(r, 2, 3, 4) >= 0 ? 1 : 0;
	}
	
	int DY(int r) {
		r &= 7;
		return findarg(r, 0, 1, 2) >= 0 ? -1 : findarg(r, 4, 5, 6) >= 0 ? 1 : 0;
	}
	
	RGBA Get(int r) {
		return ::Get(m, x + DX(r), y + DY(r));
	}

	int GetCI(Color c)
	{
		return GetColorIndex(*this, c, 500);
	}

	int  GetCI(int r) {
		return GetCI(Get(r));
	}
	
	Colors(const Image& m, int x, int y) : m(m), x(x), y(y) {}
};

Image Upscale(const Image& simg)
{
	Size isz = simg.GetSize();

	ImageBuffer ib(2 * isz);
	
	Fill(~ib, RGBAZero(), ib.GetLength());

	for(int x = 0; x < isz.cx; x++)
		for(int y = 0; y < isz.cy; y++) {
			DLOG("---------- " << x << ", " << y);
			RGBA c = simg[y][x];
			int p[16];
			// 0 1 2
			// 7 c 3
			// 6 5 4
			Colors colors(simg, x, y);
			for(int i = 0; i < 8; i++) {
				p[i] = p[i + 8] = colors.GetCI(i);
				DLOG(i << ": " << p[i]);
			}
			int ci = colors.GetCI(c);
			DDUMP(colors);
			// 0 1
			// 3 2
			int *pp = p;
			for(int r = 0; r < 4; r++) {
				RGBA& t = ib[2 * y + (r >= 2)][2 * x + (r == 1 || r == 2)];
				t = c;
				if(pp[1] == pp[7]
				   && ci != pp[0]
				   && (pp[0] != pp[1] || pp[0] != pp[6] || pp[0] != pp[2])
//				   && (pp[7] != pp[3] && pp[7] != pp[4])
//				   && (pp[1] != pp[5] && pp[1] != pp[4])
				) {
				    DLOG(r);
					DLOG("x: " << (r == 1 || r == 2));
					DLOG("y: " << (r >= 2));
					t = Blend(colors.Get(2 * r + 1), colors.Get(2 * r + 7));
				}
				pp += 2;
			}
		}
	
	return ib;
}

void Test::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SColorFace());
	Image simg = TestImg::test1();
	int y = 10;
	PutImage(w, y, simg);

	Size isz = simg.GetSize();
	w.DrawImage(10, 150 + 12 * isz.cy, Magnify(simg, 8, 8));
	
	simg = Unmultiply(simg);
	

	Image r = Upscale(simg);
	w.DrawImage(10, 100, r);

	w.DrawImage(10, 150 + 2 * isz.cy, Magnify(r, 4, 4));
	
#if 0
	for(int i = FILTER_NEAREST; i <= FILTER_LANCZOS5; i++) {
		Image img = RescaleFilter(simg, 2 * simg.GetSize(), i);
		w.DrawImage(10, y, img);
		img = Unmultiply(img);
		for(int j = 0; j < (int)~passes; j++)
			img = Sharpen(img, ~amount);
		w.DrawImage(300, y, Premultiply(img));
		w.DrawText(600, y + 10, AsString(i));
		
		y += img.GetSize().cy + 20;
	}
#endif
}

Test::Test()
{
	amount <<= 50;
	passes <<= 2;
	Add(amount.BottomPos(2, 20).LeftPos(2, 50));
	Add(passes.BottomPos(2, 20).LeftPos(60, 50));

//	amount <<= THISBACK(Amount);
//	passes <<= THISBACK(Passes);
	Zoomable().Sizeable();
}

using namespace std;

GUI_APP_MAIN
{
	Test().Run();
}
