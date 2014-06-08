#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	String path;
	
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), SColorPaper());
		w.DrawText(10, 10, path);
	}

	virtual void DragAndDrop(Point p, PasteClip& d);

};

GUI_APP_MAIN
{
}
