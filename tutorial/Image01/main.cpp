#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS MyImages
#define IMAGEFILE  <Image01/images.iml>
#include <Draw/iml.h>

struct MyApp : TopWindow {
	MyApp() {
		const auto isz = MyImages::MyImage().GetSize();
		SetRect(0, 0, 100 + isz.cx, 100 + isz.cy);
	}
	
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), SColorFace());
		w.DrawImage(50, 50, MyImages::MyImage());
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
