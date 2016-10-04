#include <CtrlLib/CtrlLib.h>

using namespace Upp;

LineEdit logger;

struct MyCtrl : Ctrl
{
	DropList lst;
	Button btn;
	
	void Log(const char *fmt, ...)
	{
		static int pos = 0;
		char buffer[1024];
		va_list argptr;
		va_start(argptr, fmt);
		int l = vsprintf(buffer, fmt, argptr);
		va_end(argptr);
		
		logger.Insert(pos, buffer);
		pos += l;
		logger.Insert(pos, "\n");
		pos++;	
		logger.SetCursor(pos);
	}
	
	virtual void Paint(Draw &w)
	{
		w.DrawRect(GetSize(), Green());
	}
	
	MyCtrl()
	{
		Add(lst.LeftPos(10, 150).TopPos(10, 19));
		Add(btn.RightPos(50, 150).TopPos(10, 40));
		lst.Add("Item 0").Add("Item 1").Add("Item 2");
	}
	
	virtual void ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
	{
		if(event == CURSORIMAGE || event == MOUSEMOVE)
			return;		
		Log("event :%x child: %x", event, child);			
		return Ctrl::ChildMouseEvent(child, event, p, zdelta, keyflags);
	}

	virtual void ChildFrameMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
	{
		if(event == CURSORIMAGE || event == MOUSEMOVE)
			return;		
		Log("frameevent :%x child: %x", event, child);			
		return Ctrl::ChildMouseEvent(child, event, p, zdelta, keyflags);
	}
	
};

struct App : TopWindow
{
	MyCtrl myctrl;
	App()
	{
		Add(myctrl.HSizePos().TopPos(0, 200));
		Add(logger.HSizePos().VSizePos(200, 0));		
	}
};

GUI_APP_MAIN
{
	App().Run();
}

