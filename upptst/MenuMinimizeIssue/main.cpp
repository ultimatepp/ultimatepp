#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	MenuBar menu;
	TopWindow win;
	win.Zoomable();
	win.AddFrame(menu);
	menu.Set([&](Bar& bar) {
		bar.Sub("Test", [&] (Bar& bar) {
			bar.Add("Test", [&] { Exclamation("Hello!"); });
		});
	});
	win.Run();
}
