#include "CtrlLib.h"

namespace Upp {

CH_STYLE(ProgressIndicator, Style, StyleDefault)
{
	classic = GUI_GlobalStyle() == GUISTYLE_CLASSIC;
	hlook = CtrlsImg::PI();
	hchunk = CtrlsImg::PIC();
	vlook = CtrlsImg::VPI();
	vchunk = CtrlsImg::VPIC();
	bound = false;
	nomargins = false;
}

Rect ProgressIndicator::GetMargins()
{
	if(style->classic || percent || !IsNull(color))
		return ChMargins(ViewEdge());
	Size sz = GetSize();
	Rect r = ChMargins(sz.cx > sz.cy ? style->hlook : style->vlook);
	if(style->nomargins)
		r.left = r.right = r.top = r.bottom = 0;
	return r;
}

Size ProgressIndicator::GetMsz()
{
	Size sz = GetSize();
	Rect mg = GetMargins();
	sz.cx -= mg.left + mg.right;
	sz.cy -= mg.top + mg.bottom;
	return sz;
}

void ProgressIndicator::Paint(Draw& w) {
	Size sz = GetSize();
	Size msz = GetMsz();
	int p0 = 0;
	int p = pxp;
	if(total <= 0) {
		int l = max(msz.cx, msz.cy) & ~7;
		p0 = pxp - l / 4;
		p = min(p - p0, max(msz.cx, msz.cy) - p0);
		if(style->bound && p0 < 0) {
			p += p0;
			p0 = 0;
		}
	}
	if(style->classic || percent || !IsNull(color)) {
		ChPaintEdge(w, sz, ViewEdge());
		Rect mg = GetMargins();
		sz -= Size(mg.left + mg.right, mg.top + mg.bottom);
		Rect r1, r2, r3;
		r1 = r2 = r3 = RectC(mg.left, mg.top, sz.cx, sz.cy);
		w.Clip(r1);
		if(sz.cx > sz.cy) {
			r1.right = r2.left = min(p, sz.cx) + mg.left + p0;
			r3.right = mg.left + p0;
		}
		else {
			r2.bottom = r1.top = sz.cy - min(p, sz.cy) + mg.top - p0;
			r3.top = r3.bottom - p0;
		}
		w.DrawRect(r1, Nvl(color, SColorHighlight()));
		w.DrawRect(r2, SColorPaper);
		w.DrawRect(r3, SColorPaper);
		if(percent) {
			String pt = Format("%d %%", (int)(100L * actual / max(total, 1)));
			Size psz = GetTextSize(pt, StdFont());
			int px = (sz.cx - psz.cx) / 2 + 2;
			int py = (sz.cy - psz.cy) / 2 + 2;
			w.Clip(r1);
			w.DrawText(px, py, pt, StdFont(), SColorLight);
			w.End();
			w.Clip(r2);
			w.DrawText(px, py, pt, StdFont(), SColorText);
			w.End();
		}
		w.End();
	}
	else {
		Rect r = GetMargins();
		if(sz.cx > sz.cy) {
			ChPaint(w, sz, style->hlook);
			w.Clip(r.left, r.top, sz.cx - r.left - r.right, sz.cy - r.top - r.bottom);
			ChPaintNoCache(w, r.left + p0, r.top, p, sz.cy - r.top - r.bottom, style->hchunk);
		}
		else {
			ChPaint(w, sz, style->vlook);
			w.Clip(r.left, r.top, sz.cx - r.left - r.right, sz.cy - r.top - r.bottom);
			ChPaintNoCache(w, r.left, sz.cy - r.bottom - p - p0, sz.cx - r.left - r.right, p, style->vchunk);
		}
		w.End();
	}
}

void ProgressIndicator::Layout() {
	Set(actual, total);
}

void ProgressIndicator::Set(int _actual, int _total) {
	actual = _actual;
	total = _total;
	Size sz = GetMsz();
	int p;
	if(total <= 0) {
		int l = max(1, max(sz.cx, sz.cy) & ~7);
		int p = msecs() / 15 % (l + l / 4);
		if(pxp != p) {
			pxp = p;
			Refresh();
		}
		return;
	}
	else {
		int l = max(1, max(sz.cx, sz.cy));
		p = total ? min(iscale(actual, l, total), l) : 0;
	}
	if(p != pxp) {
		pxp = p;
		Refresh();
	}
}

ProgressIndicator::ProgressIndicator() {
	style = &StyleDefault();
	Transparent();
	NoWantFocus();
	total = actual = 0;
	percent = false;
	color = Null;
	pxp = 0;
}

ProgressIndicator::~ProgressIndicator() {}

void Progress::Reset() {
	pos = 0;
	total = 0;
	cancel = false;
	granularity = 50;
	show_delay = 250;
	
	set_time = show_time = msecs(); // + 300;
}

void Progress::Init() {
	CtrlLayout(*this, t_("Working..."));
	stop <<= callback(this, &Progress::Cancel);
	info.SetAlign(ALIGN_CENTER);
	owner = NULL;
	Reset();
}

Progress::Progress() {
	Init();
}

Progress::Progress(Ctrl *owner) {
	Init();
	SetOwner(owner);
}

Progress::Progress(const char *s, int total) {
	Init();
	text = s;
	SetTotal(total);
}

Progress::Progress(Ctrl *owner, const char *s, int total) {
	Init();
	text = s;
	SetTotal(total);
	SetOwner(owner);
}

Progress::~Progress() {}

void Progress::Create() {
	if(IsPainting() || IsOpen() || cancel)
		return;
	stop.Hide();
	if(owner)
		Open(owner);
	else
		Open();
	if(IsOpen()) { // in some context, e.g. headless skeleton, window does not open - need prevent infinite recursion here
		SetFocus();
		Show();
		modality.Begin(this);
		if(total) Set(pos, total);
		Setxt();
		Sync();
		Process();
	}
}

void Progress::Process()
{
	if(!IsMainThread())
		return;
	if(!IsOpen()) {
		dword t = msecs();
		if((int)(t - show_time) >= show_delay) {
			Create();
			show_time = t;
		}
	}
	if(IsOpen()) {
		GuiSleep(0);
		ProcessEvents();
	}
}

void Progress::SetText(const String& s)
{
	Mutex::Lock __(mtx);
	text = s;
	Setxt();
}

String FitText(const String& src, int cx, Font font)
{
	String r = src;
	int n = (GetTextSize(r, font).cx - cx) / font['W'] / 2;
	while(cx > 0  && GetTextSize(r, font).cx > cx && n > 0 && 2 * n < src.GetCount()) {
		r = src;
		r.Remove(r.GetCount() / 2 - n, 2 * n);
		r.Insert(r.GetCount() / 2, "...");
		n++;
	}
	return r;
}

void Progress::Setxt0()
{
	info = FitText(Format(text, pos), info.GetSize().cx, StdFont());
}

void Progress::Setxt()
{
	if(!IsMainThread())
		return;

	Setxt0();

	Process();
}

void Progress::Set(int apos, int atotal) {
	{
		Mutex::Lock __(mtx);
		pos = apos;
		total = atotal;
	}
	if(!IsMainThread())
		return;
	dword t = msecs();
	if(abs((int)(t - set_time)) < granularity)
		return;
	set_time = t;
	while(atotal > 30000) {
		atotal >>= 8;
		apos >>= 8;
	}
	pi.Set(apos, atotal);
	Setxt();
	Sync();
	
	Process();
}

void Progress::SetTotal(int atotal) {
	Set(pos, atotal);
}

void Progress::SetPos(int apos) {
	Set(apos, total);
}

void Progress::Step(int n)
{
	Mutex::Lock __(mtx);
	Set(pos + n, total);
}

void Progress::Close()
{
	modality.End();
	TopWindow::Close();
}

void Progress::Layout()
{
	Setxt0();
}

void Progress::Cancel() {
	cancel = true;
	Close();
}

bool Progress::Canceled()
{
	if(IsMainThread()) {
		stop.Show();
		Process();
	}
	return cancel;
}

bool Progress::SetCanceled(int pos, int total)
{
	if(IsMainThread())
		stop.Show();
	Mutex::Lock __(mtx);
	Set(pos, total);
	return cancel;
}

bool Progress::SetPosCanceled(int pos)
{
	if(IsMainThread())
		stop.Show();
	Mutex::Lock __(mtx);
	Set(pos, total);
	return cancel;
}

bool Progress::StepCanceled(int n)
{
	if(IsMainThread())
		stop.Show();
	Mutex::Lock __(mtx);
	Step(n);
	return cancel;
}

}
