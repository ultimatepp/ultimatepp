#include "CtrlCore.h"

NAMESPACE_UPP

#define LLOG(x) // DLOG(x)

void LocalLoop::Run()
{
	ASSERT(master);
	master->AddChild(this);
	Ptr<Ctrl> focus = GetFocusCtrl();
	SetCapture();
	SetFocus();
	LLOG("LocalLoop::Run");
	EventLoop(this);
	LLOG("LocalLoop Finished");
	Remove();
	if(focus)
		focus->SetFocus();
}

void LocalLoop::CancelMode()
{
	EndLoop();
}

void RectTracker::LeftUp(Point, dword)
{
	EndLoop();
}

void RectTracker::RightUp(Point, dword)
{
	EndLoop();
}

Image RectTracker::CursorImage(Point, dword)
{
	if(animation)
		DrawRect(rect, rect);
	return cursorimage;
}

RectTracker::RectTracker(Ctrl& master)
{
	SetMaster(master);
	Clip(Rect(0, 0, 100000, 100000));
	width = 1;
	minsize = Size(0, 0);
	maxsize = Size(100000, 100000);
	maxrect = Rect(-100000, -100000, 100000, 100000);
	keepratio = false;
	cursorimage = Image::Arrow();
	color = SColorPaper;
	pattern = DRAWDRAGRECT_NORMAL;
	animation = 0;
	rounder = NULL;
}

RectTracker::~RectTracker() {}

void RectTracker::DrawRect(Rect r1, Rect r2)
{
	if(ty < 0) {
		r1.left = r1.right - 1;
		r2.left = r2.right - 1;
	}
	if(tx < 0) {
		r1.top = r1.bottom - 1;
		r2.top = r2.bottom - 1;
	}
	Rect c = clip & GetMaster().GetSize();
	if(animation) {
		int nanim = (GetTickCount() / animation) % 8;
		DrawDragRect(GetMaster(), Rect(0, 0, 0, 0), r2, c, width, color, pattern, nanim);
		DrawDragRect(GetMaster(), r1, Rect(0, 0, 0, 0), c, width, color, pattern, panim);
		panim = nanim;
	}
	else
		DrawDragRect(GetMaster(), r1, r2, c, width, color, pattern, 0);
}

Rect RectTracker::Track(const Rect& r, int _tx, int _ty)
{
	rect = r;
	tx = _tx;
	ty = _ty;
	org = rect;
	o = rect;
	op = GetMousePos();
	GetMaster().Sync();
	DrawRect(Rect(0, 0, 0, 0), rect);
	Run();
	DrawRect(o, Rect(0, 0, 0, 0));
	FinishDragRect(GetMaster());
	return rect;
}

int RectTracker::TrackHorzLine(int x0, int y0, int cx, int line)
{
	return Track(RectC(x0, y0, cx, line + 1), -1, ALIGN_BOTTOM).bottom - 1;
}

int RectTracker::TrackVertLine(int x0, int y0, int cy, int line)
{
	return Track(RectC(x0, y0, line + 1, cy), ALIGN_RIGHT, -1).right - 1;
}

Rect RectTracker::Round(const Rect& r)
{
	Rect h = r;
	if(round)
		round(h);
	return rounder ? rounder->Round(h) : h;
}

void RectTracker::MouseMove(Point, dword)
{
	Point p = GetMousePos();
	rect = org;
	if(tx == ALIGN_CENTER && ty == ALIGN_CENTER) {
		int x = org.left - op.x + p.x;
		int y = org.top - op.y + p.y;
		if(x + org.Width() > maxrect.right)
			x = maxrect.right - org.Width();
		if(x < maxrect.left)
			x = maxrect.left;
		if(y + org.Height() > maxrect.bottom)
			y = maxrect.bottom - org.Height();
		if(y < maxrect.top)
			y = maxrect.top;
		rect = RectC(x, y, org.Width(), org.Height());
	}
	else {
		if(tx == ALIGN_LEFT) {
			rect.left = max(org.left - op.x + p.x, maxrect.left);
			rect.left = minmax(rect.left, rect.right - maxsize.cx, rect.right - minsize.cx);
		}
		if(tx == ALIGN_RIGHT) {
			rect.right = min(org.right - op.x + p.x, maxrect.right);
			rect.right = minmax(rect.right, rect.left + minsize.cx, rect.left + maxsize.cx);
		}
		if(ty == ALIGN_TOP) {
			rect.top = max(org.top - op.y + p.y, maxrect.top);
			rect.top = minmax(rect.top, rect.bottom - maxsize.cy, rect.bottom - minsize.cy);
		}
		if(ty == ALIGN_BOTTOM) {
			rect.bottom = min(org.bottom - op.y + p.y, maxrect.bottom);
			rect.bottom = minmax(rect.bottom, rect.top + minsize.cy, rect.top + maxsize.cy);
		}
		if(tx == ALIGN_NULL) {
			rect.right = min(org.right - op.x + p.x, maxrect.right);
			if (rect.right < rect.left) {
				Swap(rect.right, rect.left);
				rect.InflateHorz(1);
			}	
		}
		if(ty == ALIGN_NULL) {
			rect.bottom = min(org.bottom - op.y + p.y, maxrect.bottom);
			if (rect.bottom < rect.top) {
				Swap(rect.bottom, rect.top);
				rect.InflateVert(1);
			}
		}
		if(keepratio) {
			int cy = org.Width() ? rect.Width() * org.Height() / org.Width() : 0;
			int cx = org.Height() ? rect.Height() * org.Width() / org.Height() : 0;
			if(tx == ALIGN_BOTTOM && ty == ALIGN_RIGHT) {
				Size sz = rect.Size();
				if(cx > sz.cx)
					rect.right = rect.left + cx;
				else
					rect.bottom = rect.top + cy;
			}
			else
			if(tx == ALIGN_RIGHT)
				rect.bottom = rect.top + cy;
			else
			if(ty == ALIGN_BOTTOM)
				rect.right = rect.left + cx;
		}
	}
	if(rect != o) {
		rect = Round(rect);
		if(rect != o) {
			DrawRect(o, rect);
			sync(rect);
			o = rect;
		}
	}
}

class PointLooper : public LocalLoop {
	const Vector<Image>& ani;
	int ani_ms;
	bool result;

public:
	virtual void  LeftUp(Point, dword);
	virtual Image CursorImage(Point p, dword keyflags);
	virtual bool  Key(dword key, int);

	operator bool() const     { return result; }

	PointLooper(Ctrl& ctrl, const Vector<Image>& ani, int ani_ms)
	: ani(ani), ani_ms(ani_ms) { SetMaster(ctrl); }
};

void  PointLooper::LeftUp(Point, dword)
{
	result = true;
	EndLoop();
}

Image PointLooper::CursorImage(Point p, dword keyflags)
{
	return ani[int(GetTimeClick() / ani_ms % ani.GetCount())];
}

bool  PointLooper::Key(dword key, int)
{
	if(key == K_ESCAPE) {
		result = false;
		EndLoop();
	}
	return true;
}

bool PointLoop(Ctrl& ctrl, const Vector<Image>& ani, int ani_ms)
{
	PointLooper p(ctrl, ani, ani_ms);
	p.Run();
	return p;
}

bool PointLoop(Ctrl& ctrl, const Image& img)
{
	Vector<Image> m;
	m.Add(img);
	return PointLoop(ctrl, m, 1);
}

END_UPP_NAMESPACE
