#include "TCtrlLib.h"

NAMESPACE_UPP

RulerCtrl::RulerCtrl()
{
	width = GetStdWidth();
	scale = 1;
	delta = 0;
	cursor = Null;
	raw_delta = Null;
	min_range = Null;
	max_range = Null;
	small_repeat = Null;
	text_repeat = Null;
	text_convert = NULL;
	font = Arial(11);
	is_vert = false;
	is_right = false;
	background = Yellow;
	outer_color = SGray();
	autoselect = false;
}

RulerCtrl::~RulerCtrl() {}

void RulerCtrl::SetZoom(double sc, double dl)
{
	scale = sc;
	delta = dl;
	Layout();
}

void RulerCtrl::MinMax(double min, double max)
{
	min_range = min;
	max_range = max;
	Refresh();
}

void RulerCtrl::PickSmallStep(pick_ Vector<double>& pos, double repeat)
{
	small_step = pos;
	small_repeat = repeat;
	Refresh();
}

void RulerCtrl::SetSmallStep(double delta)
{
	small_step.Clear();
	small_step.Add(0);
	small_repeat = delta;
	Refresh();
}

void RulerCtrl::PickTextStep(pick_ Vector<double>& pos, pick_ Vector<String>& texts, double repeat, const Convert& convert)
{
	text_step = pos;
	text_value = texts;
	text_repeat = repeat;
	text_convert = &convert;
}

void RulerCtrl::SetTextStep(double repeat, const Convert& convert)
{
	text_step.SetCount(1);
	text_step[0] = 0;
	text_value.Clear();
	text_repeat = repeat;
	text_convert = &convert;
}

double RulerCtrl::Snap(double v) const
{
	if(small_step.IsEmpty() || IsNull(small_repeat))
		return v;
	double base = 0;
	if(!IsNull(small_repeat)) {
		base = small_repeat * floor(v / small_repeat);
		v -= base;
	}
	double best_v = small_step[0] + Nvl(small_repeat, 0.0);
	double best = fabs(best_v - v);
	for(int i = 0; i < small_step.GetCount(); i++) {
		double d = fabs(v - small_step[i]);
		if(d < best) {
			best = d;
			best_v = small_step[i];
		}
	}
	return best_v + base;
}

void RulerCtrl::Layout()
{
	raw_delta = Null;
	Refresh();
}

double RulerCtrl::GetRawDelta() const
{
	if(IsNull(raw_delta)) {
		raw_delta = delta;
		if(InFrame()) {
			Point pt = GetScreenView().TopLeft(), par = GetParent() ? GetParent() -> GetScreenView().TopLeft() : pt;
			raw_delta -= (is_vert ? pt.y - par.y : pt.x - par.x);
		}
	}
	return raw_delta;
}

void RulerCtrl::FrameLayout(Rect& rc)
{
	Rect pos = rc;
	int wd = (IsShown() ? width : 0);
	if(IsVert()) {
		if(IsBottomRight())
			rc.right = max(rc.left, pos.left = rc.right - wd);
		else
			rc.left = min(rc.right, pos.right = pos.left + wd);
	}
	else {
		if(IsBottomRight())
			rc.bottom = max(rc.top, pos.top = pos.bottom - wd);
		else
			rc.top = min(rc.bottom, pos.bottom = pos.top + wd);
	}
	SetFrameRect(pos);
}

void RulerCtrl::FrameAddSize(Size& sz)
{
	if(IsShown()) {
		if(IsVert())
			sz.cx += width;
		else
			sz.cy += width;
	}
}

void RulerCtrl::RefreshCursor(double cursor)
{
	if(!IsNull(cursor)) {
		double cl = ToClientf(cursor);
		Size size = GetSize();
		int lim = (is_vert ? size.cy : size.cx);
		if(cl >= -ARROW_RAD && cl <= lim + ARROW_RAD) {
			int icl = fround(cl);
			if(is_vert)
				Refresh(0, icl - ARROW_RAD, size.cx, ARROW_DIAM);
			else
				Refresh(icl - ARROW_RAD, 0, ARROW_DIAM, size.cy);
		}
	}
}

void RulerCtrl::SetCursor(double c)
{
	if(c != cursor) {
		RefreshCursor(cursor);
		RefreshCursor(cursor = c);
	}
}

void RulerCtrl::SetData(const Value& v)
{
	SetCursor(v);
}

Value RulerCtrl::GetData() const
{
	return cursor;
}

