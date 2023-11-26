#include "Local.h"

#ifdef VIRTUALGUI

NAMESPACE_UPP

struct DrawDragRectInfo {
	Rect  rect1, rect2, clip;
	int   n;
	int   type;
	int   animation;
};

void DrawDragLine(SystemDraw& w, bool horz, int x, int y, int len, int n, const int *pattern, int animation)
{
	if(len <= 0)
		return;
	if(horz)
		w.Clip(x, y, len, n);
	else
		w.Clip(x, y, n, len);
	
	(horz ? x : y) -= animation;
	len += animation;
	bool ch = false;
	while(len > 0) {
		int segment = pattern[ch];
		int d = segment + pattern[2];
		if(horz) {
			w.DrawRect(x, y, segment, n, InvertColor());
			x += d;
		}
		else {
			w.DrawRect(x, y, n, segment, InvertColor());
			y += d;
		}
		len -= d;
		ch = !ch;
	}
	w.End();
}

void DrawDragFrame(SystemDraw& w, const Rect& r, int n, const int *pattern, int animation)
{
	DrawDragLine(w, true, r.left, r.top, r.GetWidth(), n, pattern, animation);
	DrawDragLine(w, false, r.left, r.top + n, r.GetHeight() - 2 * n, n, pattern, animation);
	DrawDragLine(w, false, r.right - n, r.top + n, r.GetHeight() - 2 * n, n, pattern, animation);
	DrawDragLine(w, true, r.left, r.bottom - n, r.GetWidth(), n, pattern, animation);
}

void DrawDragRect(Ctrl& q, const DrawDragRectInfo& f)
{
	SystemDraw& w = VirtualGuiPtr->BeginDraw();
	Ctrl::PaintScene(w);
	w.Clip(f.clip);
	static int dashes[3][3] = {
		{ 32, 32, 0 },
		{ 1, 1, 1 },
		{ 5, 1, 2 },
	};
	const int *dash = dashes[minmax(f.type, 0, 2)];
	DrawDragFrame(w, f.rect1, f.n, dash, f.animation);
	DrawDragFrame(w, f.rect2, f.n, dash, f.animation);
	w.End();
	Ctrl::PaintCaretCursor(w);
	VirtualGuiPtr->CommitDraw();
}

void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, int type, int animation)
{
	Ctrl *top = q.GetTopCtrl();
	if(top) {
		Point off = q.GetScreenView().TopLeft();
		DrawDragRectInfo f;
		f.rect1 = rect1.Offseted(off);
		f.rect2 = rect2.Offseted(off);
		f.clip = (clip & q.GetSize()).Offseted(off);
		f.n = n;
		f.type = type;
		f.animation = animation;
		DrawDragRect(*top, f);
	}
}

void FinishDragRect(Ctrl& q)
{
	SystemDraw& w = VirtualGuiPtr->BeginDraw();
	Ctrl::PaintScene(w);
	Ctrl::PaintCaretCursor(w);
	VirtualGuiPtr->CommitDraw();
}

END_UPP_NAMESPACE

#endif