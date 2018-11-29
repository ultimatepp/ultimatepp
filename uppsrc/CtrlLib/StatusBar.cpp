#include "CtrlLib.h"

namespace Upp {

class RightInfoFrame : public CtrlFrame {
	virtual void FrameLayout(Rect& r);
	virtual void FramePaint(Draw& draw, const Rect& r);
	virtual void FrameAddSize(Size& sz);
};

void RightInfoFrame::FrameLayout(Rect& r)
{
	r.left += 3;
	r.right--;
	r.top++;
	r.bottom--;
}

void RightInfoFrame::FramePaint(Draw& w, const Rect& r)
{
	w.DrawRect(r.left, r.top, 2, r.Height(), SColorFace);
	DrawFrame(w, r.left + 2, r.top, r.Width() - 2, r.Height(), SColorShadow, SColorLight);
}

void RightInfoFrame::FrameAddSize(Size& sz)
{
	sz.cx += 4;
	sz.cy += 2;
}

class LeftInfoFrame : public CtrlFrame {
	virtual void FrameLayout(Rect& r);
	virtual void FramePaint(Draw& draw, const Rect& r);
	virtual void FrameAddSize(Size& sz);
};

void LeftInfoFrame::FrameLayout(Rect& r)
{
	r.left++;
	r.right -= 3;
	r.top++;
	r.bottom--;
}

void LeftInfoFrame::FramePaint(Draw& w, const Rect& r)
{
	w.DrawRect(r.right - 2, r.top, 2, r.Height(), SColorFace);
	DrawFrame(w, r.left, r.top, r.Width() - 2, r.Height(), SColorShadow, SColorLight);
}

void LeftInfoFrame::FrameAddSize(Size& sz)
{
	sz.cx += 4;
	sz.cy += 2;
}

InfoCtrl::InfoCtrl()
{
	SetFrame(ThinInsetFrame());
	right = false;
	defaulttext = t_("Ready");
	Set(Null);
	Transparent();
}

void InfoCtrl::Paint(Draw& w)
{
	Size sz = GetSize();
	Rect r = sz;
	if(temp) {
		r.left = 2;
		temp.Paint(w, r);
	}
	else {
		r.right = 2;
		for(int i = 0; i < tab.GetCount() && r.right < sz.cx; i++) {
			const Tab& t = tab[i];
			r.left = r.right;
			if(t.width < 0)
				r.right = max(r.right, sz.cx + t.width);
			else
				r.right = min((dword)sz.cx, dword(r.right) + t.width);
			t.info.Paint(w, r);
		}
	}
}

int InfoCtrl::GetTabOffset(int t) const
{
	Size sz = GetSize();
	Rect r = sz;
	r.right = 2;
	for(int i = 0; i < tab.GetCount() && r.right < sz.cx; i++) {
		const Tab& t = tab[i];
		r.left = r.right;
		if(t.width < 0)
			r.right = max(r.right, sz.cx + t.width);
		else
			r.right = min((dword)sz.cx, dword(r.right) + t.width);
	}
	return r.right;
}

int InfoCtrl::GetRealTabWidth(int tabi, int width) const
{
	int cx = GetSize().cx;
	int x = 2;
	for(int i = 0; i < tabi; i++) {
		const Tab& t = tab[i];
		if(t.width < 0)
			x = max(x, cx + t.width);
		else {
			if((dword)x + t.width > (dword)cx) return 0;
			x += t.width;
		}
	}
	return width < 0 ? max(x, cx + width) - x : min(cx - x, width);
}

void InfoCtrl::FrameLayout(Rect& r) {
	(right ? LayoutFrameRight : LayoutFrameLeft)(r, this, cx ? cx : r.Height());
};

class VCenterDisplay : public Display
{
public:
	void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword s) const {
		if(IsType<Image>(q)) {
			Image img = q;
			Size sz = img.GetSize();
			w.DrawImage(r.left, r.top + (r.Height() - sz.cy) / 2, img);
		}
		else {
			String text;
			if(IsString(q))
				text = q;
			else
				text = StdFormat(q);
			w.DrawText(r.left, r.top + (r.Height() - StdFont().Info().GetHeight()) / 2,
			           text, StdFont(), ink);
		}
	}
};

void InfoCtrl::Set(int _tab, const PaintRect& info, int width)
{
	Tab& t = tab.At(_tab);
	t.width = width;
	t.info = info;
	Refresh();
}

void InfoCtrl::Set(int tab, const Value& info, int width)
{
	Set(tab, PaintRect(Single<VCenterDisplay>(), info), width);
}