void RulerCtrl::Paint(Draw& draw)
{
	Size client = GetSize();
	int csize = is_vert ? client.cy : client.cx;
	int cheight = is_vert ? client.cx : client.cy;
	Rect clip = GetSize();
	int cy = font.Info().GetHeight();
	double pos1 = FromClient(0);
	double pos2 = FromClient(csize);
	if(pos1 > pos2)
		Swap(pos1, pos2);
	double lrange = min_range, rrange = max_range;
	if(!IsNull(min_range) && pos1 < min_range) pos1 = min_range;
	if(!IsNull(max_range) && pos2 > max_range) pos2 = max_range;
	int cli1 = 0, cli2 = csize;
	if(scale < 0)
		Swap(lrange, rrange);
	if(!IsNull(lrange))
		cli1 = max<int>(cli1, ToClient(lrange));
	if(!IsNull(rrange))
		cli2 = min<int>(cli2, ToClient(rrange));
	Rect nclip;
	if(is_vert) {
		draw.DrawRect(0, 0, client.cx, cli1, outer_color);
		if(!IsNull(lrange))
			draw.DrawRect(0, cli1++, client.cx, 1, SBlack);
		draw.DrawRect(0, cli2, client.cx, client.cy - cli2, outer_color);
		if(!IsNull(rrange))
			draw.DrawRect(0, --cli2, client.cx, 1, SBlack);
		draw.DrawRect(0, cli1, 1, cli2 - cli1, SGray);
//		draw.DrawRect(0, cli1, 1, cli2 - cli1, SBlack);
		draw.DrawRect(client.cx - 1, cli1, 1, cli2 - cli1, SBlack);
		nclip = RectC(1, cli1, client.cx - 2, cli2 - cli1);
	}
	else {
		draw.DrawRect(0, 0, cli1, client.cy, outer_color);
		if(!IsNull(lrange))
			draw.DrawRect(cli1++, 0, 1, client.cy, SBlack);
		draw.DrawRect(cli2, 0, client.cx - cli2, client.cy, outer_color);
		if(!IsNull(rrange))
			draw.DrawRect(--cli2, 0, 1, client.cy, SBlack);
		draw.DrawRect(cli1, 0, cli2 - cli1, 1, SGray);
		draw.DrawRect(cli1, client.cy - 1, cli2 - cli1, 1, SBlack);
		nclip = RectC(cli1, 1, cli2 - cli1, client.cy - 2);
	}
	draw.DrawRect(nclip, background);
	draw.Clip(nclip);

	enum {
		SMALL_LIMIT = 100,
//		TEXT_LIMIT = 50,
		TGAP = 3,
		HGAP = 2,
		VGAP = 0,
		HXGAP = 3,
		VXGAP = 2,
		SMALL_SIZE = 5,
	};

	double rep_count = 1;
	double rep_delta = 0;
	if(!IsNull(small_repeat)) {
		rep_delta = small_repeat * floor(pos1 / small_repeat);
		rep_count = ceil(pos2 / small_repeat) - floor(pos1 / small_repeat);
	}
	if(!small_step.IsEmpty() && rep_count > 0
	&& rep_count * small_step.GetCount() <= cli2 - cli1) {
		int ix = BinFindIndex(small_step, pos1 - rep_delta);
		int ppos = (is_right ? 0 : cheight - SMALL_SIZE);
		for(int c = fround(rep_count); --c >= 0; ix = 0, rep_delta += small_repeat) {
			int lim = small_step.GetCount();
			if(small_step.Top() > pos2 - rep_delta)
				lim = BinFindIndex(small_step, pos2 - rep_delta);
			for(; ix < lim; ix++) {
				int cli = ToClient(small_step[ix] + rep_delta);
				if(is_vert)
					draw.DrawRect(ppos, cli, SMALL_SIZE, 1, Gray());
				else
					draw.DrawRect(cli, ppos, 1, SMALL_SIZE, Gray());
			}
		}
	}
	rep_count = 1;
	double rep_index = 0;
	if(!IsNull(text_repeat)) {
		rep_index = floor(pos1 / text_repeat);
		rep_count = ceil(pos2 / text_repeat) - floor(pos1 / text_repeat) + 1;
	}
	if(!text_step.IsEmpty() && (!text_value.IsEmpty() || text_convert)
	&& rep_count > 0 /*&& rep_count * text_step.GetCount() <= TEXT_LIMIT*/) {
		int ix = BinFindIndex(text_step, pos1 - rep_index * text_repeat);
		int ppos = (is_right ? SMALL_SIZE : cheight - cy - SMALL_SIZE);
		int rpos = (is_right ? 0 : cheight - SMALL_SIZE - 1);
		for(int c = fround(rep_count); --c >= 0; ix = 0, rep_index++) {
			int lim = text_step.GetCount();
			double dp2 = pos2 - rep_index * text_repeat;
			if(text_step.Top() > dp2) {
				lim = BinFindIndex(text_step, dp2);
				if(lim + 1 < text_step.GetCount() && text_step[lim + 1] - dp2 < dp2 - text_step[lim])
					lim++;
			}
			for(; ix < lim; ix++) {
				double value = text_step[ix] + rep_index * text_repeat;
				int cli = ToClient(value);
				String text;
				if(ix < text_value.GetCount())
					text = text_value[ix];
				else if(text_convert)
					text = StdFormat(text_convert -> Format(value));
				else
					text = StdFormat(value);
				Size tsize = GetTextSize(text, font);
				int half = tsize.cx >> 1;
				if(is_vert) {
					draw.DrawRect(0, cli, ppos, 1, Black());
					draw.DrawRect(rpos, cli, SMALL_SIZE + 1, 1, Black());
//					draw.DrawRect(ppos - VGAP, cli - half - HGAP, tsize.cy + 2 * VGAP, tsize.cx + 2 * HGAP, background);
					draw.DrawText(ppos, cli + half, 900, text, font, Gray());
				}
				else {
					draw.DrawRect(cli, 0, 1, ppos, Black());
					draw.DrawRect(cli, rpos, 1, SMALL_SIZE + 1, Black());
//					draw.DrawRect(cli - half - HGAP, ppos - VGAP, tsize.cx + 2 * HGAP, tsize.cy + 2 * VGAP, background);
					draw.DrawText(cli - half, ppos, text, font, Gray());
				}
			}
		}
	}

	Font labelfont = font().Bold();
	double cl = IsNull(cursor) ? double(Null) : ToClientf(cursor);
	Size lsize = GetTextSize(label_text, labelfont);
	if(is_vert) {
		int tx = (IsBottomRight() ? client.cx - cy : 0);
//		draw.DrawRect(tx - VXGAP, cli2 - lsize.cx - TGAP - HXGAP, lsize.cy + 2 * VXGAP, lsize.cx + 2 * HXGAP, background);
		draw.DrawText(tx, cli2 - TGAP, 900, label_text, labelfont, Black());
		if(cl >= -ARROW_RAD && cl <= client.cy + ARROW_RAD) {
			int icl = fround(cl);
			Image icon = is_right ? CtrlImg::smallleft() : CtrlImg::smallright();
			draw.DrawImage(is_right ? 1 : client.cx - icon.GetWidth() - 1, icl - ARROW_RAD, icon);
		}
	}
	else {
		int tx = (IsBottomRight() ? client.cy - cy : 0);
//		draw.DrawRect(cli1 + TGAP - HXGAP, tx - VXGAP, lsize.cx + 2 * HXGAP, lsize.cy + 2 * VXGAP, background);
		draw.DrawText(cli1 + TGAP, tx, label_text, labelfont, Black());
		if(cl >= -ARROW_RAD && cl <= client.cx + ARROW_RAD) {
			int icl = fround(cl);
			Image icon = is_right ? CtrlImg::smallup() : CtrlImg::smalldown();
			draw.DrawImage(icl - ARROW_RAD, is_right ? 1 : client.cy - icon.GetHeight() - 1, icon);
		}
	}

	draw.End();
}

void RulerCtrl::LeftDown(Point pt, dword keyflags)
{
	double d = FromClient(is_vert ? pt.y : pt.x);
	if(autoselect)
		SetCursor(d);
	WhenSelect(d);
	if(autoselect) {
		WhenAction();
		SetCapture();
	}
	WhenLeftClick();
}

void RulerCtrl::LeftUp(Point pt, dword keyflags)
{
	ReleaseCapture();
}

void RulerCtrl::MouseMove(Point pt, dword keyflags)
{
	if(keyflags & K_MOUSELEFT) {
		double d = FromClient(is_vert ? pt.y : pt.x);
		if(autoselect)
			SetCursor(d);
		WhenSelect(d);
		if(autoselect)
			WhenAction();
	}
}

void RulerCtrl::RightDown(Point pt, dword keyflags)
{
	MenuBar::Execute(WhenBar);
}

int RulerCtrl::GetStdWidth()
{
	return 22;
}

END_UPP_NAMESPACE
