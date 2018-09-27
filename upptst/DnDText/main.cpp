#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	String text;
	
	Point  drop;
	String type;

	void LeftDrag(Point, dword) override {
		VectorMap<String, ClipData> data;
		Append(data, String("Hello world!"));
		text = decode(DoDragAndDrop(data, CtrlImg::exclamation()), DND_MOVE, "MOVE",
		                                                           DND_COPY, "COPY",
		                                                           "NONE");
		Refresh();
	}
	
	void DragAndDrop(Point p, PasteClip& d) override
	{
		drop = p;
		type = "?";
		if(d.IsAvailable("text"))
			type = "text";
		if(AcceptText(d))
			text = ~d;
		Refresh();
	}

	void DragLeave() override
	{
		drop = Null;
		Refresh();
	}

	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), White());
		w.DrawText(10, 10, text, Arial(50));
		if(!IsNull(drop)) {
			Size sz = GetTextSize(type, StdFont());
			w.DrawRect(Rect(drop, sz), Blue());
			w.DrawText(drop.x, drop.y, type, StdFont(), Yellow());
		}
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
