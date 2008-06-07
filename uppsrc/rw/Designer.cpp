#include "Designer.h"
#pragma hdrstop

#define  IMAGEFILE <rw/rw.iml>
#include <Draw/iml_source.h>

#define LLOG(x) // LOG(x)

int LayoutDesigner::GetTrack(int a, int b, int type) {
	return type == LEFT   ? a - TRACKERSIZE :
	       type == CENTER ? (a + b - TRACKERSIZE) / 2 :
	                        b;
}

Image LayoutDesigner::GetTrackShape(Point track) {
	const Image& (*id[3][3])() = {
		{ CtrlImg::SizeHoVe0, CtrlImg::SizeVert0, CtrlImg::SizeVeHo0 },
		{ CtrlImg::SizeHorz0, Image::SizeAll,     CtrlImg::SizeHorz0 },
		{ CtrlImg::SizeVeHo0, CtrlImg::SizeVert0, CtrlImg::SizeHoVe0 }
	};
	if(track.x == FORM)
		return CtrlImg::SizeHoVe0();
	if(track.x >= 0 && track.x <= 4 && track.y >= 0 && track.y <= 4)
		return (*id[track.y][track.x])();
	return Image::Arrow();
}

Rect LayoutDesigner::GetTracker(Point track) {
	if(track.x == FORM) {
		Rect r = form.GetRect() + sb;
		return RectC(r.right, r.bottom, TRACKERSIZE, TRACKERSIZE);
	}
	Rect actual = GetActual();
	return RectC(GetTrack(actual.left, actual.right, track.x),
		         GetTrack(actual.top, actual.bottom, track.y),
				 TRACKERSIZE, TRACKERSIZE);
}

Rect LayoutDesigner::GetSpringRect(int i) {
	Rect r = form.GetRect() + sb;
	r.Inflate(10, 10);
	Rect actual = GetActual();
	if(i == SLEFT || i == SRIGHT) {
		r.top = actual.top + actual.Height() / 4 - TRACKERSIZE;
		r.bottom = r.top + 2 * TRACKERSIZE;
		if(i == SLEFT)
			r.right = actual.left;
		else {
			int h2 = actual.Height() / 2;
			r.top += h2;
			r.bottom += h2;
			r.left = actual.right;
		}
	}
	if(i == STOP || i == SBOTTOM) {
		r.left = actual.left + actual.Width() / 4 - TRACKERSIZE;
		r.right = r.left + 2 * TRACKERSIZE;
		if(i == STOP)
			r.bottom = actual.top;
		else {
			int w2 = actual.Width() / 2;
			r.left += w2;
			r.right += w2;
			r.top = actual.bottom;
		}
	}
	return r;
}

Rect LayoutDesigner::GetCtrlRect(int i) {
	return ctrl[i].GetRect();
}

Rect LayoutDesigner::GetActualRect() {
	return itemi.IsEmpty() ? Rect(-INT_MAX, -INT_MAX, -INT_MAX, -INT_MAX)
		                   : GetCtrlRect(itemi.Top());
}

Rect LayoutDesigner::GetActual() {
	return GetActualRect() + Point(10, 10);
}

bool LayoutDesigner::GetSpring(int im, int i) {
	LogPos p = ctrl[im].GetPos();
	switch(i) {
	case SLEFT:
		return p.x.GetAlign() == Ctrl::LEFT || p.x.GetAlign() == Ctrl::SIZE;
	case SRIGHT:
		return p.x.GetAlign() == Ctrl::RIGHT || p.x.GetAlign() == Ctrl::SIZE;
	case STOP:
		return p.y.GetAlign() == Ctrl::TOP || p.y.GetAlign() == Ctrl::SIZE;
	case SBOTTOM:
		return p.y.GetAlign() == Ctrl::BOTTOM || p.y.GetAlign() == Ctrl::SIZE;
	};
	return false;
}

bool LayoutDesigner::GetSpring(int i) {
	if(itemi.IsEmpty()) return false;
	return GetSpring(itemi.Top(), i);
}

void LayoutDesigner::Paint(Draw& w) {
	Size sz = GetSize();
	w.DrawRect(0, 0, sz.cx, sz.cy, layouti < 0 ? SLtGray : SWhite);
	if(layouti >= 0) {
		Rect r = form.GetRect();
		DrawFatFrame(w, r, SBlack, -1);
		w.DrawRect(r.right, r.bottom, TRACKERSIZE, TRACKERSIZE, SLtBlue);
	}
}

bool LayoutDesigner::IsMoving() {
	return HasCapture() && track.x == CENTER && track.y == CENTER && cltm;
}

void LayoutDesigner::DrawHorzSpring(Draw& w, int i) {
	Rect r = GetSpringRect(i) - sb;
	bool spring = GetSpring(i);
	int wd = r.Width();
	int y = (r.top + r.bottom) / 2;
	Point p1 = Point(r.left, y);
	if(!spring)
		for(int i = 1; i < 10; i++) {
			Point p2 = Point(r.left + i * wd / 10, y + (1 - 2 * (i & 1)) * (TRACKERSIZE - 1));
			w.DrawLine(p1, p2, 1, SGray);
			p1 = p2;
		}
	w.DrawLine(p1, Point(r.right - spring, y), 1 + spring, spring ? SRed : SGray);
}

