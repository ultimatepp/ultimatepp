#include "IconDes.h"

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
	
	force_inline
	RGBA Get(int dx, int dy) {
		return GetPixel(img, x + dx, y + dy);
	}
	
	force_inline
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
	
	force_inline
	RGBA Get(double level) {
		RGBA x;
		n = max(int(n * level), 1);
		x.r = r / n;
		x.g = g / n;
		x.b = b / n;
		x.a = a / n;
		return x;
	}
	
	SmoothPixel(const Image& m, int x, int y) : img(m), x(x), y(y) { r = g = b = a = n = 0; }
};

Image Smoothen(const Image& img, double level)
{
	Image m = img;
	ImageBuffer ib(m);
	Size isz = img.GetSize();
	level = 1 / level;
	for(int y = 0; y < isz.cy; y++)
		for(int x = 0; x < isz.cx; x++) {
			SmoothPixel p(img, x, y);
			p.Do(-1, -1);
			p.Do(-1, 1);
			p.Do(1, -1);
			p.Do(1, 1);
			RGBA a = img[y][x];
			if(p.n) {
				RGBA b = p.Get(level);
				AlphaBlend(&a, &b, 1);
			}
			ib[y][x] = a;
		}
	return Image(ib);
}

void IconDes::Smoothen()
{
	WithImageDblLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Smoothen");
	PlaceDlg(dlg);
	dlg.txt = "Level";
	dlg.level.MinMax(0, 1);
	dlg.level <<= 0.75;
	dlg.level <<= dlg.Breaker();
	Couple(dlg, dlg.level, dlg.slider, 1, 0.75);
	Image bk = ImageStart();
	for(;;) {
		ImageSet(::Smoothen(bk, 0.4 * minmax((double)~dlg.level + 0.01, 0.01, 1.1)));
		switch(dlg.Run()) {
		case IDCANCEL:
			ImageSet(bk);
			return;
		case IDOK:
			return;
		}
	}
}