void InfoCtrl::Set(const PaintRect& info)
{
	tab.Clear();
	Set(0, info, INT_MAX);
}

void InfoCtrl::Set(const Value& info)
{
	tab.Clear();
	Set(0, Nvl(info, (Value)defaulttext), INT_MAX);
}

void InfoCtrl::EndTemporary()
{
	temptime.Kill();
	temp.Clear();
	Refresh();
}

void InfoCtrl::Temporary(const PaintRect& info, int timeout)
{
	temptime.KillSet(timeout, THISBACK(EndTemporary));
	temp = info;
	Refresh();
}

void InfoCtrl::Temporary(const Value& info, int timeout)
{
	Temporary(PaintRect(Single<VCenterDisplay>(), info), timeout);
}

InfoCtrl& InfoCtrl::Left(int w)
{
	right = false;
	Width(w);
	SetFrame(Single<LeftInfoFrame>());
	return *this;
}

InfoCtrl& InfoCtrl::Right(int w)
{
	right = true;
	Width(w);
	SetFrame(Single<RightInfoFrame>());
	return *this;
}

CH_STYLE(StatusBar, Style, StyleDefault)
{
	look = SColorFace();
}

void StatusBar::TopFrame::FrameLayout(Rect& r)
{
	r.top += 2;
}

void StatusBar::TopFrame::FramePaint(Draw& w, const Rect& r)
{
	ChPaint(w, r, style->look);
}

void StatusBar::Paint(Draw& w)
{
	ChPaint(w, GetScreenRect().Offseted(-GetScreenView().TopLeft()), frame.style->look);
	InfoCtrl::Paint(w);
}

void StatusBar::TopFrame::FrameAddSize(Size& sz)
{
	sz.cy += 2;
}

StatusBar::StatusBar()
{
	Height(Zy(5) + max(16, Draw::GetStdFontCy()));
	frame.style = &StyleDefault();
	SetFrame(frame);
	AddFrame(grip);
}

StatusBar::~StatusBar()
{
}

StatusBar& StatusBar::Height(int _cy)
{
	cy = _cy;
	RefreshParentLayout();
	return *this;
}

void StatusBar::FrameLayout(Rect& r) {
	LayoutFrameBottom(r, this, cy ? cy : r.Width());
}

void StatusBar::FrameAddSize(Size& sz) {
	sz.cy += cy * IsShown();
}

class ProgressDisplayCls : public Display {
public:
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword s) const;
};

void ProgressDisplayCls::Paint(Draw& w, const Rect& _r, const Value& q,
                               Color ink, Color paper, dword s) const
{
	Rect r = _r;
	r.top += 1;
	r.bottom -= 1;
	DrawBorder(w, r, InsetBorder);
	r.Deflate(2);
	int pos = minmax(int((double)q * r.Width() / 1000), 0, r.Width());
	if(pos) {
		w.DrawRect(r.left, r.top, 1, r.Height(), SColorLight);
		w.DrawRect(r.left + 1, r.top, pos - 1, 1, SColorLight);
		w.DrawRect(r.left + 1, r.top + 1, pos - 1, r.Height() - 2, LtBlue);
		w.DrawRect(r.left + 1, r.top + r.Height() - 1, pos - 1, 1, SColorLight);
		w.DrawRect(r.left + pos - 1, r.top + 1, 1, r.Height() - 1, SColorLight);
	}
	w.DrawRect(r.left + pos, r.top, r.Width() - pos, r.Height(), SColorPaper);
};

Display& ProgressDisplay()
{
	return Single<ProgressDisplayCls>();
}

void ProgressInfo::Refresh()
{
	if(!info) return;
	String txt = Format(~text, pos);
	info->Set(tabi, txt, tw ? tw : GetTextSize(txt, StdFont()).cx + 8);
	info->Set(tabi + 1, PaintRect(ProgressDisplay(), 1000.0 * pos / (total ? total : 1000)), cx);
}

ProgressInfo& ProgressInfo::Set(int _pos, int _total)
{
	pos = _pos;
	total = _total;

	dword t = msecs();
	if(abs((int)(t - set_time)) >= granularity) {
		set_time = t;
	
		Refresh();
		if(info)
			info->Sync();
	}
	return *this;
}

void ProgressInfo::Reset()
{
	tabi = 0;
	cx = 200;
	total = 100;
	pos = 0;
	tw = 0;
	info = NULL;
	granularity = 50;
	set_time = 0;
}

ProgressInfo::~ProgressInfo()
{
	if(info)
		info->Set(Null);
}

}
