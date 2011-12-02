#include "TopFrame.h"
#include "ControlPanel.h"
#include "Console.h"

#ifdef GUI_WINGL

NAMESPACE_UPP

#define LLOG(x)   DLOG(x)
#define LDUMP(x)  DDUMP(x)
#define LDUMPC(x) DDUMPC(x)

Ptr<Ctrl>      Ctrl::desktop;
Vector<Ctrl *> Ctrl::topctrl;
InfoPanel      Ctrl::infoPanel;
Console        Ctrl::console;
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
int            Ctrl::PaintLock;

void GlLog(int line, const char* text, Color ink)
{
	Ctrl::console.Log(line, text, ink);
}

void GlLog(const char* text, Color ink)
{
	Ctrl::console.Log(text, ink);
}

void GlLogF(Color ink, const char* fmt, ...)
{
	char buffer[1024];
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(buffer, fmt, argptr);
	va_end(argptr);
	GlLog(buffer, ink);
}

void GlLogF(const char* fmt, ...)
{
	char buffer[1024];
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(buffer, fmt, argptr);
	va_end(argptr);
	GlLog(buffer);
}

void Ctrl::SetDesktop(Ctrl& q)
{
	q.SetRect(screenRect.GetSize());
	q.SetOpen(true);
	q.StateH(OPEN);
	q.NewTop();
	desktop = &q;
}

void Ctrl::SetWindowSize(Size sz)
{
	screenRect = sz;
	if(desktop)
		desktop->SetRect(screenRect);
}

void Ctrl::InitGl()
{
	Ctrl::GlobalBackBuffer();
	Ctrl::InitTimer();
	ChStdSkin();
	static StaticRect x;
	x.Color(Cyan());
	SetDesktop(x);
}

void Ctrl::EndSession()
{
	GuiLock __;
	glEndSession = true;
	EndSessionLoopNo = EventLoopNo;
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
	if(focusCtrl == &infoPanel || focusCtrl == &console)
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
	else if(glDrawMode == DRAW_ON_IDLE)
		DrawScreen();
	
	return false;
}

void Ctrl::DrawScreen()
{
	if(hRC && desktop && !painting) {
		resources.BindStatic();
		int64 t0 = GetHighTickCount();
		frameInfo.curr_tick_count = t0;
		painting = true;
		desktop->ApplyTransform(TS_BEFORE_SCREEN);
		for(int i = 0; i < topctrl.GetCount(); i++) {
			topctrl[i]->ApplyTransform(TS_BEFORE_SCREEN);
		}
		Rect clip = desktop->GetRect();
		SystemDraw draw(clip.GetSize());
		infoPanel.Init(*desktop);
		infoPanel.Show(controlPanelActive);
		console.Init(*desktop);
		console.Show(consoleActive);
		draw.alpha = infoPanel.GetAlpha();
		draw.angle = infoPanel.GetAngle();
		draw.scale = infoPanel.GetScale();
		draw.FlatView();
		draw.Clear();
		desktop->ApplyTransform(TS_BEFORE_PAINT);
		desktop->CtrlPaint(draw, clip);
		for(int i = 0; i < topctrl.GetCount(); i++) {
			Ctrl* tq = topctrl[i];
			if(tq == &infoPanel || tq == &console)
				continue;
			Rect r = tq->GetRect();
			tq->ApplyTransform(TS_BEFORE_PAINT);
			if(tq->cliptobounds)
				draw.Clipoff(r);
			else
				draw.Offset(r.left, r.top);
			tq->CtrlPaint(draw, clip);
			draw.End();
			tq->ApplyTransform(TS_AFTER_PAINT);
		}
		CursorSync(draw);
			
		desktop->ApplyTransform(TS_AFTER_PAINT);		
		glLoadIdentity();
		#if CLIP_MODE == 2
		//glDisable(GL_STENCIL_TEST);
		#endif
		draw.ImageColoring();
		draw.alpha = 255.f;
		if(controlPanelActive)
		{
			draw.Offset(infoPanel.GetRect().TopLeft());
			infoPanel.CtrlPaint(draw, clip);
			draw.End();
		}
		if(consoleActive)
		{
			draw.Offset(console.GetRect().TopLeft());
			console.CtrlPaint(draw, clip);
			draw.End();
		}
		#if CLIP_MODE == 2
		//glEnable(GL_STENCIL_TEST);
		#endif
		MouseSync(draw);		
		SwapBuffers(hDC);
		painting = false;
		int64 t1 = GetHighTickCount();
		frameInfo.frame_time = t1 - t0;
		frameInfo.frame_factor = frameInfo.frame_time * frameInfo.frame_skip / 1000.f;
		frameInfo.fps = frameInfo.frame_time > 0.f ? 1000.f / (float)frameInfo.frame_time : 0.f;
		//LOGF("ft: %d, t0: %d, t1: %d\n", frameInfo.frame_time, t0, t1);
	}
}

void Ctrl::WndUpdate0r(const Rect& r)
{
	GuiLock __;
}

bool Ctrl::ProcessEvents(bool *quit)
{
	if(!ProcessEvent(quit))
		return false;
	while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop()));
	TimerProc(GetTickCount());
	SweepMkImageCache();
	if(glDrawMode == DRAW_ON_TIMER)
		DrawScreen();
	return true;
}

void Ctrl::EventLoop0(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT(IsMainThread());
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
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
		SyncCaret();
		if(glDrawMode == DRAW_ON_TIMER)
			GuiSleep(20);
		ProcessEvents(&quit);
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
}

void Ctrl::GuiSleep0(int ms)
{
	GuiLock __;
	ASSERT(IsMainThread());
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

void Ctrl::DestroyWnd()
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

void Ctrl::WndDestroy0()
{
	DestroyWnd();
	if(topctrl.GetCount())
		topctrl.Top()->ActivateWnd();
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
	StateH(OPEN);
	if(activate) SetFocusWnd();
}

Rect Ctrl::GetDefaultWindowRect() {
	GuiLock __;
	int ii = 0;
	Size sz = screenRect.GetSize();
	Rect rect = sz;
	rect.Deflate(sz / 8);
	rect.Offset(Point(sz) / 16 * (ii % 8));
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

END_UPP_NAMESPACE

#endif
