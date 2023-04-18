#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w);
};

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White());
	w.DrawText(10, 300, "key (⌘G) (⇧G) (⌥G)", StdFont(30).Italic());
	w.DrawText(10, 330, "key (⌘G) (⇧G) (⌥G)", Arial(30).Italic());
	w.DrawText(10, 360, "key (⌘G) (⇧G) (⌥G)", Roman(30).Italic());
	for(int i = 0; i < 4; i++)
		w.DrawText(10, i * 30, "key (⌘G) (⇧G) (⌥G)", StdFont(30).Bold(i & 1).Italic(i & 2));
}

GUI_APP_MAIN
{
	MyApp().Run();
}