void LayoutDesigner::DrawVertSpring(Draw& w, int i) {
	Rect r = GetSpringRect(i) - sb;
	bool spring = GetSpring(i);
	int hg = r.Height();
	int x = (r.left + r.right) / 2;
	Point p1 = Point(x, r.top);
	if(!spring)
		for(int i = 1; i < 10; i++) {
			Point p2 = Point(x + (1 - 2 * (i & 1)) * (TRACKERSIZE - 1), r.top + i * hg / 10);
			w.DrawLine(p1, p2, 1, SGray);
			p1 = p2;
		}
	w.DrawLine(p1, Point(x, r.bottom - spring), 1 + spring, spring ? SRed : SGray);
}

void LayoutDesigner::PaintTrack(Draw& w) {
	if(track.x == SELECTRECT) {
		Rect m = selectrect;
		m.Normalize();
		if(!m.IsEmpty())
			DrawFrame(w, m, SLtRed, SLtRed);
	}
	else
	if(!itemi.IsEmpty() && !IsMoving()) {
		SetBar();
		Point p = sb;
		for(int i = 0; i < itemi.GetCount() - 1; i++) {
			Rect r = ctrl[itemi[i]].GetRect() + (Point(10, 10) - p);
			DrawFatFrame(w, r.left, r.top, r.Width(), r.Height(), SBrown, -3);
		}
		Rect actual = GetActual() - p;
		DrawFatFrame(w, actual.left, actual.top, actual.Width(), actual.Height(), SCyan, -3);
		DrawHorzSpring(w, SLEFT);
		DrawHorzSpring(w, SRIGHT);
		DrawVertSpring(w, STOP);
		DrawVertSpring(w, SBOTTOM);
		for(int h = LEFT; h <= RIGHT; h++)
			for(int v = TOP; v <= BOTTOM; v++)
				if(h != CENTER || v != CENTER)
					w.DrawRect(GetTracker(Point(h, v)) - p, SLtBlue);
	}
}

void LayoutDesigner::PaintForm(Draw& w) {
	Size sz = form.GetRect().Size();
	w.DrawRect(0, 0, sz.cx, sz.cy, SLtGray);
	for(Ctrl *q = form.GetFirstChild(); q; q = q->GetNext())
		if(q->IsTransparent())
			DrawFrame(w, q->GetRect(), SWhiteGray, SWhiteGray);
}

Point LayoutDesigner::FindTracker(Point p) {
	Point track;
	for(track.x = LEFT; track.x <= FORM; track.x++)
		for(track.y = TOP; track.y <= BOTTOM; track.y++)
			if((track.x != CENTER || track.y != CENTER) && GetTracker(track).Contains(p))
				return track;
	return Point(-1, -1);
}

int   LayoutDesigner::FindItem(Point p) {
	int i;
	int mini = -1;
	int min = INT_MAX;
	p += Point(-10, -10);
	for(i = 0; i < ctrl.GetCount(); i++) {
		Rect r = ctrl[i].GetRect();
		if(r.Contains(p)) {
			int m = r.Width() * r.Height();
			if(m < min) {
				mini = i;
				min = m;
			}
		}
	}
	return mini;
}

void LayoutDesigner::SetItemPos(int i, const Rect& r, int chi, bool val, bool sync) {
	bool l = chi == SLEFT   ? val : GetSpring(i, SLEFT);
	bool p = chi == SRIGHT  ? val : GetSpring(i, SRIGHT);
	bool t = chi == STOP    ? val : GetSpring(i, STOP);
	bool b = chi == SBOTTOM ? val : GetSpring(i, SBOTTOM);
	Ctrl& q = ctrl[i];
	Size sz = form.GetSize();
	if(l)
		if(p) q.HSizePos(r.left, sz.cx - r.right);
		else  q.LeftPos(r.left, r.Width());
	else
		if(p) q.RightPos(sz.cx - r.right, r.Width());
		else  q.HCenterPos(r.Width(), (r.left + r.right - sz.cx) / 2);
	if(t)
		if(b) q.VSizePos(r.top, sz.cy - r.bottom);
		else  q.TopPos(r.top, r.Height());
	else
		if(b) q.BottomPos(sz.cy - r.bottom, r.Height());
		else  q.VCenterPos(r.Height(), (r.top + r.bottom - sz.cy) / 2);
	Item(i).pos = q.GetPos();
	if(sync) {
		{ TIMING("SetItemPos::Status");
		ShowStatus();
		}
		{ TIMING("SetItemPos::Layout");
		Layout();
		}
		{ TIMING("SetItemPos::Sync");
		Sync();
		}
	}
}

void LayoutDesigner::SetActualPos(const Rect& r, int chi, bool val) {
	if(itemi.GetCount())
		SetItemPos(itemi.Top(), r, chi, val);
}

