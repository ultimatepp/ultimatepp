#include "Local.h"

#ifdef GUI_TURTLE

NAMESPACE_UPP

struct DrawDragRectInfo {
	Rect  rect1, rect2, clip;
	int   n;
	int   type;
	int   animation;
};

void DrawDragLine(SystemDraw& w, bool horz, int x, int y, int len, int n, int anim)
{
	if(len <= 0)
		return;
	for(int i = 0; i < n; i++) {
		Ctrl::Put8(horz ? Ctrl::HORZDRAGLINE : Ctrl::VERTDRAGLINE);
		Ctrl::Put16(x + !horz * i);
		Ctrl::Put16(y + horz * i);
		Ctrl::Put16(len);
		Ctrl::Put16(anim);
	}
}

void DrawDragFrame(SystemDraw& w, const Rect& r, int n, int animation)
{
	DrawDragLine(w, true, r.left, r.top, r.GetWidth(), n, animation);
	DrawDragLine(w, false, r.left, r.top + n, r.GetHeight() - 2 * n, n, animation);
	DrawDragLine(w, false, r.right - n, r.top + n, r.GetHeight() - 2 * n, n, animation);
	DrawDragLine(w, true, r.left, r.bottom - n, r.GetWidth(), n, animation);
}

void DrawDragRect(Ctrl& q, const DrawDragRectInfo& f)
{
	SystemDraw w;
	w.Init(Ctrl::GetDesktopSize());
	w.Clip(f.clip);
	/* TODO
	static int dashes[3][3] = {
		{ 32, 32, 0 },
		{ 1, 1, 1 },
		{ 5, 1, 2 },
	};
	*/
	DrawDragFrame(w, f.rect1, f.n, f.animation);
	DrawDragFrame(w, f.rect2, f.n, f.animation);
	w.End();
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
		Ctrl::SyncClient();
		Ctrl::Output();
	}
}

void FinishDragRect(Ctrl& q)
{
}

END_UPP_NAMESPACE

#endif
