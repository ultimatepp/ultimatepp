#include <CtrlLib/CtrlLib.h>

using namespace Upp;


#define IMAGECLASS TestImg
#define IMAGEFILE <Smoothen/test.iml>
#include <Draw/iml_header.h>
#define IMAGECLASS TestImg
#define IMAGEFILE <Smoothen/test.iml>
#include <Draw/iml_source.h>

const int mg = 6;

RGBA GetPixel(const Image& img, int x, int y)
{
	if(x < 0 || x >= img.GetWidth() || y < 0 || y >= img.GetHeight())
		return RGBAZero();
	return img[y][x];
}

struct SmoothPixel {
	const Image& img;
	int r;
	int g;
	int b;
	int a;
	int n;
	
	void Do(int x1, int y1, int x2, int y2) {
		RGBA c = GetPixel(img, x1, y1);
		if(c.a > 128 && c == GetPixel(img, x2, y2)) {
			r += c.r;
			g += c.g;
			b += c.b;
			a += c.a;
			n++;
		}
	}
	
	RGBA Get() {
		RGBA x;
		x.r = r / n;
		x.g = g / n;
		x.b = b / n;
		x.a = a / n;
		return x;
	}
	
	SmoothPixel(const Image& m) : img(m) { r = g = b = a = n = 0; }
};

Image Smoothen(const Image& img)
{
	Image m = img;
	ImageBuffer ib(m);
	Size isz = img.GetSize();
	for(int y = 0; y < isz.cy; y++)
		for(int x = 0; x < isz.cx; x++) {
			SmoothPixel p(img);
			p.Do(x - 1, y, x, y - 1);
			p.Do(x - 1, y, x, y + 1);
			p.Do(x + 1, y, x, y - 1);
			p.Do(x + 1, y, x, y + 1);
			RGBA a = ib[y][x];
			if(p.n) {
				RGBA b = p.Get();
				a.r = (2 * a.r + b.r) / 3;
				a.g = (2 * a.g + b.g) / 3;
				a.b = (2 * a.b + b.b) / 3;
				a.a = (2 * a.a + b.a) / 3;
			}
			ib[y][x] = a;
		}
	return ib;
}

struct MyApp : TopWindow {
	Size msz;
	virtual void Paint(Draw& w) {
		Size sz =  GetSize();
		w.DrawRect(GetSize(), Gray());
		const int at = 64 * mg + 20;
		
		w.DrawImage(5, 200, Magnify(TestImg::test(), mg, mg));
		w.DrawImage(5, 0, Magnify(Smoothen(TestImg::test()), mg, mg));
		w.DrawImage(100, 0, Smoothen(TestImg::test()));
		w.DrawImage(100, 20, TestImg::test());

		w.DrawImage(205, 200, Magnify(TestImg::test2(), mg, mg));
		w.DrawImage(205, 0, Magnify(Smoothen(TestImg::test2()), mg, mg));
		w.DrawImage(300, 0, Smoothen(TestImg::test2()));
		w.DrawImage(300, 20, TestImg::test2());
	
		w.DrawImage(305, 200, Magnify(TestImg::test3(), mg, mg));
		w.DrawImage(305, 0, Magnify(Smoothen(TestImg::test3()), mg, mg));
		w.DrawImage(400, 0, Smoothen(TestImg::test3()));
		w.DrawImage(400, 20, TestImg::test3());
	}
	
	virtual void MouseMove(Point p, dword keyflags)
	{
		msz = p / mg;
		Refresh();
	}
	
	
	MyApp() {
		msz = Size(0, 0);
		Maximize();
	}
};

GUI_APP_MAIN
{
//	RescaleFilter(RescaleImg::test(), 32, 32, FILTER_BILINEAR);
	MyApp().Run();
}
