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
		Log("Layout " + AsString(GetScreenView()));
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
		return false;
	}

	virtual void MouseMove(Point p, dword keyflags)
	{
		pos = p;
		SetCaret(p.x, 0, 5, 5);
		Refresh();
	}

	virtual Image CursorImage(Point p, dword keyflags)
	{
		Size sz = GetSize();
		if(p.x < sz.cx / 4)
			return Image::Arrow();
		if(p.x < 2 * sz.cx / 4)
			return Image::Hand();
		if(p.x < 3 * sz.cx / 4)
			return CtrlImg::ibeam0();
		return (msecs() / 500) & 1 ? CtrlImg::HelpCursor1() : CtrlImg::HelpCursor0();
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

	EditString text;
	
	MyApp() {
		Sizeable().Zoomable().Title("Event test");
		Icon(CtrlImg::save_as());
//		Icon(CtrlImg::Network());
	}
};

struct MyApp2 : TopWindow {
	DocEdit text;
/*
	virtual void Activate()
	{
		LOG("Activate");
	}

	virtual void Deactivate()
	{
		LOG("Deactivate");
	}
	
	virtual void Layout()
	{
		LOG("Layout");
	}
	
	virtual void GotFocus()
	{
		LOG("GotFocus");
	}
	
	virtual void LostFocus()
	{
		LOG("LostFocus");
	}
*/
	MyApp2() {
		Zoomable().
		Sizeable().
		Title("Widget test");
		Add(text.SizePos());
		text <<= "Test\nHello World!";
	}
};

struct MyApp3 : TopWindow {
	Rect rect;
	
	typedef MyApp3 CLASSNAME;
	
	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, SColorPaper());
		DrawFrame(w, rect, Black());
		
		w.DrawRect(0, 0, 16, 16, LtGray());
		w.DrawRect(16, 16, 10, 10, Black());
	}

	virtual void LeftDown(Point p, dword keyflags) {
		RectTracker tr(*this);
		Size sz = GetSize();
		tr.Normal();
	//	tr.Width(2);
	//	tr.Dashed();
	//	tr.Animation();
		tr.SetColor(Red());
		rect = tr.Track(rect, ALIGN_RIGHT, ALIGN_BOTTOM);
		Refresh();
		ClearClipboard();
		AppendClipboardText("Hello world!");
	}

	MyApp3() {
		rect = RectC(100, 100, 100, 100);
	}
};

struct MyApp4 : TopWindow {
	RichEditWithToolBar edit;
	StatusBar status;
	MenuBar   menu;

	typedef MyApp4 CLASSNAME;
	
	void Test()
	{
		Exclamation("Test");
	}
	
	void Exit()
	{
		Break();
	}
	
	void DoMaximize()
	{
		Maximize();
	}
	
	void DoMinimize()
	{
		Minimize();
	}

	void DoOverlap()
	{
		Overlap();
	}

	void DoFullscreen()
	{
		FullScreen();
	}
	
	void DoAbove()
	{
		TopMost(!IsTopMost());
	}
	
	void DoToolWindow()
	{
		ToolWindow(!IsToolWindow());
	}
	
	void DoFrameless()
	{
		FrameLess(!IsFrameLess());
	}

	void DoUrgent()
	{
		Urgent(!IsUrgent());
	}

	void SubMenu(Bar& bar)
	{
		bar.Add("Test", THISBACK(Test));
		bar.Add("Exit", THISBACK(Exit));
	}
	
	void NewTool()
	{
		TopWindow win;
		win.ToolWindow();
		win.Run();
	}

	void NewUrgent()
	{
		TopWindow win;
		win.Urgent();
		win.Run();
	}
	
	void GtkDlg()
	{
		GtkWidget *dialog = gtk_message_dialog_new(gtk(), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
		                                           GTK_BUTTONS_CLOSE, "Hello !");
		gtk_dialog_run(GTK_DIALOG (dialog));
		gtk_widget_destroy(dialog);
	}
	
	void Menu(Bar& bar)
	{
		bar.Add("Test", THISBACK(Test));
		bar.Add("GTK dialog", THISBACK(GtkDlg));
		bar.Add("Maximize", THISBACK(DoMaximize));
		bar.Add("Minimize", THISBACK(DoMinimize));
		bar.Add("Overlap", THISBACK(DoOverlap));
		bar.Add("Fullscreen", THISBACK(DoFullscreen));
		bar.Add("TopMost", THISBACK(DoAbove)).Check(IsTopMost());
		bar.Add("Tool window", THISBACK(DoToolWindow)).Check(IsToolWindow());
		bar.Add("Frameless", THISBACK(DoFrameless)).Check(IsFrameLess());
		bar.Add("Urgent", THISBACK(DoUrgent)).Check(IsUrgent());
		bar.Add("New toolwindow", THISBACK(NewTool));
		bar.Add("New urgent", THISBACK(NewUrgent));
//		bar.Add("Change title", THISBACK(DoTitle));
		bar.Separator();
		bar.Add("SubMenu", THISBACK(SubMenu));
		bar.Separator();
		bar.Add("Exit", THISBACK(Exit));
	}
	
	void MainMenu(Bar& bar)
	{
		bar.Add("Menu", THISBACK(Menu));
		bar.Add("Menu", THISBACK(Menu));
	}
	
	void SetMenu()
	{
		menu.Set(THISBACK(MainMenu));
	}
	
	MyApp4() {
		Title("Complex Rainbow test");
		String qtf;
		for(int i = 0; i < 100; i++)
			qtf << " [A9 " << i << "[/ Hello] [_ World]!]&";
		edit.SetQTF(qtf);
		Sizeable().Zoomable();
		Add(edit.SizePos());
		AddFrame(menu);
		AddFrame(status);
		SetMenu();
	}
};


GUI_APP_MAIN
{
//	AppendClipboardText("Hello world! now it is " + AsString(GetSysTime()));
//	AppendClipboardImage(CtrlImg::exclamation());
//	return;
#if 1
	MyApp().Run();
	return;
#endif

#if 0
	String txt = "Test";

	EditText(txt, "Test", "Test");
	return;
#endif

#if 0
	PromptOK("Hello world!");
	return;
#endif

#if 1	
	RichEditWithToolBar edit;
	TopWindow win;
	win.Sizeable();
	win.Add(edit.SizePos());
	win.Run();
#endif
}
