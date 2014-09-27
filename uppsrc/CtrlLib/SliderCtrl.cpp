#include "CtrlLib.h"

NAMESPACE_UPP

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

int  SliderCtrl::HoVe(int  x, int  y) const
{
	return IsVert() ? y : x;
}

int& SliderCtrl::HoVeR(int& x, int& y) const
{
	return IsVert() ? y : x;
}

void SliderCtrl::Paint(Draw& w)
{
	Size size = GetSize();
	if(IsVert()) {
		int half = size.cx >> 1;
		DrawBorder(w, half - 2, 2, 4, size.cy - 4, InsetBorder);
		if(!IsNull(value))
			w.DrawImage((size.cx - CtrlImg::vthumb().GetSize().cx) >> 1, SliderToClient(value),
			            HasCapture() || HasFocus() ? CtrlImg::vthumb1() : CtrlImg::vthumb());
	}
	else {
		int half = size.cy >> 1;
		DrawBorder(w, 2, half - 2, size.cx - 4, 4, InsetBorder);
		if(!IsNull(value))
			w.DrawImage(SliderToClient(value), (size.cy - CtrlImg::hthumb().GetSize().cy) >> 1,
			            HasCapture() || HasFocus() ? CtrlImg::hthumb1() : CtrlImg::hthumb());
	}
	if(HasFocus())
		DrawFocus(w, size);
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
	if(p >= thumb && p < thumb + HoVe(CtrlImg::hthumb().GetSize().cx, CtrlImg::vthumb().GetSize().cy))
		SetCapture();
	else
	if(jump) {
		value = ClientToSlider(p);
		WhenSlideFinish();
		UpdateActionRefresh();		
	}
	else {		
		if( ( ( p < thumb) && (min == Min() ) ) || ( (p > thumb) && ( min == Max() ) ) )
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
	if (HasCapture())
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

int SliderCtrl::SliderToClient(int v) const
{
	if(IsNull(v))
		return Null;
	v = minmax(v, Min(), Max());

	v = iscale(v - min, HoVe(GetSize().cx - CtrlImg::hthumb().GetSize().cx,
		                         GetSize().cy - CtrlImg::vthumb().GetSize().cy), max - min);
	return v;
}

int SliderCtrl::ClientToSlider(int p) const
{
	Size hsz = CtrlImg::hthumb().GetSize();
	Size vsz = CtrlImg::vthumb().GetSize();
	p -= HoVe(hsz.cx / 2, vsz.cy / 2);
	return minmax(min + iscale(p, max - min,
	                           HoVe(GetSize().cx - hsz.cx, GetSize().cy - vsz.cy)), Min(), Max());
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

END_UPP_NAMESPACE
