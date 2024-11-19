#include "CtrlLib.h"

namespace Upp {

CH_STYLE(TabCtrl, Style, StyleDefault)
{
	font = StdFont();
	tabheight = font.Info().GetHeight() + 8;
	margin = 2;
	sel = Rect(2, 2, 2, 2);
	edge = Rect(6, 6, 6, 6);
	extendleft = 0;
	CtrlsImageLook(normal, CtrlsImg::I_TAB, 4);
	CtrlsImageLook(first, CtrlsImg::I_FTAB, 4);
	CtrlsImageLook(last, CtrlsImg::I_LTAB, 4);
	CtrlsImageLook(both, CtrlsImg::I_BTAB, 4);
	body = CtrlsImg::TABB();
	for(int i = 0; i < 4; i++)
		text_color[i] = SColorText();
}

TabCtrl::Item& TabCtrl::Item::Text(const String& _text)
{
	text = _text;
	owner->Layout();
	return *this;
}

TabCtrl::Item& TabCtrl::Item::Picture(PaintRect d)
{
	pict = d;
	owner->Layout();
	return *this;
}

TabCtrl::Item& TabCtrl::Item::SetCtrl(Ctrl *_ctrl)
{
	if(ctrl)
		ctrl->Remove();
	ctrl = _ctrl;
	owner->Layout();
	return *this;
}

TabCtrl::Item& TabCtrl::Item::Slave(Ctrl *_slave)
{
	if(slave)
		slave->Remove();
	slave = _slave;
	if(slave)
		owner->pane.Add(*slave);
	int q = owner->sel;
	owner->sel = -1;
	owner->Set(q);
	return *this;
}

TabCtrl::Item& TabCtrl::Item::Enable(bool _en)
{
	enabled = _en;
	owner->Refresh();
	return *this;
}

TabCtrl::Item::Item()
{
	ctrl = slave = NULL;
	enabled = true;
	key = 0;
}

void TabCtrl::Item::Layout(int xp, int y, int cy)
{
	Font fnt = owner->style->font;
	Size chsz = GetTextSize("M", fnt);
	x = xp;
	Size sz = pict.GetStdSize();
	pictpos = Point(0, 0);
	if(sz.cx) {
		xp += chsz.cx / 2;
		pictpos.x = xp;
		pictpos.y = y + (cy - sz.cy) / 2;
		xp += sz.cx + chsz.cx / 2;
	}
	else
		xp += chsz.cx;
	sz = GetSmartTextSize(text, fnt);
	if(sz.cx) {
		textpos.x = xp;
		textpos.y = y + (cy - sz.cy) / 2;
		xp += sz.cx;
	}
	if(ctrl) {
		xp += chsz.cx / 2;
		sz = ctrl->GetRect().GetSize();
		ctrl->SetRect(xp, y + (cy - sz.cy) / 2, sz.cx, sz.cy);
		xp += sz.cx + chsz.cx / 2;
	}
	else
		xp += chsz.cx;
	cx = xp - x;
}

void TabCtrl::Item::Paint(Draw& w, int state)
{
	Size sz = pict.GetStdSize();
	pict.Paint(w, pictpos.x, pictpos.y, sz.cx, sz.cy, owner->style->text_color[state], Null);
	DrawSmartText(w, textpos.x, textpos.y, 99999, text, owner->style->font, owner->style->text_color[state]);
}

void TabCtrl::SyncTabs()
{
	int x = style->margin - x0;
	for(int i = 0; i < tab.GetCount(); i++) {
		Item& t = tab[i];
		t.Layout(x, style->sel.top * (i != sel),
		            style->tabheight + style->sel.top * (i == sel));
		x += t.cx;
	}
	left.Show(x0 > 0);
	right.Show(tab.GetCount() && tab.Top().Right() > tabs.GetSize().cx);
}

void TabCtrl::Layout()
{
	for(int i = 0; i < tab.GetCount(); i++)
		if(tab[i].ctrl)
			tab[i].ctrl->Remove();
	for(int i = 0; i < tab.GetCount(); i++)
		if(tab[i].ctrl)
			Ctrl::Add(*tab[i].ctrl);
	int th = style->tabheight + style->sel.top;
	tabs.TopPos(0, th + style->sel.bottom)
	    .HSizePos(0, style->sel.left + style->sel.right);
	SyncTabs();
	if(tab.GetCount() && tab.Top().Right() < TabsRight())
		x0 = 0;
	SyncHot();
	if(sel < tab.GetCount())
		ScrollInto(sel);
	pane.VSizePos(style->tabheight + style->edge.top, style->edge.bottom)
	    .HSizePos(style->edge.left, style->edge.right);
	int sx = DPI(16);
	left.LeftPos(0, sx).TopPos(th - sx, sx);
	right.RightPos(0, sx).TopPos(th - sx, sx);
	SyncTabs();
	Refresh();
}

Size TabCtrl::ComputeSize(Size pane)
{
	return Size(pane.cx + style->edge.left + style->edge.right,
	            pane.cy + style->tabheight + style->edge.top + style->edge.bottom);
}

Size TabCtrl::ComputeSize()
{
	Size sz(0, 0);
	for(int i = 0; i < tab.GetCount(); i++) {
		Ctrl *q = tab[i].slave;
		if(q)
			sz = max(sz, q->GetMinSize());
	}
	return ComputeSize(sz);
}

int TabCtrl::TabsRight()
{
	return tabs.GetSize().cx - style->sel.left - style->sel.right;
}

void TabCtrl::Tabs::Paint(Draw& w)
{
	static_cast<TabCtrl *>(GetParent())->PaintTabs(w);
}

Rect TabCtrl::GetOpaqueRect() const
{
	return IsTransparent() ? Rect(0, 0, 0, 0) : pane.GetRect();
}

void TabCtrl::PaintTabs(Draw& w)
{
	int tt = style->sel.top;
	int th = style->tabheight + tt;
	Size sz = GetSize();
	ChPaint(w, 0, th, sz.cx, sz.cy - th, style->body);
	for(int phase = 0; phase < 2; phase++) {
		for(int i = tab.GetCount() - 1; i >= 0; i--)
			if((sel == i) == phase) {
				Item& t = tab[i];
				Rect r = RectC(t.x, tt, t.cx, th - tt);
				if(i)
					r.left -= style->extendleft;
				if(phase) {
					r.left -= style->sel.left;
					r.right += style->sel.right;
					r.top -= tt;
					r.bottom += style->sel.bottom;
				}
				int ndx = !IsEnabled() || !t.enabled ? CTRL_DISABLED :
					       phase ? CTRL_PRESSED :
					       i == hot ? CTRL_HOT : CTRL_NORMAL;
				ChPaint(w, r,
					(tab.GetCount() == 1 ? style->both : i == 0 ? style->first :
					 i == tab.GetCount() - 1 ? style->last : style->normal)
					[ndx]
				);
				t.Paint(w, ndx);
			}
	}
}

void TabCtrl::Paint(Draw& w)
{
	int th = style->tabheight + style->sel.top;
	Size sz = GetSize();
	ChPaint(w, 0, th, sz.cx, sz.cy - th, style->body);
}

int  TabCtrl::GetTab(Point p) const
{
	if(p.y >= 0 && p.y < style->tabheight)
		for(int i = 0; i < tab.GetCount(); i++)
			if(p.x < tab[i].Right())
				return i;
	return -1;
}

void TabCtrl::CancelMode()
{
	hot = -1;
}

void TabCtrl::SyncHot()
{
	Point p = GetMousePos() - GetScreenRect().TopLeft();
	int h = GetTab(p);
	if(h != hot) {
		hot = h;
		tabs.Refresh();
	}
	if(IsReadOnly())
		hot = -1;
}

void TabCtrl::MouseMove(Point p, dword keyflags)
{
	SyncHot();
}

void TabCtrl::LeftDown(Point p, dword keyflags)
{
	if(!IsEditable())
		return;
	int h = GetTab(p);
	if(h >= 0 && tab[h].IsEnabled()) {
		Set(h);
		Action();
	}
}

void TabCtrl::MouseLeave()
{
	SyncHot();
}

void TabCtrl::ScrollInto(int i)
{
	if(i < 0)
		return;
	Item& t = tab[i];
	int tr = TabsRight();
	if(t.Right() > tr) {
		x0 += t.Right() - tr;
		tabs.Refresh();
		SyncTabs();
	}
	if(t.x < style->margin) {
		x0 += t.x - style->margin;
		tabs.Refresh();
		SyncTabs();
	}
}

void TabCtrl::Left()
{
	if(x0 <= 0)
		return;
	for(int i = tab.GetCount() - 1; i >= 0; i--)
		if(tab[i].x < style->margin) {
			ScrollInto(i);
			break;
		}
}

void TabCtrl::Right()
{
	for(int i = 0; i < tab.GetCount(); i++)
		if(tab[i].Right() > tabs.GetRect().GetWidth() - style->sel.left - style->sel.right) {
			ScrollInto(i);
			break;
		}
}

void TabCtrl::Set(int i)
{
	if(i != sel) {
		bool refocus = HasFocusDeep();
		sel = i;
		tabs.Refresh();
		SyncTabs();
		for(int i = 0; i < tab.GetCount(); i++)
			if(tab[i].slave)
				tab[i].slave->Show(sel == i);
		if(sel >= 0 && refocus)
			IterateFocusForward(tab[sel].slave, GetTopCtrl(), false, true);
		WhenSet();
	}
	ScrollInto(sel);
}

int  TabCtrl::Find(const Ctrl& slave) const
{
	for(int i = 0; i < tab.GetCount(); i++)
		if(tab[i].slave == &slave)
			return i;
	return -1;
}

void TabCtrl::Set(Ctrl& slave)
{
	int i = Find(slave);
	if(i >= 0)
		Set(i);
}

void TabCtrl::Remove(Ctrl& slave)
{
	int i = Find(slave);
	if(i >= 0)
		Remove(i);
}

int TabCtrl::FindInsert(Ctrl& slave)
{
	int i = Find(slave);
	return i < 0 ? GetCount() : i;
}

TabCtrl::Item& TabCtrl::Insert(Ctrl& before_slave)
{
	return Insert(FindInsert(before_slave));
}

TabCtrl::Item& TabCtrl::Insert(Ctrl& before_slave, const char *text)
{
	return Insert(FindInsert(before_slave), text);
}

TabCtrl::Item& TabCtrl::Insert(Ctrl& before_slave, const Image& m, const char *text)
{
	return Insert(FindInsert(before_slave), m, text);
}

TabCtrl::Item& TabCtrl::Insert(Ctrl& before_slave, Ctrl& slave, const char *text)
{
	return Insert(FindInsert(before_slave), slave, text);
}

TabCtrl::Item& TabCtrl::Insert(Ctrl& before_slave, Ctrl& slave, const Image& m, const char *text)
{
	return Insert(FindInsert(before_slave), slave, m, text);
}


void TabCtrl::SetData(const Value& data)
{
	Set(data);
}

Value TabCtrl::GetData() const
{
	return Get();
}

TabCtrl::Item& TabCtrl::Add()
{
	CancelMode();
	Item& t = tab.Add();
	t.owner = this;
	if(sel < 0)
		Set(0);
	Layout();
	return t;
}

TabCtrl::Item& TabCtrl::Add(const char *text)
{
	return Add().Text(text);
}

TabCtrl::Item& TabCtrl::Add(const Image& m, const char *text)
{
	return Add().Text(text).SetImage(m);
}

TabCtrl::Item& TabCtrl::Add(Ctrl& slave, const char *text)
{
	return Add(text).Slave(&slave);
}

TabCtrl::Item& TabCtrl::Add(Ctrl& slave, const Image& m, const char *text)
{
	return Add(slave, text).SetImage(m);
}

TabCtrl::Item& TabCtrl::Insert(int i)
{
	CancelMode();
	int c = i < sel ? sel + 1 : sel;
	TabCtrl::Item& m = tab.Insert(i);
	m.owner = this;
	Layout();
	sel = -1;
	Set(c);
	return m;
}

TabCtrl::Item& TabCtrl::Insert(int i, const char *text)
{
	return Insert(i).Text(text);
}

TabCtrl::Item& TabCtrl::Insert(int i, const Image& m, const char *text)
{
	return Insert(i).Text(text).SetImage(m);
}

TabCtrl::Item& TabCtrl::Insert(int i, Ctrl& slave, const char *text)
{
	return Insert(i, text).Slave(&slave);
}

TabCtrl::Item& TabCtrl::Insert(int i, Ctrl& slave, const Image& m, const char *text)
{
	return Insert(i, slave, text).SetImage(m);
}


void TabCtrl::Remove(int i)
{
	CancelMode();
	if(tab[i].ctrl)
		tab[i].ctrl->Remove();
	if(tab[i].slave)
		tab[i].slave->Remove();
	int c = i < sel ? sel - 1 : sel;
	tab.Remove(i);
	Layout();
	sel = -1;
	if(tab.GetCount())
		Set(minmax(c, 0, tab.GetCount() - 1));
	else {
		x0 = 0;
		sel = -1;
		accept_current = false;
		WhenSet();
	}
}

void TabCtrl::Go(int d)
{
	if(IsEditable() && tab.GetCount()) {
		int i = sel + d;
		while(i != sel) {
			if(i < 0)
				i = tab.GetCount() - 1;
			if(i >= tab.GetCount())
				i = 0;
			if(tab[i].IsEnabled()) {
				Set(i);
				break;
			}
			i += d;
		}
	}
}

bool TabCtrl::Key(dword key, int repcnt)
{
	switch(key) {
#ifdef PLATFORM_COCOA
	case K_ALT|K_TAB:
	case K_OPTION|K_TAB:
#endif
	case K_CTRL_TAB:
		GoNext();
		Action();
		return true;
#ifdef PLATFORM_COCOA
	case K_SHIFT|K_ALT|K_TAB:
	case K_SHIFT|K_OPTION|K_TAB:
#endif
	case K_SHIFT_CTRL_TAB:
		GoPrev();
		Action();
		return true;
	}
	return Ctrl::Key(key, repcnt);
}

bool TabCtrl::HotKey(dword key)
{
	switch(key) {
#ifdef PLATFORM_COCOA
	case K_ALT|K_TAB:
	case K_OPTION|K_TAB:
#endif
	case K_CTRL_TAB:
		GoNext();
		Action();
		return true;
#ifdef PLATFORM_COCOA
	case K_SHIFT|K_ALT|K_TAB:
	case K_SHIFT|K_OPTION|K_TAB:
#endif
	case K_SHIFT_CTRL_TAB:
		GoPrev();
		Action();
		return true;
	}
	return Ctrl::HotKey(key);
}

bool TabCtrl::Accept()
{
	if(tab.GetCount() == 0 || no_accept)
		return true;
	int ii = Get();
	if(accept_current)
		return !tab[ii].slave || tab[ii].slave -> Accept();
	Ptr<Ctrl> refocus = GetFocusChildDeep();
	for(int i = 0; i < tab.GetCount(); i++)
		if(tab[i].slave) {
			Set(i);
			if(!tab[i].slave->Accept())
				return false;
		}
	Set(ii);
	if(refocus)
		refocus->SetFocus();
	return true;
}

void TabCtrl::Reset()
{
	for(int i = 0; i < tab.GetCount(); i++) {
		if(tab[i].ctrl)
			tab[i].ctrl->Remove();
		if(tab[i].slave)
			tab[i].slave->Remove();
	}
	tab.Clear();
	x0 = 0;
	CancelMode();
	sel = -1;
	Refresh();
	accept_current = false;
	WhenSet();
}

TabCtrl::TabCtrl()
{
	hot = -1;
	sel = -1;
	x0 = 0;
	no_accept = accept_current = false;
	Ctrl::Add(tabs);
	Ctrl::Add(pane);
	tabs.BackPaint().IgnoreMouse();
	Ctrl::Add(left.SetMonoImage(CtrlsImg::SLA()).ScrollStyle().NoWantFocus());
	left <<= THISBACK(Left);
	Ctrl::Add(right.SetMonoImage(CtrlsImg::SRA()).ScrollStyle().NoWantFocus());
	right <<= THISBACK(Right);
	Transparent().NoWantFocus();
	tabs.Transparent().NoWantFocus();
	SetStyle(StyleDefault());
}

// ----------------------------------------------------------------

void TabDlg::PlaceButton(Button& b, int& r)
{
	if(b.GetParent()) {
		b.Remove();
		AddChild(&b, &tabctrl);
		b.RightPosZ(r, 64).BottomPosZ(4, 24);
		r += 72;
	}
}

void TabDlg::Rearrange()
{
	int r = 4;
	PlaceButton(apply, r);
	PlaceButton(cancel, r);
	PlaceButton(exit, r);
	PlaceButton(ok, r);
	SetRect(Rect(GetRect().TopLeft(), tabctrl.ComputeSize(sz) + Ctrl::LayoutZoom(Size(8, 40))));
}

TabCtrl::Item& TabDlg::Add0(Ctrl& tab, const char *text)
{
	Size tsz = max(tab.GetRect().GetSize(), sz);
	tab.SizePos();
	TabCtrl::Item& m = tabctrl.Add(tab, text);
	if(tsz != sz) {
		sz = tsz;
		Rearrange();
	}
	return m;
}

TabDlg& TabDlg::AButton(Button& b)
{
	if(binit) {
		exit.Remove();
		binit = false;
	}
	Ctrl::Add(b);
	Rearrange();
	return *this;
}

TabDlg::TabDlg()
{
	binit = true;
	Ctrl::Add(tabctrl.HSizePosZ(4, 4).VSizePosZ(4, 36));
	sz = Size(0, 0);
	ok.Ok().SetLabel(t_("OK"));
	Acceptor(ok, IDOK);
	cancel.Cancel().SetLabel(t_("Cancel"));
	Rejector(cancel, IDCANCEL);
	apply.SetLabel(t_("Apply"));
	Acceptor(apply, IDYES);
	exit.SetLabel(t_("Close"));
	Acceptor(exit, IDEXIT);
	Ctrl::Add(exit);
}

}
