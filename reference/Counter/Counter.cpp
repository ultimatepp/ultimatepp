#include "Counter.h"

Value CounterCtrl::GetData() const
{
	return number;
}

void  CounterCtrl::SetData(const Value& v)
{
	number = v;
	Refresh();
}

void  CounterCtrl::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, paper);
	String txt = AsString(number);
	sz = (sz - GetTextSize(txt, font)) / 2;
	w.DrawText(sz.cx, sz.cy, txt, font, ink);
}

void  CounterCtrl::LeftDown(Point, dword)
{
	number++;
	Refresh();
}

CounterCtrl::CounterCtrl()
{
	number = 0;
	ink = SBlack;
	paper = SWhite;
	font = StdFont();
	SetFrame(BlackFrame());
}

CounterCtrl::~CounterCtrl() {}
