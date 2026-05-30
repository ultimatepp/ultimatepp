#include "CtrlCore.h"

namespace Upp {

#define LLOG(x)     // DLOG(x)
#define LTIMING(x)  // DTIMING(x)

bool Ctrl::globalbackpaint;
bool Ctrl::globalbackbuffer;

bool Ctrl::was_fullrefresh;

static void sCheckGuiLock()
{
	ASSERT_(ThreadHasGuiLock(), "Using GUI in non-main thread without GuiLock");
}

void Ctrl::RefreshFrame(const Rect& r) {
	sCheckGuiLock();
	GuiLock __; // Beware: Even if we have ThreadHasGuiLock ASSERT, we still can be the main thread!
	if(!IsOpen() || !IsVisible() || r.IsEmpty())
		return;
	LTIMING("RefreshFrame");
	LLOG("RefreshRect " << Name() << ' ' << r);
	if(GuiPlatformRefreshFrameSpecial(r))
		return;
	if(!top && !IsDHCtrl()) {
		Ctrl *parent = GetParent();
		if(InFrame())
			parent->RefreshFrame(r + GetRect().TopLeft());
		else
			parent->Refresh(r + GetRect().TopLeft());
	}
	else {
		LLOG("WndInvalidateRect: " << r << ' ' << Name());
		LTIMING("RefreshFrame InvalidateRect");
		WndInvalidateRect(r);
	}
}

void Ctrl::Refresh0(const Rect& area) {
	RefreshFrame((area + GetView().TopLeft()) & GetView().Inflated(OverPaint()));
}

void Ctrl::Refresh(const Rect& area) {
	sCheckGuiLock();
	GuiLock __; // Beware: Even if we have ThreadHasGuiLock ASSERT, we still can be the main thread!
	if(fullrefresh || !IsVisible() || !IsOpen()) return;
	LLOG("Refresh " << Name() << ' ' <<  area);
	Refresh0(area);
}

void Ctrl::Refresh() {
	sCheckGuiLock();
	GuiLock __; // Beware: Even if we have ThreadHasGuiLock ASSERT, we still can be the main thread!
	if(fullrefresh || !IsVisible() || !IsOpen()) return;
	LLOG("Refresh " << Name() << " full:" << fullrefresh);
	Rect r = Rect(GetSize()).Inflated(OverPaint());
	if(r.IsEmpty())
		return;
	if(!GuiPlatformSetFullRefreshSpecial())
		was_fullrefresh = fullrefresh = true; // Needs to be set ahead because of possible MT ICall that can cause repaint during Refresh0
	Refresh0(Rect(GetSize()).Inflated(OverPaint()));
}

void Ctrl::Refresh(int x, int y, int cx, int cy) {
	Refresh(RectC(x, y, cx, cy));
}

void Ctrl::RefreshFrame(int x, int y, int cx, int cy) {
	RefreshFrame(RectC(x, y, cx, cy));
}

void Ctrl::RefreshFrame() {
	LLOG("RefreshFrame " << Name());
	RefreshFrame(Rect(GetRect().Size()).Inflated(overpaint));
}

Rect  Ctrl::GetClippedView()
{
	GuiLock __;
	Rect sv = GetScreenView();
	Rect view = sv;
	Ctrl *q = GetParent();
	Ctrl *w = this;
	while(q) {
		view &= w->InFrame() ? q->GetScreenRect() : q->GetScreenView();
		w = q;
		q = q->GetParent();
	}
	return view - GetScreenRect().TopLeft();
}

void  Ctrl::ScrollView(const Rect& _r, int dx, int dy)
{
	GuiLock __;
	LLOG("ScrollView " << _r << " " << dx << " " << dy);
	Refresh(_r);
}

void  Ctrl::ScrollView(int x, int y, int cx, int cy, int dx, int dy) {
	ScrollView(RectC(x, y, cx, cy), dx, dy);
}

void  Ctrl::ScrollView(int dx, int dy) {
	ScrollView(Rect(GetSize()), dx, dy);
}

Rect Ctrl::GetOpaqueRect() const
{
	return IsTransparent() ? Rect(0, 0, 0, 0) : GetSize();
}

Rect Ctrl::GetVoidRect() const
{
	return Rect(0, 0, 0, 0);
}


#ifdef _DEBUG

struct sDrawLevelCheck {
	Draw&        w;
	int          lvl;
	const Ctrl  *q;

	void Check() {
		ASSERT_(lvl == w.GetCloffLevel(), "Draw::Begin/End mismatch for " + UPP::Name(q));
	}

