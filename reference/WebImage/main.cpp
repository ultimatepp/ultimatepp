#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Image img;
	
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), Gray());
		w.DrawImage(0, 0, img);
	}
	
	MyApp() {
		img = StreamRaster::LoadStringAny(HttpRequest("http://www.ultimatepp.org/1i.png").Execute());
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
