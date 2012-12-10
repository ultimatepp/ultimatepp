#include <CtrlLib/CtrlLib.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

struct MyApp : TopWindow {
	Point  pos;
	Vector<String> log;

	virtual Size GetMinSize() const { return Size(100, 100); }
	virtual Size GetMaxSize() const { return Size(400, 200); }
	
	void Log(const String& s)
	{
		log.Add(s);
		log.Remove(0, max(log.GetCount() - 16, 0));
		Refresh();
	}
	
	virtual void Activate()
	{
		Log("Activate");
	}

	virtual void Deactivate()
	{
		Log("Deactivate");
	}
	
	virtual void Layout()
	{
		Log("Layout");
	}
	
	virtual void GotFocus()
	{
		Log("GotFocus");
	}
	
	virtual void LostFocus()
	{
		Log("LostFocus");
	}
	
	virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags)
	{
		static Tuple2<int, const char *> map[] = {
			{ 0x10, "MOUSEENTER" },
			{ 0x20, "MOUSEMOVE" },
			{ 0x30, "MOUSELEAVE" },
			{ 0x40, "CURSORIMAGE" },
			{ 0x50, "MOUSEWHEEL" },
			{ 0x80, "DOWN" },
			{ 0x90, "UP" },
			{ 0xa0, "DOUBLE" },
			{ 0xb0, "REPEAT" },
			{ 0xc0, "DRAG" },
			{ 0xd0, "HOLD" },
			{ 0xe0, "TRIPLE" },
		};
		Tuple2<int, const char *> *a = FindTuple(map, __countof(map), event & BUTTON);
		Tuple2<int, const char *> *b = FindTuple(map, __countof(map), event & ACTION);
		Log(String().Cat() << "MOUSE " << (a ? a->b : "") << '|' << (b ? b->b : "") << ' ' << p);

		return TopWindow::MouseEvent(event, p, zdelta, keyflags);
	}
	
	virtual bool Key(dword key, int count)
	{
		Log(GetKeyDesc(key).Cat() << ' ' << count);
	}

	virtual void MouseMove(Point p, dword keyflags)
	{
		pos = p;
		Refresh();
	}
	
	virtual void Paint(Draw& w) {
		int fcy = GetStdFontCy();
		w.DrawRect(GetSize(), White());
		int y = 0;
		w.DrawText(0, y += fcy, AsString(pos));
		w.DrawText(0, y += fcy, String().Cat() << "Shift: " << (int)GetShift());
		w.DrawText(0, y += fcy, String().Cat() << "Ctrl: " << (int)GetCtrl());
		w.DrawText(0, y += fcy, String().Cat() << "Alt: " << (int)GetAlt());
		w.DrawText(0, y += fcy, String().Cat() << "CapsLock: " << (int)GetCapsLock());
		w.DrawText(0, y += fcy, String().Cat() << "MouseLeft: " << (int)GetMouseLeft());
		w.DrawText(0, y += fcy, String().Cat() << "MouseRight: " << (int)GetMouseRight());
		w.DrawText(0, y += fcy, String().Cat() << "MouseMiddle: " << (int)GetMouseMiddle());
		y = 0;
		for(int i = 0; i < log.GetCount(); i++) {
			w.DrawText(100, y, log[i]);
			y += fcy;
		}
	}
	
	MyApp() {
		Sizeable().Zoomable().Title("Event test");
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
	return;

	PromptOK("Hello world!");
	return;

/*	
	RichEditWithToolBar edit;
	edit.SetQTF("[A500 Hello World!");
	TopWindow win;
	win.Add(edit.SizePos());
	win.Run();
*/
}
