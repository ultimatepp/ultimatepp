#include "Local.h"

#ifdef GUI_TURTLE

NAMESPACE_UPP

#define LLOG(x)     // LLOG(x)
#define LDUMP(x)    // RDUMP(x)
#define LTIMING(x)

int   Ctrl::serial_time0 = Null;
int64 Ctrl::serial_0;

Time  Ctrl::stat_started;
int64 Ctrl::stat_data_send;
int   Ctrl::stat_putrect;
int   Ctrl::stat_putimage;
int   Ctrl::stat_setimage;
int64 Ctrl::stat_setimage_len;
int   Ctrl::stat_roundtrip_ms;
int   Ctrl::stat_client_ms;

static Point MousePos;

Size   Ctrl::DesktopSize = Size(1000, 1000);

StaticRect& DesktopRect()
{
	static StaticRect x;
	return x;
}

Ctrl& Ctrl::Desktop()
{
	return DesktopRect();
}

BiVector<String> Ctrl::event_queue;

void Ctrl::EndSession()
{
	GuiLock __;
	Ctrl::CloseTopCtrls();
}

bool Ctrl::IsWaitingEvent()
{
	GuiLock __;
	if(quit) {
		WhenDisconnect();
		EndSession();
		Exit(0);
	}
	while(socket.Timeout(0).WaitRead()) {
		socket.Timeout(20000);
		String s = websocket.Recieve();
		if(s.IsVoid()) { // No data returned -> means EOF was reached
			WhenDisconnect();
			EndSession();
			Exit(0);
		}
		LLOG("Recieved data " << s);
		StringStream ss(s);
		while(!ss.IsEof()) {
			String s = ss.GetLine();
			CParser p(s);
			try {
				if(p.Id("S")) {
					uint32 l = p.ReadNumber();
					uint32 h = p.ReadNumber();
					recieved_update_serial = MAKEQWORD(l, h);
					stat_client_ms = p.ReadNumber();
				}
				else
					event_queue.AddTail(s);
			}
			catch(CParser::Error) {}
		}
		if(recieved_update_serial == serial_0) {
			serial_0 = 0;
			stat_roundtrip_ms = msecs() - serial_time0;
			serial_time0 = Null;
		}
	}
	socket.Timeout(20000);
	if(socket.IsError())
		LLOG("ERROR: " << socket.GetErrorDesc());
	return event_queue.GetCount();
}

void Ctrl::GuiSleep(int ms)
{
	GuiLock __;
	ASSERT(IsMainThread());
//	LLOG("GuiSleep");
	int level = LeaveGuiMutexAll();
	socket.Timeout(ms).WaitRead();
	socket.Timeout(20000);
	EnterGuiMutex(level);
}

