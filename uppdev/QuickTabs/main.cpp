#include <CtrlLib/CtrlLib.h>

IdeQuickTabs::IdeQuickTabs()
{
	font = Arial(10);
	fcy = ScreenInfo().GetFontInfo(font).GetHeight();
	hl = -1;
	cursor = -1;
}

void PaintTab(Draw& w, int x, int y, int cx, int cy, Color paper, Color top)
{
	w.DrawRect(x + 1, y + 2, cx - 2, cy - 2, paper);
	w.DrawRect(x + 2, y + 1, cx - 4, 1, top);
	w.DrawRect(x + 2, y, cx - 6, 1, top);
	Color fc = Blend(SColorHighlight, SColorShadow);
	w.DrawRect(x + 0, y + 2, 1, cy - 2, fc);
	w.DrawRect(x + 1, y + 1, 1, 1, fc);
	w.DrawRect(x + 2, y, cx - 4, 1, fc);
	w.DrawRect(x + cx - 2, y + 1, 1, 1, fc);
	w.DrawRect(x + cx - 1, y + 2, 1, cy - 2, fc);
}

void IdeQuickTabs::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorFace);
	int cx = (sz.cx - 6) / 10;
	int x = 0;
	Color hc = Blend(Yellow, LtRed, 100);
	Color inactive = Blend(SColorPaper, SColorFace);
	for(int i = 0; i < 10; i++) {
		if(i != cursor && !IsNull(file[i]))
			PaintTab(w, x + 2, 2, cx - 1, sz.cy - 2, inactive, i == hl ? hc : inactive);
		x += cx;
	}
	w.DrawRect(0, sz.cy - 1, sz.cx, 1, Blend(SColorHighlight, SColorShadow));
	if(cursor >= 0)
		PaintTab(w, cx * cursor, 1, cx + 4, sz.cy - 1, SColorPaper, hc);
	x = 0;
	for(int i = 0; i < 10; i++) {
		if(!IsNull(file[i])) {
			DrawFileName(w, x + DX, DY - (i == cursor), cx - 10, fcy, GetFileName(file[i]),
			             false, font, SColorText, LtBlue, Null, Null, false);
			w.DrawText(x + cx - 6, DY - (i == cursor) - 1, AsString((i + 1) % 10), Arial(9), SColorText);
		}
		x += cx;
	}
}

int IdeQuickTabs::GetPos(int x)
{
	int cx = (GetSize().cx - 6) / 10;
	int i = x / cx;
	return IsNull(file[i]) ? -1 : i;
}

void IdeQuickTabs::LeftDown(Point p, dword)
{
	int c = GetPos(p.x);
	if(c > 0 && c != cursor) {
		cursor = c;
		UpdateActionRefresh();
	}
}

void IdeQuickTabs::MouseMove(Point p, dword)
{
	int c = GetPos(p.x);
	if(c != hl) {
		hl = c;
		Refresh();
	}
}

void IdeQuickTabs::MouseLeave()
{
	hl = -1;
	Refresh();
}

void IdeQuickTabs::CancelMode()
{
	hl = -1;
	Refresh();
}

void IdeQuickTabs::FramePaint(Draw& w, const Rect& rr)
{
	Rect r = rr;
	r.top += fcy + HEIGHT;
	Color c = Blend(SColorHighlight, SColorShadow);
	w.DrawRect(r.left, r.top, 1, r.Height(), c);
	w.DrawRect(r.right - 1, r.top, 1, r.Height(), c);
	w.DrawRect(r.left, r.bottom - 1, r.Width(), 1, c);
	r.top--;
	DrawFrame(w, r.Deflated(1), SColorPaper);
}

void IdeQuickTabs::FrameLayout(Rect& r)
{
	LayoutFrameTop(r, this, fcy + HEIGHT);
	r.top++;
	r.left += 2;
	r.right -= 2;
	r.bottom -= 2;
}

void IdeQuickTabs::FrameAddSize(Size& sz)
{
	sz += 4;
	sz.cy += fcy + HEIGHT;
}

GUI_APP_MAIN
{
	TopWindow win;
	LineEdit edit1, edit2;
	IdeQuickTabs tabs;
	tabs.SetFile(5, "Hello.cpp");
	tabs.SetFile(7, "World.h");
	edit2.SetFrame(tabs);
	Splitter split;
	split.Vert(edit1, edit2);
	win.Add(split);
	win.Sizeable();
	win.Run();
}
