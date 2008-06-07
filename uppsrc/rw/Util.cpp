#include "Designer.h"

struct VarLblDisplayCls : public Display
{
	VarLblDisplayCls(bool selected) : selected(selected) {}

	virtual void Paint(Draw& draw, const Rect& rc, const Value& value, Color i, Color p, dword style) const
	{
		ValueArray va(value);
		if(selected && !(style & CURSOR))
			i = SLtBlue;
		if(va.GetCount() <= 1 || !IsNull(va[0]))
			StdDisplay().Paint(draw, rc, va[0], i, p, style);
		else
		{
			if(!(style & CURSOR))
				i = Color(64, 160, selected ? 255 : 64);
			StdDisplay().Paint(draw, rc, va[1], i, p, style);
		}
	}

	bool selected;
};

const Display& VarLblDisplay(bool selected)
{
	if(selected)
	{
		static VarLblDisplayCls vls(true);
		return vls;
	}
	else
	{
		static VarLblDisplayCls vln(false);
		return vln;
	}
}

void SelectedDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper,
		                    dword style) const {
	w.DrawRect(r, paper);
	DrawSmartText(w, r.left, r.top, r.Width(),
	              IsString(q) ? String(q) : StdFormat(q), StdFont(),
	              (style & CURSOR) ? ink : SLtBlue);
}

const SelectedDisplay SelDisplay;

void UrgCtrl::SetLabelText(const char *s) {
	label = s;
	Refresh();
}

void UrgCtrl::Paint(Draw& w) {
	Size sz = GetSize();
	w.DrawRect(0, 0, sz.cx, sz.cy, SLtCyan);
	DrawSmartText(w, 0, 0, sz.cx, label);
	Size tsz = w.GetTextSize(classname, Arial(11));
	w.DrawRect(sz.cx - tsz.cx, sz.cy - tsz.cy, tsz.cx, tsz.cy, SBlue);
	w.DrawText(sz.cx - tsz.cx, sz.cy - tsz.cy, classname, Arial(11), SYellow);
}
