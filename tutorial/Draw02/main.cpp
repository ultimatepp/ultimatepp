#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void DoPainting(Draw& w) {
		w.DrawEllipse(0, 0, 100, 30, WhiteGray(), 1, Cyan);
		w.DrawText(0, 0, "Hello world", Roman(30).Bold());
	}
	
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), White());
		DoPainting(w);
		w.Offset(30, 50);
		DoPainting(w);
		w.End();
		w.Offset(20, 100);
		w.Clip(5, 5, 40, 20);
		DoPainting(w);
		w.End();
		w.End();
		w.Clipoff(10, 150, 60, 20);
		DoPainting(w);
		w.End();
	}
};

GUI_APP_MAIN
{
	MyApp app;
	app.Title("Offsets and Cliping").Zoomable().Sizeable();
	app.SetRect(0, 0, 420, 240);
	app.Run();
}
