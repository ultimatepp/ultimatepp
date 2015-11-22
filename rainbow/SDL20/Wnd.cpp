#include "Local.h"

#ifdef GUI_SDL20

NAMESPACE_UPP

#define LLOG(x)   //DLOG(x)
#define LDUMP(x)  //DDUMP(x)
#define LDUMPC(x) //DDUMPC(x)

ImageBuffer    Ctrl::framebuffer;
Vector<Rect>   Ctrl::invalid;
Vector<Rect>   Ctrl::update;

Ptr<Ctrl>      Ctrl::desktop;
Vector<Ctrl *> Ctrl::topctrl;

Point          Ctrl::fbCursorPos = Null;
Image          Ctrl::fbCursorImage;
Point          Ctrl::fbCursorBakPos = Null;
Image          Ctrl::fbCursorBak;
Rect           Ctrl::fbCaretRect;
Image          Ctrl::fbCaretBak;
int            Ctrl::fbCaretTm;
int            Ctrl::renderingMode = MODE_ANTIALIASED;
bool           Ctrl::fbEndSession;
bool           Ctrl::FullWindowDrag;
int            Ctrl::PaintLock;

void Ctrl::SetDesktop(Ctrl& q)
{
	desktop = &q;
	desktop->SetRect(framebuffer.GetSize());
	desktop->SetOpen(true);
	desktop->NewTop();
	invalid.Add(framebuffer.GetSize());
}

void Ctrl::SetRenderingMode(int mode)
{
	renderingMode = mode;
	invalid.Add(framebuffer.GetSize());
}

void Ctrl::InitFB()
{
	Ctrl::GlobalBackBuffer();
	Ctrl::InitTimer();
	framebuffer.Create(1, 1);

#ifdef PLATFORM_POSIX
	SetStdFont(ScreenSans(12)); //FIXME general handling
#endif
	ChStdSkin();

	static StaticRect x;
	x.Color(Cyan());
	SetDesktop(x);
}

void Ctrl::EndSession()
{
	GuiLock __;
	LLOG("Ctrl::EndSession");
	fbEndSession = true;
	EndSessionLoopNo = EventLoopNo;
}

void Ctrl::ExitFB()
{
	TopWindow::ShutdownWindows();
	Ctrl::CloseTopCtrls();
	if(fbEndSession)
		FBQuitSession();
}

void Ctrl::SetFramebufferSize(Size sz)
{
	framebuffer.Create(sz);
	if(desktop)
		desktop->SetRect(sz);
	invalid.Add(sz);
	SyncTopWindows();
}

int Ctrl::FindTopCtrl() const
{
	for(int i = 0; i < topctrl.GetCount(); i++)
		if(this == topctrl[i])
			return i;
	return -1;
}

bool Ctrl::IsAlphaSupported()
{
	return false;
}

bool Ctrl::IsCompositedGui()
{
	return false;
}

Vector<Ctrl *> Ctrl::GetTopCtrls()
{
	Vector<Ctrl *> ctrl;
	if(desktop)
		ctrl.Add(desktop);
	for(int i = 0; i < topctrl.GetCount(); i++)
		if(!dynamic_cast<TopWindowFrame *>(topctrl[i]))
			ctrl.Add(topctrl[i]);
	return ctrl;
}

Ctrl *Ctrl::GetOwner()
{
	GuiLock __;
	int q = FindTopCtrl();
	if(q > 0 && topctrl[q]->top) {
		Ctrl *x = topctrl[q]->top->owner_window;
		LDUMP(Upp::Name(x));
		return dynamic_cast<TopWindowFrame *>(x) ? x->GetOwner() : x;
	}
	return NULL;
}

Ctrl *Ctrl::GetActiveCtrl()
{
	GuiLock __;
	return focusCtrl ? focusCtrl->GetTopCtrl() : NULL;
}

// Vector<Callback> Ctrl::hotkey;

int Ctrl::RegisterSystemHotKey(dword key, Callback cb)
{
/*	ASSERT(key >= K_DELTA);
	int q = hotkey.GetCount();
	for(int i = 0; i < hotkey.GetCount(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.At(q) = cb;
	dword mod = 0;
	if(key & K_ALT)
		mod |= MOD_ALT;
	if(key & K_SHIFT)
		mod |= MOD_SHIFT;
	if(key & K_CTRL)
		mod |= MOD_CONTROL;
	
	return RegisterHotKey(NULL, q, mod, key & 0xffff) ? q : -1;*/
	return -1;
}

