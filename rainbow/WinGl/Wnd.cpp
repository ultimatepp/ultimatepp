#include "TopFrame.h"
#include "ControlPanel.h"

#ifdef GUI_WINGL

NAMESPACE_UPP

#define LLOG(x)

Ptr<Ctrl>      Ctrl::desktop;
Vector<Ctrl *> Ctrl::topctrl;
InfoPanel      Ctrl::infoPanel;
float          Ctrl::angle = 0.f;
float          Ctrl::scale = 1.f;
float          Ctrl::alpha = 255.f;
Rect           Ctrl::screenRect;
Point          Ctrl::glCursorPos = Null;
Image          Ctrl::glCursorImage;
Rect           Ctrl::glCaretRect;
int            Ctrl::glCaretTm;
int64          Ctrl::glEventLoop = 0;
int64          Ctrl::glEndSessionLoop = 0;

void Ctrl::SetDesktop(Ctrl& q)
{
	desktop = &q;
	desktop->SetRect(screenRect.GetSize());
	desktop->SetOpen(true);
	desktop->StateH(OPEN);
	desktop->NewTop();
}

void Ctrl::SetWindowSize(Size sz)
{
	screenRect = sz;
	DUMP(screenRect);
	if(desktop)
		desktop->SetRect(screenRect);
	SyncTopWindows();
}

void Ctrl::InitGl()
{
	Ctrl::GlobalBackBuffer();
	Ctrl::InitTimer();
}

void Ctrl::EndSession()
{
	GuiLock __;
	glEndSession = true;
	glEndSessionLoop = glEventLoop;
}
void Ctrl::ExitGl()
{
	TopWindow::ShutdownWindows();
	Ctrl::CloseTopCtrls();
	if(glEndSession)
		GlQuitSession();
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
		return dynamic_cast<TopWindowFrame *>(x) ? x->GetOwner() : x;
	}
	return NULL;
}

Ctrl *Ctrl::GetActiveCtrl()
{
	GuiLock __;
	if(focusCtrl == &infoPanel)
		return desktop;
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
	return GlIsWaitingEvent();
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
	ASSERT(IsMainThread());
	if(DoCall()) {
		SyncTopWindows();
		return false;
	}
	if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
		ReleaseCtrlCapture();
	if(GlProcessEvent(quit)) {
		SyncTopWindows();
		DefferedFocusSync();
		SyncCaret();
		return true;
	}
	return false;
}

void Ctrl::DrawScreen()
{
	if(desktop && !painting) {
		painting = true;
		//desktop->SyncLayout(2);
		Rect clip = desktop->GetRect();
		SystemDraw draw(clip.GetSize());
		infoPanel.Init(*desktop);
		draw.alpha = infoPanel.GetAlpha();
		draw.angle = infoPanel.GetAngle();
		draw.scale = infoPanel.GetScale();
		draw.FlatView();
		draw.Clear();
		desktop->ApplyTransform(TS_BEFORE_PAINT);
		desktop->CtrlPaint(draw, clip);
		for(int i = 0; i < topctrl.GetCount(); i++) {
			if(topctrl[i] == &infoPanel)
				continue;
			Rect r = topctrl[i]->GetRect();
			draw.Clipoff(r);
			topctrl[i]->CtrlPaint(draw, clip);
			draw.End();
		}
		CursorSync(draw);
		desktop->ApplyTransform(TS_AFTER_PAINT);		
		glLoadIdentity();
		#if CLIP_MODE == 2
		glDisable(GL_STENCIL_TEST);
		#endif
		draw.alpha = 255.f;
		draw.Offset(infoPanel.GetRect().TopLeft());
		infoPanel.CtrlPaint(draw, clip, NULL);
		draw.End();
		#if CLIP_MODE == 2
		glEnable(GL_STENCIL_TEST);
		#endif
		MouseSync(draw);		
		SwapBuffers(hDC);
		painting = false;
		LOGF("Fps %.2f\n", GetFps());
	}
}

