#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <UHD/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <UHD/test.iml>
#include <Draw/iml_source.h>

using namespace Upp;

struct MyApp : TopWindow {
	void DrawIcon(Draw& w, int y, const Image& img);
	void Paint(Draw& w);
};

Image SHC(const Image& img)
{
	struct SFilter : ImageFilter9 {
		virtual RGBA operator()(const RGBA **mx) {
			RGBA s = mx[1][1];
			int l = mx[0][1].a;
			int r = mx[2][1].a;
			int t = mx[1][0].a;
			int b = mx[1][2].a;
			int l1 = 128;
			int l2 = 230;
			return l * r * t * b || s.a > l1 || mx[0][1].a > l2 || mx[2][1].a > l2 || mx[1][0].a > l2 || mx[1][2].a > l2 ? s
			       : RGBAZero();
		}
	} ef;
	return Filter(img, ef);
}

Image STR(const Image& img)
{
	struct SFilter : ImageFilter9 {
		virtual RGBA operator()(const RGBA **mx) {
			RGBA s = mx[1][1];
			if(s.a < 128)
				return RGBAZero();
			s.a = 255;
			return s;
		}
	} ef;
	return Premultiply(Filter(Unmultiply(img), ef));
}

void MyApp::DrawIcon(Draw& w, int y, const Image& img)
{
	Size isz = 2 * img.GetSize();
	int x = 10;
	int cx = 150;
	w.DrawImage(x, y + 8, img);
	auto Do = [&](const Image& m, const char *txt) {
		x += cx;
		if(IsNull(img))
			w.DrawText(x, y, txt);
		else
			w.DrawImage(x, y, m);
	};
	Do(Magnify(img, 2, 2), "Magnify");
	Do(Magnify(STR(img), 2, 2), "STR");
	Do(Upscale2x(img), "Upscale2x");
	Do(SHC(Upscale2x(img)), "Upscale2x+");
	Do(SHC(SHC(Upscale2x(img))), "Upscale2x++");
	Do(RescaleFilter(img, isz, FILTER_LANCZOS3), "Lanczos3");
	Do(SHC(RescaleFilter(img, isz, FILTER_LANCZOS3)), "Lanczos3+");
	Do(RescaleFilter(img, isz, FILTER_BILINEAR), "Bilinear");
	Do(RescaleFilter(img, isz, FILTER_BICUBIC_CATMULLROM), "Bicubic");
}

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SColorFace());
	int y = 0;
	int cy = 40;
	DrawIcon(w, y += cy, Null);
	for(int i = 0; i < TestImg::GetCount(); i++)
		DrawIcon(w, y += cy, TestImg::Get(i));
}

GUI_APP_MAIN
{
	MyApp().Run();
}