/*
void LayoutDesigner::SetAutoItemPos(int i, const Rect& r) {
	Ctrl& q = ctrl[i];
	Size sz = form.GetSize();
	if(r.left < sz.cx / 2)
		if(r.right < sz.cx / 2)
			q.LeftPos(r.left, r.Width());
		else
			q.HSizePos(r.left, sz.cx - r.right);
	else
		q.RightPos(sz.cx - r.right, r.Width());
	if(r.top < sz.cy / 2)
		if(r.bottom < sz.cy / 2)
			q.TopPos(r.top, r.Height());
		else
			q.VSizePos(r.top, sz.cy - r.bottom);
	else
		q.BottomPos(sz.cy - r.bottom, r.Height());
	Item(i).pos = q.GetPos();
	ShowStatus();
	Layout();
	Sync();
}
*/

Image LayoutDesigner::CursorImage(Point p, dword keyflags) {
	p += sb;
	if(IsMoving())
		return CrossCursor();
	if(HasCapture())
		return track.x == SELECTRECT ? Image::Arrow() : GetTrackShape(track);
	if(GetActual().Contains(p))
		return Image::SizeAll();
	Point track = FindTracker(p);
	if(track.x >= 0)
		return GetTrackShape(track);
	return Image::Arrow();
}

void LayoutDesigner::RefreshActual() {
	if(itemi.GetCount()) {
		Rect r = GetActual();
		r.Inflate(TRACKERSIZE, TRACKERSIZE);
		Refresh(r - sb);
		for(int i = SLEFT; i <= SBOTTOM; i++)
			Refresh(GetSpringRect(i) - sb);
	}
}

void LayoutDesigner::RefreshSelect(int i) {
	if(i == itemi.GetCount() - 1)
		RefreshActual();
	else {
		Rect r = ctrl[itemi[i]].GetRect() + Point(10, 10);
		r.Inflate(TRACKERSIZE, TRACKERSIZE);
		Refresh(r - sb);
	}
}

int  LayoutDesigner::FindSelect(int im) {
	for(int i = 0; i < itemi.GetCount(); i++)
		if(itemi[i] == im) return i;
	return -1;
}

void LayoutDesigner::UnselectItem(int im) {
	int i = FindSelect(im);
	if(i < 0) return;
	RefreshSelect(i);
	itemi.Remove(i);
	RefreshActual();
	if(itemi.GetCount() == 0)
		ClearSelection();
	else
		SelectItem(itemi.Top());
}

void LayoutDesigner::ClearSelection0() {
	for(int i = 0; i < itemi.GetCount(); i++) {
		RefreshSelect(i);
		itab.SetDisplay(itemi[i], 0, StdDisplay());
		itab.SetDisplay(itemi[i], 1, VarLblDisplay(false));
	}
	itemi.Clear();
	classname.Disable();
	classname.Clear();
	variable.Disable();
	variable.Clear();
	label.Disable();
	label.Set("");
	help.Disable();
	help.Set("");
	ShowStatus();
	SetBar();
}

void LayoutDesigner::ShowHideLayouts()
{
	if(layouti < 0)
		return;
	Vector<int> layouts;
	String dummy;
	int i;
	for(i = 0; i < ItemCount(); i++)
		if(ParseLayoutRef(Item(i).classname, dummy) >= 0)
			layouts.Add(i);
	for(i = 0; i < layouts.GetCount(); i++)
	{
		int li = layouts[i];
		Rect rc = ctrl[li].GetRect();
		bool show = (FindIndex(itemi, li) >= 0);
		if(!show)
		{
			show = true;
			for(int p = 0; p < layouts.GetCount(); p++)
				if(p != i && (ctrl[layouts[p]].GetRect() && rc) && (p < i || FindIndex(itemi, layouts[p]) >= 0))
				{
					show = false;
					break;
				}
		}
		ctrl[li].Show(show);
	}
	SynchronizeMnemonic();
}

void LayoutDesigner::ClearSelection() {
	ClearSelection0();
	itab.SetCursor(-1);
}

void LayoutDesigner::SelectItem(int im) {
	if(im < 0) return;
	RefreshActual();
	UnselectItem(im);
	itemi.Add(im);
	RefreshActual();
	itab.SetCursor(im);
	itab.SetDisplay(im, 0, SelDisplay);
	itab.SetDisplay(im, 1, VarLblDisplay(true));
	classname.Enable();
	variable.Enable();
	label.Enable();
	help.Enable();
	ItemInfo& ii = Item(im);
	classname <<= ii.classname;
	variable <<= ii.variable;
	label <<= Lng(ii.label);
	help <<= Lng(ii.help);
	ShowHideLayouts();
	ShowStatus();
	SetBar();
}

void LayoutDesigner::SelectOneItem(int im) {
	ClearSelection0();
	if(im < ctrl.GetCount())
		SelectItem(im);
}

int FilterName(int c) {
	return IsAlNum(c) ? ToLower(c) : 0;
}

void LayoutDesigner::WhenITab() {
	if(itab.GetCursor() >= 0)
		if(::GetCtrl() || ::GetShift())
			SelectItem(itab.GetCursor());
		else
			SelectOneItem(itab.GetCursor());
	SetFocus();
}

