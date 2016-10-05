#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class App : public TopWindow {
public:
	virtual void LeftDown(Point, dword);
	virtual void Paint(Draw& w);

private:
	String text;
	bool   cowork;
	int    phase;

	void   SyncTitle();

public:
	typedef App CLASSNAME;

	App& Text(const String& t)     { text = t; Refresh(); return *this; }

	App();
};

App::App()
{
	BackPaint();
	Zoomable().Sizeable();
	cowork = true;
	phase = 0;
	SyncTitle();
}

void App::SyncTitle()
{
	Title(cowork ? "Now using all CPU cores (click the image for single core)"
	             : "Now using single CPU core (click the image to use all cores)");
}

void App::LeftDown(Point, dword)
{
	cowork = !cowork;
	SyncTitle();
}

void DoLine(RGBA *line, int phase, int y, Size sz)
{
	Point c = sz / 2;
	c = Point(int(sin((double)phase / 131) * c.x + c.x), int(sin((double)phase / 127) * c.y + c.y));
	int yy = (y - c.y) * (y - c.y);
	for(int x = 0; x < sz.cx; x++) {
		double d = (x - c.x) * (x - c.x) + yy;
		line[x] = GrayColor((int)(120 + 120 * sin(d / 1000 - (double)phase / 5)));
	}
}

void App::Paint(Draw& w)
{
	Size sz = GetSize();
	ImageBuffer ib(sz);
	if(cowork) {
		CoWork co;
		for(int y = 0; y < sz.cy; y++)
			co & [=, &ib] { DoLine(ib[y], phase, y, sz); };
	}
	else
		for(int y = 0; y < sz.cy; y++)
			DoLine(ib[y], phase, y, sz);
	w.DrawImage(0, 0, ib);
	Refresh();
	phase++;
}

GUI_APP_MAIN
{
	App().Run();
}