void Ctrl::DrawLine(const Vector<Rect>& clip, int x, int y, int cx, int cy, bool horz, const byte *pattern, int animation)
{
	if(cx <= 0 || cy <= 0)
		return;
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
	Vector<Rect> ir;
	ir.Add(screenRect);
	Vector<Rect> pr = Intersection(ir, clip.Offseted(p));
	const byte *pattern = type == DRAWDRAGRECT_DASHED ? dashed :
	                      type == DRAWDRAGRECT_NORMAL ? normal : solid;
	DragRectDraw0(pr, rect1.Offseted(p), n, pattern, animation);
	DragRectDraw0(pr, rect2.Offseted(p), n, pattern, animation);
}

void Ctrl::WndUpdate0r(const Rect& r)
{
	GuiLock __;
}

bool Ctrl::ProcessEvents(bool *quit)
{
	LOGBLOCK("@ ProcessEvents");
	MemoryCheckDebug();
	if(!ProcessEvent(quit))
		return false;
	while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop()) && !GlEndSession());
	TimerProc(GetTickCount());
	SweepMkImageCache();
	if(hRC)
		DrawScreen();
	return true;
}

void Ctrl::EventLoop0(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT(IsMainThread());
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << BeginIndent);
	Ptr<Ctrl> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	bool quit = false;
	ProcessEvents(&quit);
	int64 loopno = ++glEventLoop;
	while(loopno > glEndSessionLoop && !quit && (ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount()))
	{
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
		SyncCaret();
		GuiSleep(20);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		ProcessEvents(&quit);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / after ProcessEvents");
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(EndIndent << "Leaving event loop ");
}

void Ctrl::GuiSleep0(int ms)
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("GuiSleep");
	int level = LeaveGMutexAll();
	GlSleep(ms);
	EnterGMutex(level);
}

Rect Ctrl::GetWndScreenRect() const
{
	GuiLock __;
	return GetRect();
}

void Ctrl::WndShow0(bool b)
{
	GuiLock __;
}

void Ctrl::WndUpdate0()
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
	return screenRect;
}

void Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
	//Array<Rect> r;
	rc.Add(screenRect.GetSize());
}

Rect Ctrl::GetVirtualWorkArea()
{
	return screenRect;
}

Rect Ctrl::GetWorkArea(Point pt)
{
	return screenRect;
}

Rect Ctrl::GetVirtualScreenArea()
{
	return screenRect;
}

Rect Ctrl::GetPrimaryWorkArea()
{
	return screenRect;
}

Rect Ctrl::GetPrimaryScreenArea()
{
	return screenRect;
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

void Ctrl::WndDestroy0()
{
	for(int i = 0; i < topctrl.GetCount(); i++)
		if(topctrl[i]->top && topctrl[i]->top->owner_window == this)
			topctrl[i]->WndDestroy0();
	int q = FindTopCtrl();
	if(q >= 0) {
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

void Ctrl::PutForeground()
{
	int q = FindTopCtrl();
	if(q >= 0) {
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

void Ctrl::SetWndForeground0()
{
	GuiLock __;
	ASSERT(IsOpen());
	if(top && top->owner_window && !IsWndForeground())
		top->owner_window->PutForeground();
	PutForeground();
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

void Ctrl::WndEnable0(bool *b)
{
	GuiLock __;
	*b = true;
}

void Ctrl::SetWndFocus0(bool *b)
{
	GuiLock __;
	*b = true;
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

void Ctrl::WndInvalidateRect0(const Rect& r)
{
	GuiLock __;
	//::InvalidateRect(glHwnd, NULL, false);
}

void Ctrl::WndSetPos0(const Rect& rect)
{
	GuiLock __;
	TopWindow *w = dynamic_cast<TopWindow *>(this);
	if(w)
		w->SyncFrameRect(rect);
	SetWndRect(rect);
}

void  Ctrl::WndScrollView0(const Rect& r, int dx, int dy)
{
	GuiLock __;
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
}

Rect Ctrl::GetDefaultWindowRect() {
	GuiLock __;
/*	int ii = 0;
	Size sz = screenRect.GetSize();
	Rect rect = sz;
	rect.Deflate(sz / 8);
	rect.Offset(Point(sz) / 16 * (ii % 8));
	return rect;*/
	return screenRect;
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

END_UPP_NAMESPACE

#endif