void LayoutDesigner::SetFormSize(Size sz) {
	Point p = sb;
	form.SetRect(-p.x + 10, -p.y + 10, sz.cx, sz.cy);
	ActualForm().size = sz;
	Refresh();
	Layout();
	Sync();
}

void LayoutDesigner::NewCtrl(int i) {
	String clss = i < 0 ? String() : ClassMap().GetKey(i);
	AddCtrl(clss, String(), String(), String());
	Size sz = ctrl[itemi.Top()].GetMinSize();
	SetActualPos(RectC(newctrl.x, newctrl.y, sz.cx * 8, sz.cy));
}

void LayoutDesigner::ClassMenu(Bar& menu) {
	menu.Add("User class", classset[-1]);
	menu.Separator();
	for(int i = 0; i < ClassMap().GetCount(); i++) {
		Bar::Item& m = menu.Add(ClassMap().GetKey(i), classset[i]);
		if((i + 2) % 16 == 0) menu.Break();
	}
}

void LayoutDesigner::RightDown(Point p, dword keyflags) {
	if(layouti < 0) return;
	newctrl = p;
	MenuBar bar;
	bar.SetFont(Arial(10)).LeftGap(2);
	ClassMenu(bar);
	bar.Execute();
}

void LayoutDesigner::RefreshSelectRect() {
	Rect m = selectrect;
	m.Normalize();
	Refresh(m.left, m.top, 1, m.Height());
	Refresh(m.left, m.top, m.Width(), 1);
	Refresh(m.left, m.bottom - 1, m.Width(), 1);
	Refresh(m.right - 1, m.top, 1, m.Height());
}

void LayoutDesigner::StartMoving(Point p) {
	Rect actual = GetActualRect();
	delta = p - actual.TopLeft();
	melta.SetCount(itemi.GetCount() - 1);
	for(int i = 0; i < itemi.GetCount() - 1; i++)
		melta[i] = GetCtrlRect(itemi[i]).TopLeft() - actual.TopLeft();
	track.x = track.y = CENTER;
	SetCapture();
}

void LayoutDesigner::DoMoving() {
	cltm = true;
	for(int i = 0; i < itemi.GetCount(); i++)
		RefreshSelect(i);
}

void LayoutDesigner::LeftDown(Point p, dword keyflags) {
	LOG("@@ LeftDown");
	if(layouti < 0) return;
	int i;
	moved = cltm = false;
	p += sb;
	SetFocus();
	Rect actual = GetActual();
	if(actual.Contains(p)) {
		if((keyflags & K_SHIFT) && itemi.GetCount()) {
			UnselectItem(itemi.Top());
			return;
		}
		StartMoving(p);
		DoMoving();
		return;
	}
	track = FindTracker(p);
	if(track.x >= 0) {
		Rect fr = form.GetRect();
		delta = Point(p.x - (track.x == FORM ? fr.right :
		                     track.x == LEFT ? actual.left : actual.right),
				      p.y - (track.x == FORM ? fr.bottom :
							 track.y == TOP ? actual.top : actual.bottom));
		SetCapture();
		return;
	}
	i = FindItem(p);
	if(i >= 0) {
		if(keyflags & K_SHIFT)
			if(ItemSelected(i))
				UnselectItem(i);
			else
				SelectItem(i);
		else
			if(!ItemSelected(i))
				SelectOneItem(i);
		StartMoving(p);
		DoMoving();
		return;
	}
	for(i = SLEFT; i <= SBOTTOM; i++)
		if(GetSpringRect(i).Contains(p)) {
			RefreshActual();
			SetActualPos(GetActualRect(), i, !GetSpring(i));
			return;
		}
	if(!(keyflags & (K_SHIFT|K_CTRL)))
		ClearSelection();
	track.x = SELECTRECT;
	selectrect.Set(p.x, p.y, p.x, p.y);
	RefreshSelectRect();
	SetCapture();
}

void LayoutDesigner::LeftRepeat(Point p, dword keyflags) {
	if(!cltm) DoMoving();
}

