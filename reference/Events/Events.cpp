#include <CtrlLib/CtrlLib.h>

using namespace Upp;

String GetKeyDescEx(int key)
{
	String desc = GetKeyDesc(key & ~K_KEYUP);
	if(key & K_KEYUP)
		desc << " UP";
	return desc;
}

struct App : TopWindow {
	ArrayCtrl  log;

	void Log(const String& s)
	{
		log.Add(s);
		log.GoEnd();
	}

	virtual void   Activate()
	{
		Log("Activate");
		SetFocus();
	}

	virtual void   Deactivate()
	{
		Log("Deactivate");
	}

	virtual Image  FrameMouseEvent(int event, Point p, int zdelta, dword keyflags)
	{
		if(event != CURSORIMAGE)
			Log(Format("FrameMouseEvent(%d, Point(%d, %d), %d, %x)",
			           event, p.x, p.y, zdelta, (int)keyflags));
		return TopWindow::FrameMouseEvent(event, p, zdelta, keyflags);
	}

	virtual Image  MouseEvent(int event, Point p, int zdelta, dword keyflags)
	{
		if(event != CURSORIMAGE)
			Log(Format("MouseEvent(%d, Point(%d, %d), %d, %x)",
			           event, p.x, p.y, zdelta, (int)keyflags));
		return TopWindow::MouseEvent(event, p, zdelta, keyflags);
	}

	virtual void   MouseEnter(Point p, dword keyflags)
	{
		Log(Format("MouseEnter(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   MouseMove(Point p, dword keyflags)
	{
		Log(Format("MouseMove(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   LeftDown(Point p, dword keyflags)
	{
		Log(Format("LeftDown(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
		SetFocus();
	}

	virtual void   LeftDouble(Point p, dword keyflags)
	{
		Log(Format("LeftDouble(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void LeftTriple(Point p, dword keyflags)
	{
		Log(Format("LeftTriple(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}
	
	virtual void LeftDrag(Point p, dword keyflags) {
		Log(Format("LeftDrag(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}
	
	virtual void LeftHold(Point p, dword keyflags) {
		Log(Format("LeftHold(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   LeftRepeat(Point p, dword keyflags)
	{
		Log(Format("LeftRepeat(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   LeftUp(Point p, dword keyflags)
	{
		Log(Format("LeftUp(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   RightDown(Point p, dword keyflags)
	{
		Log(Format("RightDown(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   RightDouble(Point p, dword keyflags)
	{
		Log(Format("RightDouble(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void RightTriple(Point p, dword keyflags)
	{
		Log(Format("RightTriple(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}
	
	virtual void RightDrag(Point p, dword keyflags) {
		Log(Format("RightDrag(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}
	
	virtual void RightHold(Point p, dword keyflags) {
		Log(Format("RightHold(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   RightRepeat(Point p, dword keyflags)
	{
		Log(Format("RightRepeat(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   RightUp(Point p, dword keyflags)
	{
		Log(Format("RightUp(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   MiddleDown(Point p, dword keyflags)
	{
		Log(Format("MiddleDown(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   MiddleDouble(Point p, dword keyflags)
	{
		Log(Format("MiddleDouble(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void MiddleTriple(Point p, dword keyflags)
	{
		Log(Format("MiddleTriple(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}
	
	virtual void MiddleDrag(Point p, dword keyflags) {
		Log(Format("MiddleDrag(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}
	
	virtual void MiddleHold(Point p, dword keyflags) {
		Log(Format("MiddleHold(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   MiddleRepeat(Point p, dword keyflags)
	{
		Log(Format("MiddleRepeat(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   MiddleUp(Point p, dword keyflags)
	{
		Log(Format("MiddleUp(Point(%d, %d), %x)", p.x, p.y, (int)keyflags));
	}

	virtual void   MouseWheel(Point p, int zdelta, dword keyflags)
	{
		Log(Format("MouseWheel(Point(%d, %d), %d, %x)", p.x, p.y, zdelta, (int)keyflags));
	}

	virtual void   MouseWheelH(Point p, int zdelta, dword keyflags)
	{
		Log(Format("MouseWheelH(Point(%d, %d), %d, %x)", p.x, p.y, zdelta, (int)keyflags));
	}

	virtual void   MouseLeave()
	{
		Log("MouseLeave");
	}

	virtual Image  CursorImage(Point p, dword keyflags)
	{
		return keyflags & K_CTRL ? CtrlImg::swap_color_cursor() : Image::Arrow();
	}

	virtual bool   Key(dword key, int count)
	{
		Log(Format("Key(%x, %d) ", (int)key, count) + GetKeyDescEx(key));
		return false;
	}

	virtual void   GotFocus()
	{
		Log("GotFocus");
	}

	virtual void   LostFocus()
	{
		Log("LostFocus");
	}

	virtual bool   HotKey(dword key)
	{
		Log(Format("HotKey(%x) ", (int)key) + GetKeyDescEx(key));
		return false;
	}

	virtual void   ChildGotFocus()
	{
		Log("ChildGotFocus");
	}

	virtual void   ChildLostFocus()
	{
		Log("ChildLostFocus");
	}

	virtual void   Layout()
	{
		Log("Layout");
	}

	App()
	{
		SetFrame(InsetFrame());
		AddFrame(InsetFrame());
		AddFrame(InsetFrame());
		AddFrame(InsetFrame());
		log.AddColumn("");
		log.NoHeader();
		Add(log.HSizePos().BottomPosZ(0, 200));
	}

	~App()
	{
		Shutdown();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
