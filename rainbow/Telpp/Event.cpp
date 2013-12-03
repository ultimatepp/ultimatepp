#include "Local.h"

#include "Telpp.brc"


#ifdef GUI_TELPP

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)
#define LDUMP(x) // DDUMP(x)
#define LTIMING(x)

static Point MousePos;

int Width = 1000;
int Height = 1000;

TcpSocket   server;
TcpSocket   socket;

One<Ctrl>   desktop_ctrl;

void Ctrl::InitTelpp()
{
	if(!server.Listen(8000, 10)) {
		LOG("Cannot open server port for listening\r\n");
		return;
	}

	Ctrl::GlobalBackBuffer();
	Ctrl::InitTimer();

#ifdef PLATFORM_POSIX
	SetStdFont(ScreenSans(12)); //FIXME general handling
#endif
	ChStdSkin();

	static StaticRect x;
	x.Color(Cyan());
	x.SetRect(0, 0, Width, Height);
	SetDesktop(x);
}

Point GetMousePos() {
	return MousePos;
}

dword mouseb = 0;
dword modkeys = 0;

enum KM {
	KM_NONE  = 0x00,

	KM_LSHIFT= 0x01,
	KM_RSHIFT= 0x02,
	KM_LCTRL = 0x04,
	KM_RCTRL = 0x08,
	KM_LALT  = 0x10,
	KM_RALT  = 0x20,

	KM_CAPS  = 0x40,
	KM_NUM   = 0x80,
	
	KM_CTRL = KM_LCTRL | KM_RCTRL,
	KM_SHIFT = KM_LSHIFT | KM_RSHIFT,
	KM_ALT = KM_LALT | KM_RALT,
};

bool GetMouseLeft()   { return mouseb & (1<<0); }
bool GetMouseRight()  { return mouseb & (1<<1); }
bool GetMouseMiddle() { return mouseb & (1<<2); }
bool GetShift()       { return modkeys & KM_SHIFT; }
bool GetCtrl()        { return modkeys & KM_CTRL; }
bool GetAlt()         { return modkeys & KM_ALT; }
bool GetCapsLock()    { return modkeys & KM_CAPS; }

dword fbKEYtoK(dword chr) {
	return chr + K_DELTA;
/*
	if(chr == SDLK_TAB)
		chr = K_TAB;
	else
	if(chr == SDLK_SPACE)
		chr = K_SPACE;
	else
	if(chr == SDLK_RETURN)
		chr = K_RETURN;
	else
		chr = chr + K_DELTA;
	if(chr == K_ALT_KEY || chr == K_CTRL_KEY || chr == K_SHIFT_KEY)
		return chr;
	if(GetCtrl()) chr |= K_CTRL;
	if(GetAlt()) chr |= K_ALT;
	if(GetShift()) chr |= K_SHIFT;
	return chr;
*/
}

dword lastbdowntime[8] = {0};
dword isdblclick[8] = {0};

void Ctrl::MouseEventFB(Ptr<Ctrl> t, int event, Point p, int zdelta)
{
	if(!t->IsEnabled())
		return;
	Rect rr = t->GetRect();
	if((event & Ctrl::ACTION) == DOWN) {
		Ptr<Ctrl> q = t;
		TopWindowFrame *wf = dynamic_cast<TopWindowFrame *>(~t);
		if(wf)
			q = wf->window;			
		if(q) q->ClickActivateWnd();
		if(q) q->SetForeground();
		if(ignoreclick)
			return;
	}
	if(t)
		t->DispatchMouse(event, p - rr.TopLeft(), zdelta);
	if(t)
		t->PostInput();
}

Ctrl *Ctrl::FindMouseTopCtrl()
{
	for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
		Ctrl *t = topctrl[i];
		if(t->GetRect().Contains(MousePos))
			return t->IsEnabled() ? t : NULL;
	}
	return desktop->IsEnabled() ? desktop : NULL;
}

void Ctrl::DoMouseFB(int event, Point p, int zdelta)
{
	MousePos = p;
	int a = event & Ctrl::ACTION;
	if(a == Ctrl::UP && Ctrl::ignoreclick) {
		EndIgnore();
		return;
	}
	else
	if(a == Ctrl::DOWN && ignoreclick)
		return;
	LLOG("### Mouse event: " << event << " position " << p << " zdelta " << zdelta << ", capture " << Upp::Name(captureCtrl));
	if(captureCtrl)
		MouseEventFB(captureCtrl->GetTopCtrl(), event, p, zdelta);
	else
		for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
			Ptr<Ctrl> t = topctrl[i];
			Rect rr = t->GetRect();
			if(rr.Contains(p)) {
				MouseEventFB(t, event, p, zdelta);
				return;
			}
		}
	Ctrl *desktop = GetDesktop();
	if(desktop) {
		desktop->DispatchMouse(event, p, zdelta);
		desktop->PostInput();
	}
}

