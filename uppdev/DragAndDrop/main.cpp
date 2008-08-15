#include "dnd.h"

/*
void Test::LeftDown(Point p, dword keyflags)
{
}
*/

struct LineEditDnd : LineEdit {
///	virtual void   DragEnter(Point p, const ClipData& d, bool drop) { LOG("Enter"); }
//	virtual void   DragLeave() { LOG("Leave"); }
//	virtual void   DragAndDrop(Point p, DnDEvent& d);
	virtual void   LeftHold(Point p, dword flags);
	virtual void   LeftDrag(Point p, dword flags);
	virtual void   LeftTriple(Point p, dword flags);
	virtual String GetClip(const char *fmt, int kind);
};

/*
void LineEditDnd::DragAndDrop(Point p, DnDEvent& d)
{
	if(!d.Has("text")) return;
	d.Accept();
	if(d.IsDrop())
		Paste(String(~d.Get("text")).ToWString());
}
*/
void LineEditDnd::LeftHold(Point p, dword flags)
{
//	PromptOK("Hold!");
}

void LineEditDnd::LeftTriple(Point p, dword flags)
{
	PromptOK("Left triple-click");
}

void LineEditDnd::LeftDrag(Point p, dword f)
{
	String txt = GetData();
	ImageDraw iw(128, 128);
	iw.Alpha().DrawRect(0, 0, 128, 128, Black);
	for(int y = 0; y < 128; y += 140) {
		iw.Alpha().DrawText(2, y + 2, txt, Courier(11), White);
		iw.DrawText(2, y + 2, txt, Courier(11));
	}
//	DnDAction(this, "text").Sample(iw).Do();
	LineEdit::LeftDown(p, f);
}

String LineEditDnd::GetClip(const char *fmt, int kind)
{
	return GetData();
}

GUI_APP_MAIN
{
	DUMP(GUI_DragDistance());
	DUMP(GUI_DblClickTime());
	Ctrl::LogMessages = true;
//	Ctrl::OverrideCursor(CtrlImg::HandCursor());
	TopWindow win;
	LineEditDnd dnd;
	win.Add(dnd.SizePos());
	win.Run();
}
