#include "CtrlLib.h"

namespace Upp {

SliderCtrl::SliderCtrl()
: value(Null)
, min(0)
, max(100)
, step(1)
, round_step(true)
, jump(false)
{
	Transparent();
	NoWantFocus();
}

bool SliderCtrl::IsVert() const
{
	return GetSize().cx < GetSize().cy;
}

int  SliderCtrl::HoVe(int x, int y) const
{
	return IsVert() ? y : x;
}

int SliderCtrl::ThumbSz() const
{
	return HoVe(CtrlImg::hthumb().GetSize().cx, CtrlImg::vthumb().GetSize().cy);
}

int SliderCtrl::SliderSz() const
{
	Size sz = GetSize();
	return HoVe(sz.cx, sz.cy) - ThumbSz();
}

int SliderCtrl::SliderToClient(int v) const
{
	if(IsNull(v))
		return Null;

	int sz = SliderSz();

	v = iscale(clamp(v, Min(), Max()) - min, sz, max - min);

	return HoVe(v, sz - v) + ThumbSz() / 2;
}

void SliderCtrl::Paint(Draw& w)
{
	Size size = GetSize();
	int ii = IsEnabled() ? HasCapture() || HasFocus() ? CTRL_PRESSED
                                                      : HasMouse() ? CTRL_HOT
                                                                   : CTRL_NORMAL
                         : CTRL_DISABLED;
	int l = SliderToClient(min);
	int t = Nvl(SliderToClient(value), l);
	int h = SliderToClient(max);
	Color c1 = SColorHighlight();
	if(Difference(c1, Gray()) < 16)
		c1 = SBlack();
	Color c2 = Gray();
	if(max < min)
		Swap(c1, c2);
	if(IsVert()) {
		int half = size.cx >> 1;
		w.DrawRect(half - DPI(1), h, DPI(2), t - h, c2);
		w.DrawRect(half - DPI(1), t, DPI(2), l - t, c1);
		if(!IsNull(value))
			w.DrawImage((size.cx - CtrlImg::vthumb().GetSize().cx) >> 1, t - ThumbSz() / 2,
			            CtrlImg::Get(CtrlImg::I_vthumb + ii));
	}
	else {
		int half = size.cy >> 1;
		w.DrawRect(l, half - DPI(1), t - l, DPI(2), c1);
		w.DrawRect(t, half - DPI(1), h - t, DPI(2), c2);
		if(!IsNull(value))
			w.DrawImage(t - ThumbSz() / 2, (size.cy - CtrlImg::hthumb().GetSize().cy) >> 1,
			            CtrlImg::Get(CtrlImg::I_hthumb + ii));
	}
	if(HasFocus())
		DrawFocus(w, size);
}

int SliderCtrl::ClientToSlider(int p) const
{
	int sz = SliderSz();
	p -= ThumbSz() / 2;
	if(IsVert())
		p = sz - p;
	return clamp(min + iscale(p, max - min, sz), Min(), Max());
}

bool SliderCtrl::Key(dword key, int repcnt)
{
	if(IsEditable())
		switch(key) {
		case K_LEFT:
		case K_UP:
			Dec();
			return true;
		case K_RIGHT:
		case K_DOWN:
			Inc();
			return true;
		}
	return Ctrl::Key(key, repcnt);
}

void SliderCtrl::LeftDown(Point pos, dword keyflags)
{
	if(!IsEditable())
		return;
	SetWantFocus();
	int thumb = SliderToClient(value);
	int p = HoVe(pos.x, pos.y);
	if(IsNull(thumb)) {
		value = ClientToSlider(p);
		WhenSlideFinish();
		UpdateActionRefresh();
	}
	else
	if(abs(p - thumb) <= ThumbSz() / 2)
		SetCapture();
	else
	if(jump) {
		value = ClientToSlider(p);
		WhenSlideFinish();
		UpdateActionRefresh();
	}
	else {
		if((min < max) != IsVert() ? p < thumb : p > thumb)
			Dec();
		else
			Inc();
	}
	Refresh();
}

void SliderCtrl::LeftRepeat(Point p, dword f)
{
	if(!HasCapture())
		LeftDown(p, f);
}

void SliderCtrl::LeftUp(Point pos, dword keyflags)
{
	if(HasCapture())
		WhenSlideFinish();
	Refresh();
	ReleaseCapture();
}

void SliderCtrl::MouseMove(Point pos, dword keyflags)
{
	if(HasCapture()) {
		int n = ClientToSlider(HoVe(pos.x, pos.y));
		if(n != value) {
			value = n;
			UpdateActionRefresh();
		}
	}
	Refresh();
}

void SliderCtrl::MouseEnter(Point p, dword keyflags)
{
	Refresh();
}

void SliderCtrl::MouseLeave()
{
	Refresh();
}

void SliderCtrl::SetData(const Value& v)
{
	int i = v;
	if(!IsNull(i))
		i = minmax(i, Min(), Max() );

	if(i != value) {
		value = i;
		UpdateRefresh();
	}
}

Value SliderCtrl::GetData() const
{
	return value;
}

SliderCtrl& SliderCtrl::MinMax(int _min, int _max)
{
	if(min != _min || max != _max) {
		min = _min;
		max = _max;
		if(!IsNull(value)) {
			int v = minmax(value, Min(), Max());
			if(value != v) {
				value = v;
				Update();
			}
		}
		Refresh();
	}
	return *this;
}

void SliderCtrl::Dec()
{
	int n = value;
	if(IsNull(value))
		n = Max();
	else
	if(n > Min()) {
		if(round_step && step > 1)
			n = idivfloor(n - 1, step) * step;
		else
			n -= step;
		if(n < Min())
			n = Min();
	}
	if(n != value) {
		value = n;
		WhenSlideFinish();
		UpdateActionRefresh();
	}
}

void SliderCtrl::Inc()
{
	int n = value;
	if(IsNull(value))
		n = Min();
	else
	if(n < Max()) {
		if(round_step && step > 1)
			n = idivceil(n + 1, step) * step;
		else
			n += step;
		if(n > Max())
			n = Max();
	}
	if(n != value) {
		value = n;
		WhenSlideFinish();
		UpdateActionRefresh();
	}
}

void SliderCtrl::GotFocus()
{
	Refresh();
}

void SliderCtrl::LostFocus()
{
	Refresh();
}

SliderCtrl::~SliderCtrl() {}

}
