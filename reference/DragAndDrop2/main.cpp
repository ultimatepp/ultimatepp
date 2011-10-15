#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppData {
	String text;
	Color  color;
};

struct MyApp : TopWindow {
	Point     pos;
	MyAppData data;
	Point     dragpos;
	
	virtual void CancelMode();
	virtual void Paint(Draw& w);
	virtual void DragAndDrop(Point p, PasteClip& clip);
	virtual void DragLeave();
	virtual void LeftDrag(Point p, dword keyflags);
	virtual String GetSelectionData(const String& fmt) const;

	MyApp();
};

void MyApp::CancelMode()
{
	dragpos = Null;
	Refresh();
}

void MyApp::DragAndDrop(Point p, PasteClip& clip)
{
	if(clip.Accept("MyAppData")) {
		String bin = clip;
		if(bin.GetLength() > sizeof(Color)) { // prudent check
			pos = p;
			memcpy(&data.color, ~bin, sizeof(Color));
			data.text = bin.Mid(sizeof(Color));
		}
		Refresh();
	}
	if(AcceptText(clip)) {
		pos = p;
		data.text = GetString(clip);
	}
	dragpos = clip.IsAccepted() ? p : Null;
	Refresh();
}

void MyApp::DragLeave()
{
	dragpos = Null;
	Refresh();
}

String MyApp::GetSelectionData(const String& fmt) const
{
	if(fmt == "MyAppData") {
		String bin;
		bin.Cat((byte *)&data.color, sizeof(data.color));
		bin.Cat(data.text);
		return bin;
	}
	return GetTextClip(data.text, fmt);
}

void MyApp::LeftDrag(Point p, dword keyflags)
{
	Size sz(128, 64);
	ImageDraw iw(sz);
	iw.DrawRect(sz, Black());
	iw.Alpha().DrawRect(sz, Black());
	iw.Alpha().DrawText(0, 0, data.text, Courier(14), White());
	DoDragAndDrop(String("MyAppData;") + ClipFmtsText(), iw);
}

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SColorPaper());
	w.DrawText(pos.x, pos.y, data.text, StdFont(), data.color);
	if(!IsNull(dragpos))
		w.DrawRect(RectC(dragpos.x - 1, dragpos.y - 1, 3, 3), LtBlue);
}

MyApp::MyApp()
{
	data.text = FormatIntRoman(Random(2000));
	data.color = Color(Random() & 127, Random() & 127, Random() & 127);
	pos = Point(10, 10);
	SetRect(0, 0, 500, 100);
	dragpos = Null;
}

GUI_APP_MAIN
{
	MyApp().Run();
}
