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
	int x;
	int y;
	
	RGBA Get(int dx, int dy) {
		return GetPixel(img, x + dx, y + dy);
	}
	
	void Do(int dx, int dy) {
		RGBA c = Get(dx, 0);
		if(c.a > 128 && c == Get(0, dy) && (c != Get(dx, dy) || c != Get(-dx, dy) || c != Get(dx, -dy))) {
			r += c.r;
			g += c.g;
			b += c.b;
			a += c.a;
			n++;
		}
	}
	
	RGBA Get() {
		RGBA x;
		n *= 4;
		x.r = r / n;
		x.g = g / n;
		x.b = b / n;
		x.a = a / n;
		return x;
	}
	
	SmoothPixel(const Image& m, int x, int y) : img(m), x(x), y(y) { r = g = b = a = n = 0; }
};

Image Smoothen(const Image& img)
{
	Image m = img;
	ImageBuffer ib(m);
	Size isz = img.GetSize();
	for(int y = 0; y < isz.cy; y++)
		for(int x = 0; x < isz.cx; x++) {
			SmoothPixel p(img, x, y);
			p.Do(-1, -1);
			p.Do(-1, 1);
			p.Do(1, -1);
			p.Do(1, 1);
			RGBA a = img[y][x];
			if(p.n) {
				RGBA b = p.Get();
				AlphaBlend(&a, &b, 1);
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
		const int at = 16 * mg + 16 + 16;
		
		for(int i = 0; i < TestImg().GetCount(); i++) {
			Image m = TestImg().Get(i);
			w.DrawImage(i * at, 0, Magnify(m, mg, mg));
			w.DrawImage(i * at, 16 * mg + 8, Magnify(Smoothen(m), mg, mg));
			w.DrawImage(i * at + at - 20, 0, Smoothen(m));
			w.DrawImage(i * at + at - 20, 20, m);
		}
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
