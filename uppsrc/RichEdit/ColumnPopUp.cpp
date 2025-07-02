#include "RichEdit.h"

namespace Upp {

ColumnPopUp::ColumnPopUp()
{
	SetFrame(BlackFrame());
}

void ColumnPopUp::Paint(Draw& w)
{
	int ii = 0;
	int y = 0;
	while(ii < count || ii == 0) {
		for(int x = 0; x < columns; x++) {
			w.Clipoff(x * isz.cx, y * isz.cy, isz.cx, isz.cy);
			w.DrawRect(isz, ii == cursor ? SColorHighlight() : SColorPaper());
			if(ii < count)
				WhenPaintItem(w, isz, ii, ii == cursor);
			w.End();
			ii++;
		}
		y++;
	}
}

void ColumnPopUp::MouseMove(Point p, dword keyflags)
{
	int ii = p.x / isz.cx + p.y / isz.cy * columns;
	if(ii < 0 || ii >= count)
		ii = -1;
	if(ii != cursor) {
		cursor = ii;
		Refresh();
	}
}

void ColumnPopUp::LeftUp(Point, dword keyflags)
{
	if(cursor >= 0)
		WhenSelect(cursor);
	Close();
}

void ColumnPopUp::MouseLeave()
{
	cursor = -1;
	Refresh();
}

void ColumnPopUp::PopUp(Point p, Ctrl *owner)
{
	if(IsOpen())
		Close();
	Size sz = AddFrameSize(columns * isz.cx, (count + columns - 1) / columns * isz.cy);
	Rect r = owner ? owner->GetWorkArea() : GetWorkArea();
	if(p.x + sz.cx > r.right)
		p.x = max(r.right - sz.cx, 0);
	if(p.y + sz.cy > r.bottom)
		p.y = max(r.bottom - sz.cy, 0);
	SetRect(p.x, p.y, sz.cx, sz.cy);
	Ctrl::PopUp(owner);
}

int ColumnPopUp::Execute()
{
	PopUp(GetMousePos(), GetActiveCtrl());
	EventLoop(this);
	if(IsOpen())
		Close();
	return cursor;
}

void ColumnPopUp::Deactivate()
{
	Close();
}

struct MyApp : TopWindow {
	ColumnPopUp dl;

	void LeftDown(Point p, dword keyflags) override {
		dl.PopUp(GetScreenView().TopLeft() + p, this);
	}

	
	MyApp() {
		dl.WhenPaintItem = [=](Draw& w, Size sz, int ii, bool sel) {
			w.DrawText(0, 0, AsString(ii), StdFont(), sel ? SColorHighlightText() : SColorText());
		};
		dl.WhenSelect = [=](int i) { PromptOK(AsString(i)); };
//		Add(dl.SizePos());
	}
};

};