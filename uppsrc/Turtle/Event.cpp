#include "Turtle.h"

#define LLOG(x)     // RLOG(x)
#define LDUMP(x)    // RDUMP(x)
#define LTIMING(x)

namespace Upp {

static bool sQuit; // Ctrl::IsEndSession() would be much better to use had it been implemented.
static BiVector<String> sEventQueue;

Point ReadPoint(CParser& p)
{
	Point pt;
	pt.x = p.ReadInt();
	pt.y = p.ReadInt();
	return pt;
}

bool TurtleServer::ProcessEvent(bool *quit)
{
	if(!IsWaitingEvent())
		return false;

	while(sEventQueue.GetCount() >= 2
	  && *sEventQueue[0] == 'M'
	  && *sEventQueue[1] == 'M')
		sEventQueue.DropHead(); // MouseMove compression

	String event = sEventQueue[0];
	sEventQueue.DropHead();
	
	LLOG("Processing event " << event);
	
	CParser p(event);
	
	try
	{
		if(p.Id("i"))
		{
			ResetImageCache();
		}
		else
		if(p.Id("R"))
		{
			Resize(p);
		}
		else
		if(p.Id("M"))
		{
			MouseMove(p);
		}
		else
		if(p.Id("W"))
		{
			MouseWheel(p);
		}
		else
		if(p.Id("I"))
		{
			mousein = true;
		}
		else
		if(p.Id("O"))
		{
			mousebuttons = 0;
			mousein = false;
		}
		else
		if(p.Id("D"))
		{
			MouseButton(Ctrl::DOWN, p);
		}
		else
		if(p.Id("U"))
		{
			MouseButton(Ctrl::UP, p);
		}
		else
		if(p.Id("K"))
		{
			KeyDown(event, p);
		}
		else
		if(p.Id("k"))
		{
			KeyUp(event, p);
		}
		else
		if(p.Id("C"))
		{
			KeyPress(event, p);
		}
	}
	catch(const CParser::Error& e)
	{
		LLOG("ProcessEvent() -> Parser error");
	}
	
	return true;
}

void TurtleServer::WaitEvent(int ms)
{
	websocket.Do();
	SocketWaitEvent we;
	websocket.AddTo(we);
	we.Wait(ms);
}

bool TurtleServer::IsWaitingEvent()
{
	websocket.Do();

	String s = websocket.Receive();

	if(websocket.IsClosed()) {
		Ctrl::EndSession();
		sQuit = true; // Ugly..
		return false;
	}

	if(s.GetCount() == 0)
		return sEventQueue.GetCount();
	
	LLOG("Received data " << s);

	StringStream ss(s);
	while(!ss.IsEof()) {
		String s = ss.GetLine();
		CParser p(s);
		try
		{
			if(p.Id("S")) {
				uint32 l = p.ReadNumber();
				uint32 h = p.ReadNumber();
				recieved_update_serial = MAKEQWORD(l, h);
				stat_client_ms = p.ReadNumber();
			}
			else
				sEventQueue.AddTail(s);
		}
		catch(const CParser::Error& e)
		{
			LLOG("IsWaitingEvent() -> Parser error. " << e);
		}
	}
	if(recieved_update_serial == serial_0) {
		serial_0 = 0;
		stat_roundtrip_ms = msecs() - serial_time0;
		serial_time0 = Null;
	}

	if(websocket.IsError())
		LLOG("ERROR: " << websocket.GetErrorDesc());
	
	return sEventQueue.GetCount();
}

void TurtleServer::SyncClient()
{
	while(recieved_update_serial < update_serial && !sQuit) {
		Ctrl::GuiSleep(10);
		IsWaitingEvent();
	}
}

void TurtleServer::MouseButton(dword event, CParser& p)
{
	auto MaxDistance = [](Point a, Point b) -> int
	{
		return IsNull(a) ? INT_MAX : max(abs(a.x - b.x), abs(a.y - b.y));
	};

	static int64 sMouseDownTime;
	static Point sMouseDownPos;

	int   bt = p.ReadInt();
	Point pt = ReadPoint(p);
	int64 tm = p.ReadInt64();

	dword down = (dword) event == Ctrl::DOWN;
	dword bt2 = decode(bt, 0, (1 << 0), 2, (1 << 1), (1 << 2));
	mousebuttons = (mousebuttons & ~bt2) | (-down & bt2); // Toggles button flags.

	if(event == Ctrl::DOWN) {
		if(MaxDistance(sMouseDownPos, pt) < GUI_DragDistance() && tm - sMouseDownTime < 800) {
			event = Ctrl::DOUBLE;
			sMouseDownTime = 0;
		}
		else {
			sMouseDownPos = pt;
			sMouseDownTime = tm;
		}
	}

	ReadModifierKeys(p);
	Ctrl::DoMouseFB(decode(bt, 0, Ctrl::LEFT, 2, Ctrl::RIGHT, Ctrl::MIDDLE) | event, pt, 0);
}

void TurtleServer::MouseWheel(CParser& p)
{
	double w = p.ReadDouble();
	Point pt = ReadPoint(p);
	ReadModifierKeys(p);
	Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, pt, w < 0 ? 120 : -120);
}

