#include "Display.h"

Value DisplayCtrl::GetData() const {
	return val;
}

void  DisplayCtrl::SetData(const Value& v) {
	val = v;
	Refresh();
}

void  DisplayCtrl::Paint(Draw& w) {
	Size sz = GetSize();
	//Rect r = GetRect();
	w.DrawRect(sz, paper);
	String txt = AsString(val);
	Size textsize = w.GetTextSize(txt, font);
	w.DrawText(sz.cx - textsize.cx - 5, (sz.cy - textsize.cy) / 2, txt, font, ink);
}

DisplayCtrl::DisplayCtrl() {
	val = 0;
	ink = SBlack;
	paper = SWhite;
	font = StdFont();
	SetFrame(BlackFrame());
}

DisplayCtrl::~DisplayCtrl() {}
