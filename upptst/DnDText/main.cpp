#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	String text;

	void LeftDrag(Point, dword) {
		VectorMap<String, ClipData> data;
		Append(data, String("Hello world!"));
		text = decode(DoDragAndDrop(data, CtrlImg::exclamation()), DND_MOVE, "MOVE",
		                                                           DND_COPY, "COPY",
		                                                           "NONE");
		Refresh();
	}

	void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		w.DrawText(10, 10, text, Arial(50));
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
