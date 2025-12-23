#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <S3toSHD/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <S3toSHD/test.iml>
#include <Draw/iml_source.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), White());
		for(int i = 0; i < TestImg::COUNT; i++) {
			Image m = TestImg::Get(i);
			DDUMP(m.Get2ndSpot());
			w.DrawImage(10 + i * 128, 10, m);
			w.DrawImage(10 + i * 128, 128, DownSample3x(m));
			w.DrawImage(10 + i * 128, 200, Downscale2x(DownSample3x(m)));
			w.DrawImage(40 + i * 128, 200, DownSample2x(DownSample3x(m)));
			w.DrawImage(70 + i * 128, 200, RescaleFilter(m, m.GetSize() / 6, FILTER_LANCZOS3));

			w.DrawImage(10 + i * 128, 300, Downscale2x(m));
			w.DrawImage(10 + i * 128, 400, Downscale2x(Downscale2x(m)));

			w.DrawImage(10 + i * 128, 500, Rescale(m, 64, 64));
		}
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
