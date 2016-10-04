#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <Iml2/test.iml>
#include <Draw/iml.h>

struct MyApp : TopWindow {
	void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		w.DrawImage(100, 100, TestImg::Test());
		w.DrawImage(110, 115, TestImg::Test());
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
