//$ class Ctrl {
	static Ptr<Ctrl>      desktop;
	static Vector<Ctrl *> topctrl;

	static Point glCursorPos;
	static Image glCursorImage;
	static Rect  glCaretRect;
	static int   glCaretTm;

	int FindTopCtrl() const;
	static void SyncTopWindows();
	void NewTop()                       { top = new Top; top->owner_window = NULL; }
	void PutForeground();
	static void MouseEventGl(Ptr<Ctrl> t, int event, Point p, int zdelta);

	friend class TopWindowFrame;
	friend class SystemDraw;

public:
	static Rect screenRect;

	static void InitGl();
	static void DoMouseGl(int event, Point p, int zdelta = 0);
	static bool DoKeyGl(dword key, int cnt);
	static void CursorSync(Draw& w);
	
	void  SetOpen(bool b)               { isopen = b; }
	void  SetTop()                      { top = new Top; }
	
	static void  SetDesktop(Ctrl& q);
	static Ctrl *GetDesktop()                  { return desktop; }
	static void  SetWindowSize(Size sz);
	
	static void  DrawScreen();
//$ };