void Ctrl::UnregisterSystemHotKey(int id)
{
/*	if(id >= 0 && id < hotkey.GetCount()) {
		UnregisterHotKey(NULL, id);
		hotkey[id].Clear();
	}*/
}

bool Ctrl::IsWaitingEvent()
{
	return FBIsWaitingEvent();
}

void Ctrl::AddUpdate(const Rect& rect)
{
	LLOG("@AddUpdate " << rect);
	AddRefreshRect(update, rect);
}

void Ctrl::AddInvalid(const Rect& rect)
{
	LLOG("@AddInvalid " << rect);
	AddRefreshRect(invalid, rect);
}

void Ctrl::SyncTopWindows()
{
	for(int i = 0; i < topctrl.GetCount(); i++) {
		TopWindow *w = dynamic_cast<TopWindow *>(topctrl[i]);
		if(w)
			w->SyncRect();
	}
}

bool Ctrl::ProcessEvent(bool *quit)
{
	LLOG("@ ProcessEvent");
	ASSERT(IsMainThread());
	if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
		ReleaseCtrlCapture();
	if(FBProcessEvent(quit)) {
		LLOG("FBProcesEvent returned true");
		SyncTopWindows();
		DefferedFocusSync();
		SyncCaret();
		return true;
	}
	return false;
}

Rect Ctrl::GetClipBound(const Vector<Rect>& inv, const Rect& r)
{
	Rect ri = Null;
	for(int j = 0; j < inv.GetCount(); j++) {
		Rect rr = inv[j] & r;
		if(!rr.IsEmpty())
			ri = IsNull(ri) ? rr : rr | ri;
	}
	return ri;
}


ViewDraw::ViewDraw(Ctrl *ctrl)
{
	if(Ctrl::invalid.GetCount())
		Ctrl::DoPaint();
	Ctrl::invalid.Clear();
	Ctrl::RemoveCursor();
	Ctrl::RemoveCaret();
	Rect r = ctrl->GetScreenView();
	Ctrl::invalid.Add(r);
	Ctrl::AddUpdate(r);
	for(int i = max(ctrl->GetTopCtrl()->FindTopCtrl() + 1, 0); i < Ctrl::topctrl.GetCount(); i++) {
		Rect rr = Ctrl::topctrl[i]->GetScreenRect();
		ExcludeClip(rr);
		Subtract(Ctrl::invalid, rr);
	}
	Offset(r.TopLeft());
}

ViewDraw::~ViewDraw()
{
	FBInitUpdate();
	Ctrl::DoUpdate();
	FBFlush();
//	Ctrl::invalid.Clear();
}

void Ctrl::DoUpdate()
{
	LLOG("DoUpdate");
	invalid.Clear();
	CursorSync();
	LDUMPC(update);
	screen.Present();
#if 0
	FBUpdate(framebuffer.GetSize());
#else
	for(int i = 0; i < update.GetCount(); i++) {
		LDUMP(update[i]);
		FBUpdate(update[i]);
	}
#endif
	update.Clear();
//	Sleep(1000);
}

Vector<Rect> Ctrl::GetPaintRects()
{
	Vector<Rect> r;
	r.Add(GetScreenRect());
	for(int i = max(FindTopCtrl() + 1, 0); i < topctrl.GetCount(); i++)
		Subtract(r, topctrl[i]->GetScreenRect());
	return r;
}


void DDRect(RGBA *t, int dir, const byte *pattern, int pos, int count)
{
	while(count-- > 0) {
		byte p = pattern[7 & pos++];
		t->r ^= p;
		t->g ^= p;
		t->b ^= p;
		t += dir;
	}
}

void Ctrl::DrawLine(const Vector<Rect>& clip, int x, int y, int cx, int cy, bool horz, const byte *pattern, int animation)
{
	if(cx <= 0 || cy <= 0)
		return;
	Vector<Rect> rr = Intersection(clip, RectC(x, y, cx, cy));
	for(int i = 0; i < rr.GetCount(); i++) {
		Rect r = rr[i];
		AddUpdate(r);
		if(horz)
			for(int y = r.top; y < r.bottom; y++)
				DDRect(framebuffer[y] + r.left, 1, pattern, r.left + animation, r.GetWidth());
		else
			for(int x = r.left; x < r.right; x++)
				DDRect(framebuffer[r.top] + x, framebuffer.GetWidth(), pattern, r.top + animation, r.GetHeight());
	}
}

