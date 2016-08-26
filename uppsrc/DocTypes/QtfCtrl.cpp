#include "DocTypes.h"

namespace Upp {

void QtfCtrl::MouseWheel(Point p, int zdelta, dword keyflags) {
	sb.Wheel(zdelta);
}

QtfCtrl::QtfCtrl() {
	zoom = VertLayoutZoom(140);
	sb.SetLine(8).Hide();
	sb.WhenScroll = callback(this, &QtfCtrl::Scroll);
	NoWantFocus();
	tm = bm = lm = rm = 0;
	vcenter = false;
	color = SLtGray;
	Background(Null);
	AddFrame(sb);
}

QtfCtrl::~QtfCtrl() {}

QtfCtrl& QtfCtrl::SetLabel(const char *text) {
	Set(text);
	return *this;
}

bool QtfCtrl::Key(dword key, int count) {
	return sb.VertKey(key);
}

int QtfCtrl::GetHeight(int cx) const {
	cx -= lm + rm;
	if(cx <= 0) return 0;
	return doc.GetHeight(zoom, cx, cache);
}

SystemDraw& ScreenInfo();

void QtfCtrl::Layout() {
	Size sz = sb.GetViewSize();
	if(GetHeight(sz.cx) > sz.cy - tm - bm) {
		sb.Show();
		sb.Set(sb, GetSize().cy - tm - bm, GetHeight(sb.GetReducedViewSize().cx));
	}
	else
		sb.Hide();
//	if(WhenLink) //!! fails when text is set prior to WhenLink
	{
		int cx = GetSize().cx - lm - rm;
		if(cx <= 0) return;
#ifdef SYSTEMDRAW
		valuerect = doc.GetValueRects(zoom, NilDraw(), 0, 0, cx);
#else
		valuerect = doc.GetValueRects(zoom, ScreenInfo(), 0, 0, cx);
#endif
	}
}

Value QtfCtrl::FindValue(Point p) const {
	p.y += sb - tm;
	p.x -= lm;
	for(int i = 0; i < valuerect.GetCount(); i++) {
		const ValueRect& r = valuerect[i];
		if(r.rect.Contains(p))
			return r.value;
	}
	return Null;
}

Image QtfCtrl::CursorImage(Point p, dword) {
	if(!WhenLink) return Image::Arrow();
	Value v = FindValue(p);
	return IsNull(v) ? Image::Arrow() : CtrlImg::HandCursor();
}

void  QtfCtrl::LeftUp(Point p, dword keyflags) {
	SetWantFocus();
	if(!WhenLink) return;
	Value v = FindValue(p);
	if(IsNull(v)) return;
	WhenLink(v);
}

void QtfCtrl::Paint(Draw& w) {
	Size sz = GetSize();
	w.DrawRect(0, 0, sz.cx, sz.cy, color);
	int hg = GetHeight(sz.cx);
	sz.cx -= lm + rm;
	sz.cy -= tm + bm;
	w.Clipoff(lm, tm, sz.cx, sz.cy);
	int dcy = vcenter && hg <= sz.cy - tm - bm ? (sz.cy - hg) : 0;
	doc.Paint(zoom, w, 0, dcy / 2, sz.cx, sz.cy - dcy, sb, cache, color);
	w.End();
}

void QtfCtrl::Scroll() {
	Refresh();
}

void QtfCtrl::Set(const char *s) {
	cache.Clear();
	doc.Clear();
	doc.Qtf(s, GetDefaultCharset());
	sb = 0;
	Layout();
	Refresh();
}

void QtfCtrl::Set(const Document& _doc) {
	cache.Clear();
	doc <<= _doc;
	sb = 0;
	Layout();
	Refresh();
}

void QtfCtrl::Pick(pick_ Document& _doc) {
	cache.Clear();
	doc = _doc;
	sb = 0;
	Layout();
	Refresh();
}

}