void WakeUpGuiThread()
{
	// TODO: Implement correctly
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

bool keyShift;
bool keyCtrl;
bool keyAlt;

bool GetShift()       { return keyShift; }
bool GetCtrl()        { return keyCtrl; }
bool GetAlt()         { return keyAlt; }
bool GetCapsLock()    { return false; } // Impossible to implement

dword fbKEYtoK(dword chr) {
	if(findarg(chr, 9, 0xd) < 0)
		chr = chr + K_DELTA;
	if(chr == K_ALT_KEY || chr == K_CTRL_KEY || chr == K_SHIFT_KEY)
		return chr;
	if(GetCtrl()) chr |= K_CTRL;
	if(GetAlt()) chr |= K_ALT;
	if(GetShift()) chr |= K_SHIFT;
	return chr;
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
*/
}

void Ctrl::ReadKeyMods(CParser& p)
{
	const char *s = p.GetPtr();
	if(*s)
		keyShift = *s++ == '1';
	if(*s)
		keyCtrl = *s++ == '1';
	if(*s)
		keyAlt = *s++ == '1';
}

bool Ctrl::DoKeyFB(dword key, int cnt)
{
	LLOG("DoKeyFB [" << GetKeyDesc(key) << "] " << key << ", " << cnt);

	bool b = DispatchKey(key, cnt);
	SyncCaret();
	Ctrl *desktop = GetDesktop();
	if(desktop)
		desktop->PostInput();
	return b;
}


bool  mouseLeft, mouseMiddle, mouseRight;
Point mouseDownPos;
int64 mouseDownTime;

bool GetMouseLeft()   { return mouseLeft; }
bool GetMouseRight()  { return mouseRight; }
bool GetMouseMiddle() { return mouseMiddle; }

void Ctrl::DoMouseFB(int event, Point p, int zdelta, CParser& cp)
{
	ReadKeyMods(cp);
	MousePos = p;
	int a = event & ACTION;
	if(a == UP && Ctrl::ignoreclick) {
		EndIgnore();
		return;
	}
	else
	if(a == DOWN && ignoreclick)
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

static int sDistMax(Point a, Point b)
{
	return IsNull(a) ? INT_MAX : max(abs(a.x - b.x), abs(a.y - b.y));
}

Point ReadPoint(CParser& p)
{
	Point pt;
	pt.x = p.ReadInt();
	pt.y = p.ReadInt();
	return pt;
}

void Ctrl::DoMouseButton(int event, CParser& p)
{
	int button = p.ReadInt();
	Point pt = ReadPoint(p);
	int64 tm = p.ReadInt64();
	(button == 0 ? mouseLeft : button == 2 ? mouseRight : mouseMiddle) = event == DOWN;
	if(event == DOWN) {
		if(ignoremouseup) {
			KillRepeat();
			ignoreclick = false;
			ignoremouseup = false;
		}
		if(ignoreclick)
			return;
	}
	if(event == UP) {
		if(ignoreclick)
			EndIgnore();
	}
	if(event == DOWN)
		if(sDistMax(mouseDownPos, pt) < GUI_DragDistance() && tm - mouseDownTime < 800) {
			event = DOUBLE;
			mouseDownTime = 0;
		}
		else {
			mouseDownPos = pt;
			mouseDownTime = tm;
		}
	DoMouseFB(decode(button, 0, LEFT, 2, RIGHT, MIDDLE)|event, pt, 0, p);
}

bool Ctrl::ProcessEvent(const String& event)
{
	LLOG("Processing event " << event);
	CParser p(event);
	try {
		if(p.Id("I"))
			SystemDraw::ResetI();
		else
		if(p.Id("R")) {
			DesktopSize = ReadPoint(p);
			Desktop().SetRect(0, 0, DesktopSize.cx, DesktopSize.cy);				
		}
		if(p.Id("M")) {
			Point pt = ReadPoint(p);
			int64 tm = p.ReadInt64();
			DoMouseFB(MOUSEMOVE, pt, 0, p);
		}
		else
		if(p.Id("W")) {
			double w = p.ReadDouble();
			Point pt = ReadPoint(p);
			int64 tm = p.ReadInt64();
			DoMouseFB(MOUSEWHEEL, pt, w < 0 ? 120 : -120, p);
		}
		else
		if(p.Id("O")) {
			mouseLeft = mouseMiddle = mouseRight = false;
			mouseDownTime = 0;
		}
		else
		if(p.Id("D")) {
			DoMouseButton(DOWN, p);
		}
		else
		if(p.Id("U")) {
			DoMouseButton(UP, p);
		}
		else
		if(p.Id("K")) {
			int code = p.ReadInt();
			int which = p.ReadInt();
			int count = 1;
			for(;;) {
				if(event_queue.GetCount() && event_queue[0] == event) { // Chrome autorepeat
					event_queue.DropHead();
					count++;
				}
				else
				if(event_queue.GetCount() >= 2 && *event_queue[0] == 'C' && event_queue[1] == event) { // Firefox autorepeat
					String h = event_queue[0];
					event_queue.DropHead();
					event_queue.DropHead();
					event_queue.AddHead(h);
					count++;
				}
				else
					break;
			}
			ReadKeyMods(p);
			DoKeyFB(fbKEYtoK(which), count);
		}
		else
		if(p.Id("k")) {
			int code = p.ReadInt();
			int which = p.ReadInt();
			ReadKeyMods(p);
			DoKeyFB(K_KEYUP|fbKEYtoK(which), 1);
		}
		else
		if(p.Id("C")) {
			int code = p.ReadInt();
			int which = p.ReadInt();
			ReadKeyMods(p);
			int count = 1;
			while(event_queue.GetCount() && event_queue[0] == event) { // 'K's are not there anymore
				event_queue.DropHead();
				count++;
			}
			if(which && !keyAlt && !keyCtrl && findarg(which, 9, 0xd) < 0)
				DoKeyFB(which, 1);
		}
	}
	catch(CParser::Error) {}
	return true;
}

bool Ctrl::ProcessEvents(bool *quit)
{
	GuiLock __;
	LLOG("---- Process events");
	bool r = false;
	while(IsWaitingEvent()) {
		while(event_queue.GetCount() >= 2 && *event_queue[0] == 'M' && *event_queue[1] == 'M')
			event_queue.DropHead(); // MouseMove compression
		String ev = event_queue[0];
		event_queue.DropHead();
		ProcessEvent(ev);
		r = true;
	}
	TimerAndPaint();
	return r;
}

Point GetMousePos() {
	return MousePos;
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

END_UPP_NAMESPACE

#endif
