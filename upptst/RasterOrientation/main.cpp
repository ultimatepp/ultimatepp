#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <RasterOrientation/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <RasterOrientation/test.iml>
#include <Draw/iml_source.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		
		for(int i = 0; i < TestImg::GetCount(); i++) {
			w.DrawImage(i * 128 + 10, 10, Magnify(TestImg::Get(i), 12, 12));
			w.DrawImage(i * 128 + 10, 160, FlipImage(Magnify(TestImg::Get(i), 12, 12), i));
		}
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