void TurtleServer::MouseMove(CParser& p)
{
	Point pt = ReadPoint(p);
	ReadModifierKeys(p);
	Ctrl::DoMouseFB(Ctrl::MOUSEMOVE, pt, 0);
}

void TurtleServer::KeyDown(const String& event, CParser& p)
{
	int count = 1;
	int code  = p.ReadInt();
	int which = p.ReadInt();
	
	for(;;) {
		if(sEventQueue.GetCount()
		&& sEventQueue[0] == event) { // Chrome autorepeat
			sEventQueue.DropHead();
			count++;
		}
		else
		if(sEventQueue.GetCount() >= 2
		&& *sEventQueue[0] == 'C'
		&&  sEventQueue[1] == event) { // Firefox autorepeat
			String h = sEventQueue[0];
			sEventQueue.DropHead();
			sEventQueue.DropHead();
			sEventQueue.AddHead(h);
			count++;
		}
		else
			break;
	}

	ReadModifierKeys(p);
	Ctrl::DoKeyFB(TranslateWebKeyToK(which), count);
}

void TurtleServer::KeyUp(const String& event, CParser& p)
{
	int code  = p.ReadInt();
	int which = p.ReadInt();

	ReadModifierKeys(p);
	Ctrl::DoKeyFB(TranslateWebKeyToK(which) | K_KEYUP, 1);
}

void TurtleServer::KeyPress(const String& event, CParser& p)
{
	int code  = p.ReadInt();
	int which = p.ReadInt();

	ReadModifierKeys(p);

	while(sEventQueue.GetCount() && sEventQueue[0] == event) // 'K_'s are not there anymore
		sEventQueue.DropHead();

	if(which && !GetAlt() && !GetCtrl() && findarg(which, 0x09, 0x0D, 0x20) < 0)
		Ctrl::DoKeyFB(which, 1);
}

void TurtleServer::Resize(CParser& p)
{
	desktopsize = ReadPoint(p);
	SetCanvasSize(desktopsize);
	Ctrl::SetDesktopSize(desktopsize);
}

void TurtleServer::SetMouseCursor(const Image& image)
{
	int64 q = image.GetAuxData();
	if(q) {
		Put8(STD_CURSORIMAGE);
		Put8(clamp((int)q, 1, 16));
	}
	else {
		Point pt = image.GetHotSpot();
		String h;
		h << "url('data:image/png;base64,"
		  << Base64Encode(PNGEncoder().SaveString(image))
		  << "') " << pt.x << ' ' << pt.y << ", default";
		Put8(SETCURSORIMAGE);
		Put16(0); // TODO: Cursor cache
		Put(h);
		Put8(MOUSECURSOR);
		Put16(0); // TODO: Cursor cache
	}
}
}
