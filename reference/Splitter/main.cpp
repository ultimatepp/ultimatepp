#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow
{
	Button a, b, c, d;
	Splitter v, h;

	void Toggle();

	App();
};

void App::Toggle()
{
	h.Zoom(h.GetZoom() == 1 ? -1 : 1);
}

App::App()
{
	a.SetLabel("A");
	b.SetLabel("B");
	c.SetLabel("C");
	d.SetLabel("Toggle!");
	h.Horz(a, v);
	v << b << c << d;
	v.Vert();
	Add(h.SizePos());
	Sizeable().Zoomable();
	d <<= callback(this, &App::Toggle);
}


GUI_APP_MAIN
{
	App().Run();
}