void LayoutDesigner::MouseMove(Point p, dword keyflags) {
	p += sb;
	if(track.x == SELECTRECT && HasCapture()) {
		RefreshSelectRect();
		selectrect.right = p.x;
		selectrect.bottom = p.y;
		RefreshSelectRect();
		return;
	}
	int gx = GridX();
	int gy = GridY();
	if(IsMoving() && itemi.GetCount()) {
		LLOG("LayoutDesigner::MouseMove -> " << p);
		bool m = false;
		Size sz = GetActualRect().Size();
		if(!moved) {
			RefreshSelect(itemi.GetCount() - 1);
			m = true;
		}
		Rect cr = RectC((p.x - delta.x) / gx * gx, (p.y - delta.y) / gy * gy, sz.cx, sz.cy);
		for(int i = 0; i < itemi.GetCount() - 1; i++) {
			int im = itemi[i];
			if(!moved) {
				RefreshSelect(i);
				m = true;
			}
			Size sz = GetCtrlRect(im).Size();
			SetItemPos(im, RectC(cr.left + melta[i].x, cr.top + melta[i].y, sz.cx, sz.cy), -1, false, false);
		}
		SetItemPos(itemi.Top(), cr, -1, false, true);
		if(m) moved = true;
		return;
	}
	p -= delta;
	if(HasCapture()) {
		if(track.x == FORM) {
			Size sz = Size(max(0, p.x - 10), max(0, p.y - 10));
			sz.cx = sz.cx / gx * gx;
			sz.cy = sz.cy / gy * gy;
			SetFormSize(sz);
			ShowStatus();
			return;
		}
		if(itemi.GetCount()) {
			Rect actual = GetActualRect();
			RefreshActual();
			p.Offset(-10, -10);
			if(track.x == LEFT) actual.left = min(actual.right, p.x) / gx * gx;
			if(track.x == RIGHT) actual.right = max(actual.left, p.x) / gy * gy;
			if(track.y == TOP) actual.top = min(actual.bottom, p.y) / gx * gx;
			if(track.y == BOTTOM) actual.bottom = max(actual.top, p.y) / gy * gy;
			if(!ignoremin) {
				Size sz = ctrl[itemi.Top()].GetMinSize();
				actual.right = max(actual.left + sz.cx, actual.right);
				actual.bottom = max(actual.top + sz.cy, actual.bottom);
			}
			SetActualPos(actual);
			RefreshActual();
			moved = true;
		}
	}
}

void LayoutDesigner::LeftUp(Point p, dword keyflags) {
	LOG("@@@ LeftUp");
	for(int i = 0; i < itemi.GetCount(); i++)
		RefreshSelect(i);
	if(track.x == SELECTRECT) {
		if(!(keyflags & (K_SHIFT|K_CTRL)))
			ClearSelection();
		Rect m = selectrect;
		m.Normalize();
		m.Offset(-10, -10);
		m += sb;
		for(int i = 0; i < ctrl.GetCount(); i++) {
			Rect r = ctrl[i].GetRect();
			if(r.left >= m.left && r.right <= m.right && r.top >= m.top && r.bottom <= m.bottom)
				SelectItem(i);
		}
		track.x = -1;
		RefreshSelectRect();
		return;
	}
	if(!(keyflags & K_SHIFT) && !(keyflags & K_CTRL) && !moved) {
		int i = FindItem(p + sb);
		if(i >= 0)
			SelectOneItem(i);
	}
}

void LayoutDesigner::Scroll() {
	SetFormSize(form.GetSize());
}

void LayoutDesigner::NewClass() {
	if(itemi.IsEmpty()) return;
	for(int i = 0; i < itemi.GetCount(); i++) {
		int im = itemi[i];
		Item(im).classname = classname;
		SetClass(im);
		RefreshActual();
	}
}

void LayoutDesigner::NewVariable() {
	if(itemi.IsEmpty()) return;
	itab.Set(itemi.Top(), 1, Item(itemi.Top()).variable = variable);
}

static int FilterOutCr(int c) {
	return c == '\r' ? 0 : c;
}

void LayoutDesigner::NewLabel() {
	if(itemi.IsEmpty()) return;
	ItemLabel(itemi.Top()) = Filter(label.Get(), FilterOutCr);
	SynchronizeCtrl();
	SynchronizeMnemonic();
}

void LayoutDesigner::NewHelp() {
	if(itemi.IsEmpty()) return;
	Help(itemi.Top()) = Filter(help.Get(), FilterOutCr);
	SynchronizeCtrl();
}

void LayoutDesigner::Layout() {
	sb.SetPage(GetSize());
	Size sz = form.GetSize() + Size(20, 20);
	for(int i = 0; i < ctrl.GetCount(); i++) {
		sz.cx = max(ctrl[i].GetRect().right + TRACKERSIZE + 20, sz.cx);
		sz.cy = max(ctrl[i].GetRect().bottom + TRACKERSIZE + 20, sz.cy);
	}
	sb.SetTotal(sz);
	int cx = frame.GetSize().cx;
//	menubar.LeftPos(0, cx / 2);
	status.Width(cx >> 1);
}

void LayoutDesigner::ShowStatus() {
	String s;
	if(layouti >= 0) {
		Size sz = form.GetRect().Size();
		s = "Layout size: " + ::AsString(sz);
		if(itemi.GetCount()) {
			Rect r = ctrl[itemi.Top()].GetRect();
			s << "  Item: " << ::AsString(r) <<
				 " - {" << sz.cx - r.right << ", " << sz.cy - r.bottom << '}';
		}
	}
	status.SetLabelText(s);
}

static int RoundStep(int org, int d, int g)
{
	return d ? itimesfloor(org + d * g + (d > 0 ? 0 : g - 1), g) - org : 0;
}

