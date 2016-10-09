#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <RescaleFilter/image.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <RescaleFilter/image.iml>
#include <Draw/iml_source.h>

struct MyApp : TopWindow {
	DropList method;
	
	void Paint(Draw& w) {
		w.DrawRect(GetSize(), LtGray());
	
		int m = ~method;

		TimeStop tm;
		
		for(int i = 0; i < TestImg().GetCount(); i++) {
			Image img = TestImg().Get(i);
			int x = 260 * i;
			w.DrawImage(x, 0, img);
			w.DrawImage(x, 200, RescaleFilter(img, 84, 84, m));
			w.DrawImage(x + 94, 200, RescaleFilter(img, 42, 42, m));
			w.DrawImage(x + 94 + 52, 200, RescaleFilter(img, 21, 21, m));
			w.DrawImage(x + 94 + 52 + 31, 200, RescaleFilter(img, 10, 10, m));
			w.DrawImage(x + 94 + 52 + 31 + 20, 200, RescaleFilter(img, 5, 5, m));
			w.DrawImage(x + 94 + 52 + 31 + 20 + 15, 200, RescaleFilter(img, 2, 2, m));
			w.DrawImage(x + 94 + 52 + 31 + 20 + 15 + 12, 200, RescaleFilter(img, 1, 1, m));
			w.DrawImage(x, 300, RescaleFilter(img, 250, 250, m));
			w.DrawImage(x, 600, RescaleFilter(img, Size(250, 250), RectC(40, 40, 100, 100), m));
		}
		
		w.DrawText(GetSize().cx - 200, 40, String().Cat() << "Elapsed " << tm << "s");
	}

	MyApp() {
		SetRect(0, 0, 600, 800);
		Sizeable();
		
		method.Add(FILTER_NEAREST, "Nearest");
		method.Add(FILTER_BILINEAR, "Bilinear");
		method.Add(FILTER_BSPLINE, "Bspline");
		method.Add(FILTER_COSTELLO, "Costello");
		method.Add(FILTER_BICUBIC_MITCHELL, "Bicubic Mitchell");
		method.Add(FILTER_BICUBIC_CATMULLROM, "Bicubic Catmull Rom");
		method.Add(FILTER_LANCZOS2, "Lanczos 2");
		method.Add(FILTER_LANCZOS3, "Lanczos 3");
		method.Add(FILTER_LANCZOS4, "Lanczos 4");
		method.Add(FILTER_LANCZOS5, "Lanczos 5");
		Add(method.TopPos(0, STDSIZE).RightPos(0, 200));
		method <<= FILTER_NEAREST;;
		method << [=] { Refresh(); };

		Zoomable();
		Maximize();
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