void Ctrl::DragRectDraw0(const Vector<Rect>& clip, const Rect& rect, int n, const byte *pattern, int animation)
{
	int hn = min(rect.GetHeight(), n);
	int vn = min(rect.GetWidth(), n);
	DrawLine(clip, rect.left, rect.top, rect.GetWidth(), hn, true, pattern, animation);
	DrawLine(clip, rect.left, rect.top + hn, vn, rect.GetHeight() - hn, false, pattern, animation);
	DrawLine(clip, rect.right - vn, rect.top + hn, vn, rect.GetHeight() - hn, false, pattern, animation);
	DrawLine(clip, rect.left + vn, rect.bottom - hn, rect.GetWidth() - 2 * vn, hn, true, pattern, animation);
}

void Ctrl::DragRectDraw(const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                        Color color, int type, int animation)
{
	static byte solid[] =  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	static byte normal[] = { 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00 };
	static byte dashed[] = { 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };
	Point p = GetScreenView().TopLeft();
	Vector<Rect> pr;
	if(type & DRAWDRAGRECT_SCREEN) {
		pr.Add(Rect(framebuffer.GetSize()));
		type &= ~DRAWDRAGRECT_SCREEN;
		p = Point(0, 0);
	}
	else
		pr = Intersection(GetPaintRects(), clip.Offseted(p));
	const byte *pattern = type == DRAWDRAGRECT_DASHED ? dashed :
	                      type == DRAWDRAGRECT_NORMAL ? normal : solid;
	RemoveCursor();
	RemoveCaret();
	DragRectDraw0(pr, rect1.Offseted(p), n, pattern, animation);
	DragRectDraw0(pr, rect2.Offseted(p), n, pattern, animation);
}

void Ctrl::DoPaint()
{
	LLOG("@ DoPaint");
	if(!PaintLock) {
		bool scroll = false;
		if(desktop)
			desktop->SyncScroll();
		for(int i = 0; i < topctrl.GetCount(); i++)
			topctrl[i]->SyncScroll();
		if((invalid.GetCount() || scroll) && desktop) {
			RemoveCursor();
			RemoveCaret();
			for(int phase = 0; phase < 2; phase++) {
				LLOG("DoPaint invalid phase " << phase);
				LDUMPC(invalid);
				SystemDraw draw;
				draw.Set(screen);
				draw.Init(screen_size);
				for(int i = 0; i < invalid.GetCount(); i++) {
					draw.Clip(invalid[i]);
					AddUpdate(invalid[i]);
				}
				for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
					Rect r = topctrl[i]->GetRect();
					Rect ri = GetClipBound(invalid, r);
					if(!IsNull(ri)) {
						draw.Clipoff(r);
						topctrl[i]->UpdateArea(draw, ri - r.TopLeft());
						draw.End();
						Subtract(invalid, r);
						draw.ExcludeClip(r);
					}
				}
				Rect ri = GetClipBound(invalid, framebuffer.GetSize());
				if(!IsNull(ri))
					desktop->UpdateArea(draw, ri);
			}
		}
	}
	DoUpdate();
}

void Ctrl::WndUpdate(const Rect& r)
{
	GuiLock __;
	Rect rr = r + GetRect().TopLeft();
	bool dummy;
	Vector<Rect> h;
	h <<= invalid;
	invalid = Intersect(invalid, rr, dummy);
	FBInitUpdate();
	DoPaint();
	invalid <<= h;
	Subtract(invalid, rr);
	FBFlush();
}

bool Ctrl::ProcessEvents(bool *quit)
{
	//LOGBLOCK("@ ProcessEvents");
//	MemoryCheckDebug();
	if(!ProcessEvent(quit))
		return false;
	while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop()));
	TimeStop tm;
	TimerProc(GetTickCount());
	LLOG("TimerProc elapsed: " << tm);
	SweepMkImageCache();
	FBInitUpdate();
	DoPaint();
	FBFlush();
	return true;
}