	sDrawLevelCheck(Draw& w, const Ctrl *q) : w(w), lvl(w.GetCloffLevel()), q(q) {}
	// NOTE: Checking level in destructor is a bad idea because of exceptions
};

#define LEVELCHECK(w, q)    sDrawLevelCheck _x_(w, q)
#define DOLEVELCHECK        _x_.Check();
#else
#define LEVELCHECK(w, q)
#define DOLEVELCHECK
#endif

void Ctrl::CtrlPaint(SystemDraw& w, const Rect& clip) {
	GuiLock __;
	LEVELCHECK(w, this);
	LTIMING("CtrlPaint");
	LLOG("=== CtrlPaint " << UPP::Name(this) << ", clip: " << clip << ", rect: " << GetRect() << ", view: " << GetView());
	Rect rect = GetRect().GetSize();
	Rect orect = rect.Inflated(overpaint);
	if(!IsShown() || orect.IsEmpty() || clip.IsEmpty() || !clip.Intersects(orect))
		return;
	Rect view = rect;
	int n = GetFrameCount();
	for(int i = 0; i < n; i++) {
		LEVELCHECK(w, NULL);
		Frame& f = GetFrame0(i);
		f.frame->FramePaint(w, view);
		view = f.GetView();
	}
	Rect oview = view.Inflated(overpaint);
	bool hasviewctrls = false;
	bool viewexcluded = false;
	bool hiddenbychild = false;
	for(Ctrl& q : *this)
		if(q.IsShown()) {
			if(q.GetRect().Contains(orect) && !q.IsTransparent())
				hiddenbychild = true;
			if(q.InFrame()) {
				if(!viewexcluded && IsTransparent() && q.GetRect().Intersects(view)) {
					w.Begin();
					w.ExcludeClip(view);
					viewexcluded = true;
				}
				LEVELCHECK(w, &q);
				Point off = q.GetRect().TopLeft();
				w.Offset(off);
				q.CtrlPaint(w, clip - off);
				w.End();
			}
			else
				hasviewctrls = true;
		}
	if(viewexcluded)
		w.End();
	DOLEVELCHECK;
	if(!hiddenbychild && !oview.IsEmpty() && oview.Intersects(clip) && w.IsPainting(oview)) {
		LEVELCHECK(w, this);
		if(overpaint) {
			w.Clip(oview);
			w.Offset(view.left, view.top);
			Paint(w);
			PaintCaret(w);
			w.End();
			w.End();
		}
		else {
			w.Clipoff(view);
			Paint(w);
			PaintCaret(w);
			w.End();
		}
	}
	if(hasviewctrls && !view.IsEmpty()) {
		Rect cl = clip & view;
		w.Clip(cl);
		for(Ctrl& q : *this)
			if(q.IsShown() && q.InView()) {
				LEVELCHECK(w, &q);
				Rect qr = q.GetRect();
				Point off = qr.TopLeft() + view.TopLeft();
				Rect ocl = cl - off;
				if(ocl.Intersects(Rect(qr.GetSize()).Inflated(overpaint))) {
					w.Offset(off);
					q.CtrlPaint(w, ocl);
					w.End();
				}
			}
		w.End();
	}
	DOLEVELCHECK;
}

int sShowRepaint;

void Ctrl::ShowRepaint(int q)
{
	sShowRepaint = q;
}

void ShowRepaintRect(SystemDraw& w, const Rect& r, Color c)
{
	if(sShowRepaint) {
		w.DrawRect(r, c);
		SystemDraw::Flush();
		Sleep(sShowRepaint);
	}
}

void CombineArea(Vector<Rect>& area, const Rect& r)
{
	LTIMING("CombineArea");
	if(r.IsEmpty()) return;
	auto Area = [](const Rect& r) {	return r.GetHeight() * r.GetWidth(); };
	int ra = Area(r);
	for(int i = 0; i < area.GetCount(); i++) {
		Rect ur = r | area[i];
		int a = Area(ur);
		if(a < 2 * (ra + Area(area[i])) || a < 16000) {
			area[i] = ur;
			return;
		}
	}
	area.Add(r);
}

void Ctrl::ExcludeDHCtrls(SystemDraw& w, const Rect& r, const Rect& clip)
{
	GuiLock __;
	LTIMING("PaintOpaqueAreas");
	if(!IsShown() || r.IsEmpty() || !r.Intersects(clip) || !w.IsPainting(r))
		return;
	Point off = r.TopLeft();
	Point viewpos = off + GetView().TopLeft();
	if(dynamic_cast<DHCtrl *>(this)) {
		w.ExcludeClip(r);
		return;
	}
	Rect cview = clip & (GetView() + off);
	for(Ctrl& q : *this)
		q.ExcludeDHCtrls(w, q.GetRect() + (q.InView() ? viewpos : off),
		                 q.InView() ? cview : clip);
}

void Ctrl::UpdateArea0(SystemDraw& draw, const Rect& clip, int backpaint)
{
	GuiLock __;
	LTIMING("UpdateArea");
	LLOG("========== UPDATE AREA " << UPP::Name(this) << ", clip: " << clip << " ==========");
	ExcludeDHCtrls(draw, GetRect().GetSize(), clip);
	auto DoCtrlPaint = [&](SystemDraw& w, const Rect& clip) {
	#if defined(PLATFORM_WIN32) && !defined(VIRTUALGUI)
		PaintWinBarBackground(w, clip);
	#endif
		CtrlPaint(w, clip);
	#if defined(PLATFORM_WIN32) && !defined(VIRTUALGUI)
		PaintWinBar(w, clip);
	#endif
		for(PaintHook h : painthook())
			h(this, w, clip);
	};
	if(globalbackbuffer) {
		DoCtrlPaint(draw, clip);
		LLOG("========== END (TARGET IS BACKBUFFER)");
		return;
	}
	if(backpaint == FULLBACKPAINT || globalbackpaint) {
		ShowRepaintRect(draw, clip, LtRed());
		BackDraw bw;
		bw.Create(draw, clip.GetSize());
		bw.Offset(-clip.TopLeft());
		bw.SetPaintingDraw(draw, clip.TopLeft());
		DoCtrlPaint(bw, clip);
		bw.Put(draw, clip.TopLeft());
		LLOG("========== END (FULLBACKPAINT)");
		return;
	}
	DoCtrlPaint(draw, clip);
	LLOG("========== END");
}

void SweepMkImageCache();

void Ctrl::UpdateArea(SystemDraw& draw, const Rect& clip)
{
	GuiLock __;
	if(IsPanicMode())
		return;
	RemoveFullRefresh();
	UpdateArea0(draw, clip, backpaint);
	SweepMkImageCache();
}

void Ctrl::RemoveFullRefresh()
{
	GuiLock __;
	fullrefresh = false;
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		q->RemoveFullRefresh();
}

void Ctrl::FullRefreshCleanup()
{ // remove any potentially stuck fullrefresh
	GuiLock __;
	if(was_fullrefresh) {
		for(Ctrl *q : GetTopCtrls())
			q->RemoveFullRefresh();
		was_fullrefresh = false;
	}
}

Ctrl *Ctrl::GetTopRect(Rect& r, bool inframe, bool clip)
{
	GuiLock __;
	if(!inframe) {
		if(clip)
			r &= Rect(GetSize());
		r.Offset(GetView().TopLeft());
	}
	Ctrl *parent = GetParent();
	if(parent) {
		r.Offset(GetRect().TopLeft());
		return parent->GetTopRect(r, InFrame(), clip);
	}
	return this;
}

void  Ctrl::DoSync(Ctrl *q, Rect r, bool inframe)
{
	GuiLock __;
	ASSERT(q);
	LLOG("DoSync " << UPP::Name(q) << " " << r);
	Ctrl *top = q->GetTopRect(r, inframe);
	if(top && top->IsOpen())
		top->WndUpdate(r);
}

void  Ctrl::Sync()
{
	GuiLock __;
	LLOG("Sync " << Name());
	Ctrl *parent = GetParent();
	if(top && IsOpen()) {
		LLOG("Sync UpdateWindow " << Name());
		WndUpdate();
	}
	else
	if(parent)
		DoSync(parent, GetRect(), inframe);
	SyncCaret();
}

void Ctrl::Sync(const Rect& sr)
{
	GuiLock __;
	LLOG("Sync " << Name() << "   " << sr);
	DoSync(this, sr, true);
	SyncCaret();
}

void Ctrl::DrawCtrlWithParent(Draw& w, int x, int y)
{
	GuiLock __;
	Ctrl *parent = GetParent();
	if(parent) {
		Rect r = GetRect();
		Ctrl *top = parent->GetTopRect(r, inframe);
		w.Clip(x, y, r.Width(), r.Height());
		w.Offset(x - r.left, y - r.top);
		SystemDraw *ws = dynamic_cast<SystemDraw *>(&w);
		if(ws)
			top->UpdateArea(*ws, r);
		w.End();
		w.End();
	}
	else
		DrawCtrl(w, x, y);
}

void Ctrl::DrawCtrl(Draw& w, int x, int y)
{
	GuiLock __;
	w.Offset(x, y);

	SystemDraw *ws = dynamic_cast<SystemDraw *>(&w);
	if(ws)
		UpdateArea(*ws, GetRect().GetSize());

//	CtrlPaint(w, GetSize()); _DBG_

	w.End();
}

void  Ctrl::GlobalBackPaint(bool b)
{
	GuiLock __;
	globalbackpaint = b;
}

void  Ctrl::GlobalBackPaintHint()
{
	GlobalBackPaint();
}

void Ctrl::GlobalBackBuffer(bool b)
{
	GuiLock __;
	globalbackbuffer = b;
}

}
