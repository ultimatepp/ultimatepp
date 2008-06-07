#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : public TopWindow {
	Image img;

	virtual void Paint(Draw& w);
	virtual void LeftDown(Point p, dword keyflags);

	typedef MyApp CLASSNAME;
	MyApp();
};

MyApp::MyApp()
{
	ImageBuffer ib(50, 50);
	for(int y = 0; y < 50; y++) {
		RGBA *l = ib[y];
		for(int x = 0; x < 50; x++) {
			if(y == 0 || y == 49 || x == 0 || x == 49)
				*l++ = Black();
			else {
				l->a = 2 * (x + y);
				l->r = 4 * x;
				l->g = 4 * y;
				l->b = 200;
				l++;
			}
		}
	}
	Premultiply(ib);
	img = ib;
}

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White);
	w.DrawImage(10, 5, img);
	w.DrawImage(40, 25, img);
}

void MyApp::LeftDown(Point p, dword keyflags)
{
	ImageBuffer ib(img);
	for(int y = 15; y < 35; y++) {
		RGBA *l = ib[y];
		for(int x = 15; x < 35; x++)
			l[x] = 100 * Red();
	}
	img = ib;
	Refresh();
}

GUI_APP_MAIN
{
	MyApp().Sizeable().Run();
}