void LayoutDesigner::KeyMove(int dx, int dy, int sx, int sy) {
	if(itemi.IsEmpty())
		return;
	Rect main = GetCtrlRect(itemi.Top()), shift;
	int gx = GridX(), gy = GridY();
	shift.left = shift.right = RoundStep(main.left, dx, gx);
	shift.top = shift.bottom = RoundStep(main.top, dy, gy);
	shift.right += RoundStep(main.Width(), sx, gx);
	shift.bottom += RoundStep(main.Height(), sy, gy);
	for(int i = itemi.GetCount(); --i >= 0;) {
		RefreshSelect(i);
		SetItemPos(itemi[i], GetCtrlRect(itemi[i]) + shift, -1, false, i == 0);
		RefreshSelect(i);
	}
}

bool LayoutDesigner::Key(dword key, int count) {
	switch(key) {
	case K_RIGHT:
	case K_DOWN:
		if(itemi.IsEmpty())
			SelectOneItem(ctrl.GetCount() - 1);
		else
		if(itemi.Top() < ctrl.GetCount() - 1)
			SelectOneItem(itemi.Top() + 1);
		return true;
	case K_LEFT:
	case K_UP:
		if(itemi.IsEmpty())
			SelectOneItem(0);
		else
		if(itemi.Top() > 0)
			SelectOneItem(itemi.Top() - 1);
		return true;
	case K_PAGEUP:
		if(layouti > 0 && layout.GetCount() - 1)
			SetLayout(layouti - 1);
		break;
	case K_PAGEDOWN:
		if(layouti < layout.GetCount() - 1)
			SetLayout(layouti + 1);
		break;
	case K_HOME:
		SelectOneItem(0);
		break;
	case K_END:
		SelectOneItem(ctrl.GetCount() - 1);
		break;
	case K_SHIFT_LEFT:
		KeyMove(-1, 0, 0, 0);
		break;
	case K_SHIFT_RIGHT:
		KeyMove(1, 0, 0, 0);
		break;
	case K_SHIFT_UP:
		KeyMove(0, -1, 0, 0);
		break;
	case K_SHIFT_DOWN:
		KeyMove(0, 1, 0, 0);
		break;
	case K_SHIFT_CTRL_LEFT:
		KeyMove(0, 0, -1, 0);
		break;
	case K_SHIFT_CTRL_RIGHT:
		KeyMove(0, 0, 1, 0);
		break;
	case K_SHIFT_CTRL_UP:
		KeyMove(0, 0, 0, -1);
		break;
	case K_SHIFT_CTRL_DOWN:
		KeyMove(0, 0, 0, 1);
		break;
	default:
		if(key >= ' ' && key < 256) {
			label.SetFocus();
			return label.Key(key, count);
		}
		return menubar.HotKey(key);
	}
	SetFocus();
	return true;
}

void LayoutDesigner::AddItem() {
	if(layouti < 0) return;
	AddCtrl(String(), String(), String(), String());
}

void LayoutDesigner::MoveUp() {
#ifdef _DEBUG
	if(GetFocusCtrl())
		GetFocusCtrl()->Dump();
#endif
	if(itemi.GetCount()) {
		int sc = itab.GetCursorSc();
		Vector<int> ilist(itemi, 0), sorted(itemi, 0);
		Sort(sorted);
		if(sorted[0] == 0)
		{
			BeepExclamation();
			return;
		}
		ClearSelection();
		int i;
		for(i = 0; i < sorted.GetCount(); i++)
			Swap(sorted[i], sorted[i] - 1);
		for(i = 0; i < ilist.GetCount(); i++)
			SelectItem(ilist[i] - 1);
		itab.ScCursor(sc - itab.GetLineCy());
		itab.ScrollIntoCursor();
	}
}

void LayoutDesigner::MoveDown() {
	if(itemi.GetCount()) {
		int sc = itab.GetCursorSc();
		Vector<int> ilist(itemi, 0), sorted(itemi, 0);
		Sort(sorted);
		if(sorted.Top() >= ctrl.GetCount() - 1)
		{
			BeepExclamation();
			return;
		}
		ClearSelection();
		int i;
		for(i = sorted.GetCount(); --i >= 0;)
			Swap(sorted[i], sorted[i] + 1);
		for(i = 0; i < ilist.GetCount(); i++)
			SelectItem(ilist[i] + 1);
		itab.ScCursor(sc + itab.GetLineCy());
		itab.ScrollIntoCursor();
	}
}

void LayoutDesigner::ToggleIgnoreMin() {
	ignoremin = !ignoremin;
}

void LayoutDesigner::ToggleQLIB2Output() {
	qlib2_output = !qlib2_output;
}

