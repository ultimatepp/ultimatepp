#include "Local.h"

#ifdef GUI_SDL20GL

NAMESPACE_UPP

#define LLOG(x)     LOG(x)
#define LDUMP(x)  //DDUMP(x)
#define LDUMPC(x) //DDUMPC(x)
#define LTIMING(x)  RTIMING(x)

Ptr<Ctrl>      Ctrl::desktop;
Vector<Ctrl *> Ctrl::topctrl;

bool           Ctrl::invalid;

bool           Ctrl::sdlMouseIsIn;

Point          Ctrl::fbCursorPos = Null;
Image          Ctrl::fbCursorImage;
Rect           Ctrl::fbCaretRect;
int            Ctrl::fbCaretTm;
bool           Ctrl::fbEndSession;
int            Ctrl::PaintLock;

bool           Ctrl::SystemCursor;

void Ctrl::SetDesktop(Ctrl& q)
{
	desktop = &q;
	desktop->SetOpen(true);
	desktop->NewTop();
	invalid = true;
}

void Ctrl::InitFB()
{
	Ctrl::GlobalBackBuffer();
	Ctrl::InitTimer();

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
	if(fbEndSession) {
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}
}

void Ctrl::SetDesktopSize(Size sz)
{
	if(desktop)
		desktop->SetRect(sz);
	invalid = true;
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

int Ctrl::RegisterSystemHotKey(dword key, Function<void ()> cb)
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
	SDL_PumpEvents();
	SDL_Event events;
	return SDL_PeepEvents(&events, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) > 0;
}

void Ctrl::SyncTopWindows()
{
	for(int i = 0; i < topctrl.GetCount(); i++) {
		TopWindow *w = dynamic_cast<TopWindow *>(topctrl[i]);
		if(w)
			w->SyncRect();
	}
}

/*
ViewDraw::ViewDraw(Ctrl *ctrl)
{
	if(Ctrl::invalid)
		Ctrl::DoPaint();
	Ctrl::invalid = false;
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
	Ctrl::DoUpdate();
}
*/

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

void Ctrl::PaintScene(SystemDraw& draw)
{
	if(!desktop)
		return;
	LLOG("@ DoPaint");
	LTIMING("DoPaint paint");
	draw.Init(screen_size, (uint64)screen.glcontext);
	draw.Begin();
	Vector<Rect> invalid;
	invalid.Add(screen_size);
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
	Rect ri = GetClipBound(invalid, desktop->GetRect().GetSize());
	if(!IsNull(ri))
		desktop->UpdateArea(draw, ri);
	draw.End();
}

void Ctrl::PaintCaretCursor(SystemDraw& draw)
{
	if(!IsNull(fbCaretRect))
		draw.DrawRect(fbCaretRect, InvertColor);
	if(sdlMouseIsIn && !SystemCursor)
		draw.DrawImage(fbCursorPos.x, fbCursorPos.y, fbCursorImage);
}

void Ctrl::DoPaint()
{
	if(!PaintLock) {
		if(invalid && desktop) {
			invalid = false;
			SystemDraw draw;
			PaintScene(draw);
			PaintCaretCursor(draw);
			draw.Finish();
			SDL_GL_SwapWindow(screen.win);
		}
	}
}

void Ctrl::WndUpdate(const Rect&)
{
	GuiLock __;
	Invalidate();
	DoPaint();
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
	return GetVirtualScreenArea();
}

void Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
	Array<Rect> r;
	r.Add(GetVirtualScreenArea());
}

Rect Ctrl::GetVirtualWorkArea()
{
	return GetVirtualScreenArea();
}
/*
Rect Ctrl::GetWorkArea(Point pt)
{
	return GetVirtualScreenArea();
}
*/
Rect Ctrl::GetVirtualScreenArea()
{
	GuiLock __;
	return desktop ? desktop->GetRect() : Rect(0, 0, 0, 0);
}

Rect Ctrl::GetPrimaryWorkArea()
{
	return GetVirtualScreenArea();
}

Rect Ctrl::GetPrimaryScreenArea()
{
	return GetVirtualScreenArea();
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
		Invalidate();
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
		Invalidate();
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

void Ctrl::WndInvalidateRect(const Rect&)
{
	GuiLock __;
	Invalidate();
}

void Ctrl::WndSetPos(const Rect& rect)
{
	GuiLock __;
	TopWindow *w = dynamic_cast<TopWindow *>(this);
	if(w)
		w->SyncFrameRect(rect);
	Invalidate();
	SetWndRect(rect);
}

void  Ctrl::WndScrollView(const Rect& r, int dx, int dy)
{
	GuiLock __;
	LLOG("ScrollView " << rect);
	WndInvalidateRect(r);
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
	Invalidate();
}

Rect Ctrl::GetDefaultWindowRect() {
	GuiLock __;
	static int ii = 0;
	Rect rect = GetVirtualScreenArea();
	Size sz = rect.GetSize();
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
