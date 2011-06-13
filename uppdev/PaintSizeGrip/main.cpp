#include <CtrlLib/CtrlLib.h>
#define IMAGECLASS TestImg
#define IMAGEFILE <PaintSizeGrip/test.iml>
#include <Draw/iml_header.h>
#define IMAGECLASS TestImg
#define IMAGEFILE <PaintSizeGrip/test.iml>
#include <Draw/iml_source.h>

using namespace Upp;

struct MyApp : TopWindow {
	StatusBar status;
	
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), Gray());
		w.DrawImage(100, 100, CtrlsImg::SizeGrip());
		w.DrawImage(130, 100, TestImg::test());
	}
	
	MyApp() {
		AddFrame(status);
		Sizeable();
	}
};

GUI_APP_MAIN
{
	MyApp app;
	app.Run();
}