void LayoutDesigner::ItemPos(int xy)
{
	ITEMPOS xpos = (ITEMPOS)LOWORD(xy), ypos = (ITEMPOS)HIWORD(xy);
	Size sz = form.GetSize(), half = sz >> 1;

	for(int i = 0; i < itemi.GetCount(); i++)
	{
		RefreshSelect(i);
		Ctrl& q = ctrl[itemi[i]];
		Rect r = q.GetRect();

		ITEMPOS axpos = xpos;
		if(axpos == POS_AUTO)
			axpos = (r.left < half.cy ? r.right < half.cy ? POS_LEFT : POS_SIZE : POS_RIGHT);
		switch(axpos)
		{
		case POS_LEFT:   q.LeftPos(r.left, r.Width()); break;
		case POS_RIGHT:  q.RightPos(sz.cx - r.right, r.Width()); break;
		case POS_CENTER: q.HCenterPos(r.Width(), (r.left + r.right - sz.cx) >> 1); break;
		case POS_SIZE:   q.HSizePos(r.left, sz.cx - r.right); break;
		}

		ITEMPOS aypos = ypos;
		if(aypos == POS_AUTO)
			aypos = (r.top < half.cy ? r.bottom < half.cy ? POS_TOP : POS_SIZE : POS_BOTTOM);
		switch(aypos)
		{
		case POS_TOP:    q.TopPos(r.top, r.Height()); break;
		case POS_BOTTOM: q.BottomPos(sz.cy - r.bottom, r.Height()); break;
		case POS_CENTER: q.VCenterPos(r.Height(), (r.top + r.bottom - sz.cy) >> 1); break;
		case POS_SIZE:   q.VSizePos(r.top, sz.cy - r.bottom); break;
		}

		SetItemPos(itemi[i], r);
		RefreshSelect(i);
	}
}

int LayoutDesigner::GetItemPosMask() const
{
	int xbits = 0, ybits = 0;
	for(int i = 0; i < itemi.GetCount(); i++)
	{
		LogPos pos = ctrl[itemi[i]].GetPos();
		switch(pos.x.GetAlign())
		{
		case Ctrl::LEFT:   xbits |= 1 << POS_LEFT; break;
		case Ctrl::RIGHT:  xbits |= 1 << POS_RIGHT; break;
		case Ctrl::CENTER: xbits |= 1 << POS_CENTER; break;
		case Ctrl::SIZE:   xbits |= 1 << POS_SIZE; break;
		}
		switch(pos.y.GetAlign())
		{
		case Ctrl::LEFT:   ybits |= 1 << POS_TOP; break;
		case Ctrl::RIGHT:  ybits |= 1 << POS_BOTTOM; break;
		case Ctrl::CENTER: ybits |= 1 << POS_CENTER; break;
		case Ctrl::SIZE:   ybits |= 1 << POS_SIZE; break;
		}
	}
	return MAKELONG(xbits, ybits);
}

/*
void LayoutDesigner::AutoPos() {
	for(int i = 0; i < itemi.GetCount(); i++) {
		int im = itemi[i];
		Rect r = GetCtrlRect(im);
		SetAutoItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::NormalPos() {
	for(int i = 0; i < itemi.GetCount(); i++) {
		int im = itemi[i];
		Rect r = GetCtrlRect(im);
		ctrl[im].TopPos(0, 0).LeftPos(0, 0);
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}
*/