bool Ctrl::DoKeyFB(dword key, int cnt)
{
	DLOG("DoKeyFB " << GetKeyDesc(key) << ", " << cnt);

	bool b = DispatchKey(key, cnt);
	SyncCaret();
	Ctrl *desktop = GetDesktop();
	if(desktop)
		desktop->PostInput();
	return b;
}

void Ctrl::SetCaret(int x, int y, int cx, int cy)
{
	GuiLock __;
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	fbCaretTm = GetTickCount();
	SyncCaret();
}

void Ctrl::SyncCaret()
{
	CursorSync();
}

void Ctrl::CursorSync()
{
//	LLOG("@ CursorSync");
	Point p = GetMousePos() - fbCursorImage.GetHotSpot();
	Rect cr = Null;
	if(focusCtrl && (((GetTickCount() - fbCaretTm) / 500) & 1) == 0)
		cr = (RectC(focusCtrl->caretx, focusCtrl->carety, focusCtrl->caretcx, focusCtrl->caretcy)
		      + focusCtrl->GetScreenView().TopLeft()) & focusCtrl->GetScreenView();
	if(fbCursorPos != p && !SystemCursor || cr != fbCaretRect) {
		fbCaretRect = cr;
		fbCursorPos = p;
		Invalidate();
	}
}

String content;

void Ctrl::PaintScene(SystemDraw& draw)
{
	if(!desktop)
		return;
	LLOG("@ DoPaint");
	LTIMING("DoPaint paint");
	draw.Init(Size(Width, Height));
	draw.Begin();
	Vector<Rect> invalid;
	invalid.Add(Size(Width, Height));
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
			content << String(draw.result);
		}
	}
}

void Ctrl::DoMouseButton(int event, CParser& p)
{
	int button = p.ReadInt();
	int x = p.ReadInt();
	int y = p.ReadInt();
	DoMouseFB(decode(button, 0, LEFT, 2, RIGHT, MIDDLE)|event, Point(x, y));
}

bool Ctrl::ProcessEventQueue(const String& event_queue)
{
	StringStream ss(event_queue);
	while(!ss.IsEof()) {
		String s = ss.GetLine();
		CParser p(s);
		try {
			if(p.Id("I"))
				SystemDraw::ResetI();
			else
			if(p.Id("M")) {
				int x = p.ReadInt();
				int y = p.ReadInt();
				DoMouseFB(MOUSEMOVE, Point(x, y), 0);
			}
			else
			if(p.Id("D"))
				DoMouseButton(DOWN, p);
			else
			if(p.Id("U"))
				DoMouseButton(UP, p);
			else
			if(p.Id("K")) {
				int code = p.ReadInt();
				int which = p.ReadInt();
				DoKeyFB(which + K_DELTA, 1);
			}
			else
			if(p.Id("k")) {
				int code = p.ReadInt();
				int which = p.ReadInt();
				DoKeyFB(K_KEYUP|(which + K_DELTA), 1);
			}
			else
			if(p.Id("C")) {
				int code = p.ReadInt();
				int which = p.ReadInt();
				DoKeyFB(which, 1);
			}
		}
		catch(CParser::Error) {}
	}
	return true;
}

bool Ctrl::IsWaitingEvent()
{
//	LLOG("IsWaitingEvent");
	if(socket.IsOpen())
		return true;
	return socket.Timeout(0).Accept(server);
}

bool Ctrl::ProcessEvents(bool *quit)
{
	LLOG("ProcessEvents");
	if(!socket.IsOpen()) {
		if(!IsWaitingEvent())
			return false;
	}

	HttpHeader http;
	TimeStop tm;
	LLOG("Trying to read socket");
	socket.Timeout(20000);
	if(!http.Read(socket)) {
		LLOG("Header failed to read");
		socket.Close();
		return false;
	}
	String event_queue = socket.Get((int)http.GetContentLength());
	if(event_queue.GetCount())
		LOG(event_queue);
	content.Clear();
	bool r = ProcessEventQueue(event_queue);
	_TODO_ // Resolve eventloop exit issue
	TimerProc(GetTickCount());
	DefferedFocusSync();
	SyncCaret();
	SyncTopWindows();
	SweepMkImageCache();
	DoPaint();

	if(http.GetURI().GetCount() < 2)
		HttpResponse(socket, http.scgi, 200, "OK", "text/html", String(telpp_html, telpp_html_length));
	else	
		HttpResponse(socket, http.scgi, 200, "OK", "text/plain; charset=x-user-defined", content);

	socket.Close();
	return r;
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
//		LDUMP(loopno);
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
//	LLOG("GuiSleep");
	int level = LeaveGuiMutexAll();
	Sleep(ms); _TODO_
	EnterGuiMutex(level);
}

END_UPP_NAMESPACE

#endif
