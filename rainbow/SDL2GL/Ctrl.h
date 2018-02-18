//$ class Ctrl {
private:
	static Ptr<Ctrl>      desktop;
	static Vector<Ctrl *> topctrl;
	static bool           invalid;

	static Point fbCursorPos;
	static Image fbCursorImage;
	static bool  sdlMouseIsIn;
	
	static Rect  fbCaretRect;
	static int   fbCaretTm;
	
	static bool  fbEndSession;
	static int64 fbEventLoop;
	static int64 fbEndSessionLoop;
	
	static void CursorSync();
	
	int FindTopCtrl() const;
	static Rect GetClipBound(const Vector<Rect>& inv, const Rect& r);
	static void DoPaint();
	static void SyncTopWindows();

//	static void AddInvalid(const Rect& rect);

	void DestroyWnd();

	void NewTop()                       { top = new Top; top->owner_window = NULL; }
	void PutForeground();
	static void MouseEventFB(Ptr<Ctrl> t, int event, Point p, int zdelta);

	static void DrawLine(const Vector<Rect>& clip, int x, int y, int cx, int cy, bool horz,
	                     const byte *pattern, int animation);
	static void DragRectDraw0(const Vector<Rect>& clip, const Rect& rect, int n,
	                          const byte *pattern, int animation);

	friend struct PaintProxy__;
	friend class TopWindowFrame;
	friend class SystemDraw;
	friend struct DnDLoop;

	void  SetOpen(bool b)               { isopen = b; }

protected:
	static int PaintLock;

public:
	static void HandleSDLEvent(SDL_Event* event);

	static void DoMouseFB(int event, Point p, int zdelta = 0);
	static bool DoKeyFB(dword key, int cnt);

	static void InitFB();
	static void ExitFB();
	static void EndSession();

	static void  SetDesktop(Ctrl& q);
	static Ctrl *GetDesktop()                  { return desktop; }
	static void  SetDesktopSize(Size sz);
	
	static void Invalidate()                   { invalid = true; }

	void DragRectDraw(const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
	                  Color color, int type, int animation);

	static Ctrl *FindMouseTopCtrl();

	static void PaintScene(SystemDraw& draw);
	static void PaintCaretCursor(SystemDraw& draw);
	
	static bool SystemCursor;

	enum { DRAWDRAGRECT_SCREEN = 0x8000 };

//$ };
