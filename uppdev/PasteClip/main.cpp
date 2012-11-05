#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct DndTest : public TopWindow {
	virtual void Paint(Draw &w);
	virtual void DragAndDrop(Point p, PasteClip& d);

	Vector<String> files;

	DndTest();
};

void DndTest::Paint(Draw &w)
{
	w.DrawRect(GetSize(), SColorPaper());
	if(files.GetCount())
		for(int i = 0; i < files.GetCount(); i++)
			w.DrawText(2, 2 + i * Draw::GetStdFontCy(), files[i]);
	else
		w.DrawText(2, 2, "None");
}

void DndTest::DragAndDrop(Point p, PasteClip& d)
{
	if(d.IsAvailable("files")) {
		files = GetClipFiles(d.Get("files"));
		Refresh();
	}
}

DndTest::DndTest()
{
	Title("I need files!");
}

GUI_APP_MAIN
{
	DndTest().Run();
}
