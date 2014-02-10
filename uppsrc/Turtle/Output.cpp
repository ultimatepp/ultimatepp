#include "Local.h"

#ifdef GUI_TURTLE

NAMESPACE_UPP

#define LLOG(x)     // LLOG(x)
#define LDUMP(x)    // RDUMP(x)
#define LTIMING(x)

TurtleStream Ctrl::turtle_stream;

void Ctrl::Put16(int x)
{
	Put8(LOBYTE(x));
	Put8(HIBYTE(x));
}

void Ctrl::Put32(int x)
{
	Put16(LOWORD(x));
	Put16(HIWORD(x));
}

void Ctrl::Put(Point p)
{// TODO: Clamp?
	Put16(p.x);
	Put16(p.y);
}

void Ctrl::Put(Size sz)
{
	Put((Point)sz);
}

void Ctrl::Put(const Rect& r)
{
	Put(r.TopLeft());
	Put(r.GetSize());
}

void Ctrl::Put(const String& s)
{
	Put32(s.GetLength());
	turtle_stream.SetDataFlag();
	turtle_stream.Put(s);
}

void Turtle_PutLink(const String& link)
{
	Ctrl::Put8(Ctrl::OPENLINK);
	Ctrl::Put(link);
}

void Ctrl::Output()
{
	socket.Timeout(20000);
	if(turtle_stream.HasData()) {
		websocket.SendBinary(ZCompress(String(DISABLESENDING, 1))); // Do not send events until data transfered and processed
		int64 x = ++update_serial;
		if(IsNull(serial_time0)) {
			serial_time0 = msecs();
			serial_0 = update_serial;
		}
		Put8(UPDATESERIAL);
		Put32(LODWORD(x));
		Put32(HIDWORD(x));
		String s = turtle_stream.FlushStream();
		stat_data_send += s.GetCount();
		LLOG("Sending " << s.GetLength());
		websocket.SendBinary(s);
	}
}

void Ctrl::TimerAndPaint()
{
	LLOG("TimerAndPaint " << msecs());
	TimerProc(GetTickCount());
	DefferedFocusSync();
	SyncCaret();
	SyncTopWindows();
	SweepMkImageCache();
	DoPaint();
	Output();
}

void Ctrl::SyncCaret()
{
	Rect cr = Null;
	if(focusCtrl)
		cr = (RectC(focusCtrl->caretx, focusCtrl->carety, focusCtrl->caretcx, focusCtrl->caretcy)
		      + focusCtrl->GetScreenView().TopLeft()) & focusCtrl->GetScreenView();
	if(cr != fbCaretRect) { // TODO: SetCaret should perhaps be called on Ctrl::SetCaret
		Put8(SETCARET);
		Put(cr);
		fbCaretRect = cr;
	}
}

void Ctrl::PaintScene(SystemDraw& draw)
{
	if(!desktop)
		return;
	LLOG("@ DoPaint");
	LTIMING("DoPaint paint");
	draw.Init(invalid, DesktopSize.cy);
//	draw.Init(DesktopSize);
	draw.Begin();
	for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
		Rect r = topctrl[i]->GetRect();
		Rect ri = GetClipBound(invalid, r);
		LLOG(i << " Window " << r << ", bound " << ri);
		if(!IsNull(ri)) {
			draw.Clipoff(r);
			topctrl[i]->UpdateArea(draw, ri - r.TopLeft());
			draw.End();
			draw.ExcludeClip(r);
			Subtract(invalid, r);
		}
	}
	Rect ri = GetClipBound(invalid, DesktopSize);
	if(!IsNull(ri))
		desktop->UpdateArea(draw, ri);
	invalid.Clear();
	draw.End();
	
//	DDUMP(turtle_stream.FlushStream().GetCount()); abort();
}

void  Ctrl::SetMouseCursor(const Image& image)
{
	GuiLock __;
	if(image.GetSerialId() != fbCursorImage.GetSerialId()) {
		fbCursorImage = image;
		fbCursorPos = Null;
	}
}

void Ctrl::SyncClient()
{
	while(recieved_update_serial < update_serial) {
		GuiSleep(10);
		IsWaitingEvent();
	}
}

void Ctrl::DoPaint()
{
	if(recieved_update_serial < update_serial - 1) // Falling behind, wait
		SyncClient();
	if(invalid.GetCount() && desktop) {
		SystemDraw draw;
		PaintScene(draw);
	}
	static int64 prev_cursor_id = Null;
	int64 id = fbCursorImage.GetSerialId();
	if(id != prev_cursor_id) {
		prev_cursor_id = id;
		int64 q = fbCursorImage.GetAuxData();
		if(q) {
			Put8(STD_CURSORIMAGE);
			Put8(clamp((int)q, 1, 16));
		}
		else {
			String h;
			Point p = fbCursorImage.GetHotSpot();
			h << "url('data:image/png;base64,"
			  << Base64Encode(PNGEncoder().SaveString(fbCursorImage))
			  << "') " << p.x << ' ' << p.y << ", default";
			Put8(SETCURSORIMAGE);
			Put16(0); // _TODO_ Cursor cache
			Put(h);
			Put8(MOUSECURSOR);
			Put16(0); // _TODO_ Cursor cache
		}
	}
}

END_UPP_NAMESPACE

#endif