void Ctrl::EventLoop(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT(IsMainThread());
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << LOG_BEGIN);
	Ptr<Ctrl> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	bool quit = false;
	int64 loopno = ++EventLoopNo;
	ProcessEvents(&quit);
	while(loopno > EndSessionLoopNo && !quit && (ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount()))
	{
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
		SyncCaret();
		GuiSleep(20);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		ProcessEvents(&quit);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / after ProcessEvents");
		LDUMP(loopno);
		LDUMP(fbEndSessionLoop);
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

void Ctrl::GuiSleep(int ms)
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("GuiSleep");
	int level = LeaveGuiMutexAll();
	FBSleep(ms);
	EnterGuiMutex(level);
}

Rect Ctrl::GetWndScreenRect() const
{
	GuiLock __;
	return GetRect();
}

void Ctrl::WndShow(bool b)
{
	GuiLock __;
}

void Ctrl::WndUpdate()
{
	GuiLock __;
}

bool Ctrl::IsWndOpen() const {
	GuiLock __;
	return FindTopCtrl() >= 0 || this == desktop;
}

void Ctrl::SetAlpha(byte alpha)
{
	GuiLock __;
}

Rect Ctrl::GetWorkArea() const
{
	GuiLock __;
	return framebuffer.GetSize();
}

void Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
	Array<Rect> r;
	r.Add(framebuffer.GetSize());
}

Rect Ctrl::GetVirtualWorkArea()
{
	return framebuffer.GetSize();
}

/*
Rect Ctrl::GetWorkArea(Point pt)
{
	return framebuffer.GetSize();
}
*/
Rect Ctrl::GetVirtualScreenArea()
{
	GuiLock __;
	return framebuffer.GetSize();
}

Rect Ctrl::GetPrimaryWorkArea()
{
	Rect r;
	return framebuffer.GetSize();
}

Rect Ctrl::GetPrimaryScreenArea()
{
	return framebuffer.GetSize();
}

int Ctrl::GetKbdDelay()
{
	GuiLock __;
	return 500;
}

int Ctrl::GetKbdSpeed()
{
	GuiLock __;
	return 1000 / 32;
}

void Ctrl::DestroyWnd()
{
	for(int i = 0; i < topctrl.GetCount(); i++)
		if(topctrl[i]->top && topctrl[i]->top->owner_window == this)
			topctrl[i]->WndDestroy();
	int q = FindTopCtrl();
	if(q >= 0) {
		AddInvalid(GetRect());
		topctrl.Remove(q);
	}
	if(top) {
		delete top;
		top = NULL;
	}
	isopen = false;
	TopWindow *win = dynamic_cast<TopWindow *>(this);
	if(win)
		win->DestroyFrame();
}

void Ctrl::WndDestroy()
{
	DestroyWnd();
	if(topctrl.GetCount())
		topctrl.Top()->ActivateWnd();
}

void Ctrl::PutForeground()
{
	int q = FindTopCtrl();
	if(q >= 0) {
		AddInvalid(GetRect());
		topctrl.Remove(q);
		topctrl.Add(this);
	}
	Vector< Ptr<Ctrl> > fw;
	for(int i = 0; i < topctrl.GetCount(); i++)
		if(topctrl[i] && topctrl[i]->top && topctrl[i]->top->owner_window == this && topctrl[i] != this)
			fw.Add(topctrl[i]);
	for(int i = 0; i < fw.GetCount(); i++)
		if(fw[i])
			fw[i]->PutForeground();
}

void Ctrl::SetWndForeground()
{
	GuiLock __;
	ASSERT(IsOpen());
	if(IsWndForeground())
		return;
	Ctrl *to = this;
	while(to->top && to->top->owner_window)
		to = to->top->owner_window;
	to->PutForeground();
	if(this != focusCtrl)
		ActivateWnd();
}

bool Ctrl::IsWndForeground() const
{
	GuiLock __;
	bool b = false;
	for(int i = 0; i < topctrl.GetCount(); i++) {
		const TopWindow *tw = dynamic_cast<const TopWindow *>(topctrl[i]);
		if(tw)
			b = tw == this;
	}
	return b;
}

void Ctrl::WndEnable(bool)
{
	GuiLock __;
}

bool Ctrl::SetWndFocus()
{
	GuiLock __;
	return true;
}

bool Ctrl::HasWndFocus() const
{
	GuiLock __;
	return focusCtrl && focusCtrl->GetTopCtrl() == this;
}

bool Ctrl::SetWndCapture()
{
	GuiLock __;
	ASSERT(IsMainThread());
	return true;
}

bool Ctrl::ReleaseWndCapture()
{
	GuiLock __;
	ASSERT(IsMainThread());
	return true;
}