void LayoutDesigner::VertCenter() {
	for(int i = 0; i < itemi.GetCount(); i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		ctrl[im].VCenterPos(0, 0);
		int h = r.Height();
		r.top = (form.GetSize().cy - h) / 2;
		r.bottom = r.top + h;
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::HorzCenter() {
	for(int i = 0; i < itemi.GetCount(); i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		ctrl[im].HCenterPos(0, 0);
		int w = r.Width();
		r.left = (form.GetSize().cx - w) / 2;
		r.right = r.left + w;
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::AlignLeft() {
	if(!itemi.GetCount()) return;
	int ax = GetCtrlRect(itemi.Top()).left;
	for(int i = 0; i < itemi.GetCount() - 1; i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		r.Offset(ax - r.left, 0);
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::AlignRight() {
	if(!itemi.GetCount()) return;
	int ax = GetCtrlRect(itemi.Top()).right;
	for(int i = 0; i < itemi.GetCount() - 1; i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		r.Offset(ax - r.right, 0);
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::AlignTop() {
	if(!itemi.GetCount()) return;
	int ay = GetCtrlRect(itemi.Top()).top;
	for(int i = 0; i < itemi.GetCount() - 1; i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		r.Offset(0, ay - r.top);
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::AlignBottom() {
	if(!itemi.GetCount()) return;
	int ay = GetCtrlRect(itemi.Top()).bottom;
	for(int i = 0; i < itemi.GetCount() - 1; i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		r.Offset(0, ay - r.bottom);
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::AlignVCenter() {
	if(!itemi.GetCount()) return;
	Rect dr = GetCtrlRect(itemi.Top());
	for(int i = 0; i < itemi.GetCount() - 1; i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		r.Offset(0, dr.top + (dr.Height() - r.Height()) / 2 - r.top);
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::AlignHCenter() {
	if(!itemi.GetCount()) return;
	Rect dr = GetCtrlRect(itemi.Top());
	for(int i = 0; i < itemi.GetCount() - 1; i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		r.Offset(dr.left + (dr.Width() - r.Width()) / 2 - r.left, 0);
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::AlignLbl() {
	if(!itemi.GetCount()) return;
	for(int i = 0; i < itemi.GetCount(); i++) {
		int im = itemi[i];
		if(Item(im).classname == "Label") {
			Ctrl *ref = NULL;
			if(im + 1 >= ItemCount() || Item(im + 1).classname == "Label") {
				if(im > 0 && Item(im - 1).classname != "Label")
					ref = &ctrl[im - 1];
			}
			else
				ref = &ctrl[im + 1];
			if(ref) {
				Rect dr = ref->GetRect();
				RefreshSelect(i);
				Rect r = GetCtrlRect(im);
				r.Offset(0, dr.top + (dr.Height() - r.Height()) / 2 - r.top);
				SetItemPos(im, r);
				RefreshSelect(i);
			}
		}
	}
}

void LayoutDesigner::SameWidth() {
	if(!itemi.GetCount()) return;
	int ax = GetCtrlRect(itemi.Top()).Width();
	for(int i = 0; i < itemi.GetCount() - 1; i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		r.right = r.left + ax;
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::SameHeight() {
	if(!itemi.GetCount()) return;
	int ay = GetCtrlRect(itemi.Top()).Height();
	for(int i = 0; i < itemi.GetCount() - 1; i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		r.bottom = r.top + ay;
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::SameSize() {
	if(!itemi.GetCount()) return;
	Rect m = GetCtrlRect(itemi.Top());
	for(int i = 0; i < itemi.GetCount() - 1; i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		r.right = r.left + m.Width();
		r.bottom = r.top + m.Height();
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::MinHeight() {
	for(int i = 0; i < itemi.GetCount(); i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		Size sz = ctrl[im].GetMinSize();
		r.bottom = r.top + sz.cy;
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::MinWidth() {
	for(int i = 0; i < itemi.GetCount(); i++) {
		int im = itemi[i];
		RefreshSelect(i);
		Rect r = GetCtrlRect(im);
		Size sz = ctrl[im].GetMinSize();
		r.right = r.left + sz.cx;
		SetItemPos(im, r);
		RefreshSelect(i);
	}
}

void LayoutDesigner::Cut() {
	Copy();
	Delete();
}

void LayoutDesigner::Copy() {
	String out;
	for(int i = 0; i < itemi.GetCount(); i++)
		out << SaveCtrl(Item(itemi[i]), i, "", false);
//	EmptyClipboard();
//	WriteClipboard(CF_TEXT, out);
	WriteClipboardText(out, true);
}

void LayoutDesigner::SelectToEnd(int i) {
	ClearSelection();
	while(i < ctrl.GetCount())
		SelectItem(i++);
}

void LayoutDesigner::Paste() {
	int i = ctrl.GetCount();
	try {
		String s = ReadClipboardText();
		CParser p(s);
		for(;;)
			LoadCtrl(p, ActualForm().item.Add(), langi);
	}
	catch(CParser::Error&) {
		ActualForm().item.Drop();
	}
	SynchronizeCtrls();
	SelectToEnd(i);
}

void LayoutDesigner::SelectAll() {
	SelectToEnd(0);
}

int FindIndex(const Vector<int>& c, int ii) {
	for(int i = 0; i < c.GetCount(); i++)
		if(c[i] == ii) return i;
	return -1;
}

void LayoutDesigner::Delete() {
	for(int ii = ctrl.GetCount(); ii >= 0; ii--)
		if(FindIndex(itemi, ii) >= 0) {
			UnselectItem(ii);
			ctrl.Remove(ii);
			ActualForm().item.Remove(ii);
			itab.Remove(ii);
		}
	Refresh();
}

void LayoutDesigner::DoDuplicate(const Vector<int>& imi, int dx, int dy) {
	if(layouti < 0) return;
	for(int i = 0; i < imi.GetCount(); i++) {
		int im = imi[i];
		const ItemInfo& ii = Item(im);
		const Ctrl& q = ctrl[im];
		AddCtrl(ii.classname, String(), Lng(ii.label), Lng(ii.help));
		SetItemPos(ctrl.GetCount() - 1, q.GetRect() + Point(dx, dy));
	}
}

void LayoutDesigner::Duplicate() {
	int si = ctrl.GetCount();
	Vector<int> imi(itemi, 1);
	ClearSelection();
	DoDuplicate(imi, 16, 16);
	SelectToEnd(si);
}

void LayoutDesigner::Matrix() {
	if(!itemi.GetCount()) return;
	if(matrix.Execute() != IDOK) return;
	int si = ctrl.GetCount();
	Vector<int> imi(itemi, 1);
	Rect br = GetActualRect();
	ClearSelection();
	for(int i = 0; i < imi.GetCount() - 1; i++)
		br |= GetCtrlRect(imi[i]);
	for(int x = 0; x < (int)~matrix.x; x++)
		for(int y = 0; y < (int)~matrix.y; y++)
			if(x > 0 || y > 0)
				DoDuplicate(imi, x * (br.Width() + (int)~matrix.hspace),
								 y * (br.Height() + (int)~matrix.vspace));
	SelectToEnd(si);
}

void LayoutDesigner::ToggleGrid() {
	usegrid = !usegrid;
	SetBar();
}

void LayoutDesigner::SetupAlias() {
	if(alias_map.Edit() && !alias_map.Save(alias_map_file))
		Exclamation(NFormat("Error saving alias file [* \1%s\1].", alias_map_file));
	UpdateClassList();
}