bool Ctrl::HasWndCapture() const
{
	GuiLock __;
	return captureCtrl && captureCtrl->GetTopCtrl() == this;
}

void Ctrl::WndInvalidateRect(const Rect& r)
{
	GuiLock __;
	int q = FindTopCtrl();
	if(q >= 0)
		AddInvalid(r + topctrl[q]->GetRect().TopLeft());
	else
		AddInvalid(r);
}

void Ctrl::WndSetPos(const Rect& rect)
{
	GuiLock __;
	TopWindow *w = dynamic_cast<TopWindow *>(this);
	if(w)
		w->SyncFrameRect(rect);
	invalid.Add(GetRect());
	SetWndRect(rect);
	invalid.Add(rect);
}

void  Ctrl::WndScrollView(const Rect& r, int dx, int dy)
{
	GuiLock __;
	if(dx == 0 && dy == 0)
		return;
	if(dx && dy) {
		Refresh(r);
		return;
	}
	RemoveCursor();
	RemoveCaret();
	Rect sr = r.Offseted(GetScreenRect().TopLeft());
	Vector<Rect> pr = Intersection(GetPaintRects(), sr);
	for(int i = 0; i < pr.GetCount(); i++) {
		Rect r = pr[i];
		if(dx) {
			int n = r.GetWidth() - abs(dx);
			if(n > 0) {
				int to = r.left + dx * (dx > 0);
				int from = r.left - dx * (dx < 0);
				for(int y = r.top; y < r.bottom; y++)
					memmove(framebuffer[y] + to, framebuffer[y] + from, n * sizeof(RGBA));
			}
			n = min(abs(dx), r.GetWidth());	
			Refresh(dx < 0 ? r.left : r.right - n, r.top, n, r.GetHeight());
		}
		else {
			int n = r.GetHeight() - abs(dy);
			for(int y = 0; y < n; y++)
				memmove(framebuffer[dy < 0 ? r.top + y : r.bottom - 1 - y] + r.left,
				        framebuffer[dy < 0 ? r.top + y - dy : r.bottom - 1 - y - dy] + r.left,
				        r.GetWidth() * sizeof(RGBA));
			n = min(abs(dy), r.GetHeight());	
			Refresh(r.left, dy < 0 ? r.bottom - n : r.top, r.GetWidth(), n);
		}
	}

	Vector<Rect> ur;
	for(int i = 0; i < invalid.GetCount(); i++)
		if(invalid[i].Intersects(sr))
			ur.Add(invalid[i]);
	for(int i = 0; i < ur.GetCount(); i++)
		AddInvalid(ur[i].Offseted(dx, dy));
}

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	ASSERT(!IsChild() && !IsOpen() && FindTopCtrl() < 0);
	NewTop();
	if(owner) {
		Ctrl *owner_window = owner->GetTopWindow();
		if(!owner_window)
			owner_window = owner->GetTopCtrl();
		ASSERT(owner_window->IsOpen());
		if(owner_window != desktop) {
			owner_window->SetForeground();
			top->owner_window = owner_window;
		}
	}
	topctrl.Add(this);
	popup = isopen = true;
	RefreshLayoutDeep();
	if(activate) SetFocusWnd();
	AddInvalid(GetRect());
}

Rect Ctrl::GetDefaultWindowRect() {
	GuiLock __;
	static int ii = 0;
	Size sz = framebuffer.GetSize();
	Rect rect = framebuffer.GetSize();
	rect.Deflate(sz / 10);
	rect.Offset(Size(GetStdFontCy(), 2 * GetStdFontCy()) * (++ii % 8));
	return rect;
}

Vector<WString> SplitCmdLine__(const char *cmd)
{
	Vector<WString> out;
	while(*cmd)
		if((byte)*cmd <= ' ')
			cmd++;
		else if(*cmd == '\"') {
			WString quoted;
			while(*++cmd && (*cmd != '\"' || *++cmd == '\"'))
				quoted.Cat(FromSystemCharset(String(cmd, 1)).ToWString());
			out.Add(quoted);
		}
		else {
			const char *begin = cmd;
			while((byte)*cmd > ' ')
				cmd++;
			out.Add(String(begin, cmd).ToWString());
		}
	return out;
}

void Ctrl::InstallPanicBox()
{
}

void Ctrl::SysEndLoop()
{
}

END_UPP_NAMESPACE

#endif
