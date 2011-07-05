//$ class Ctrl {
private:
	static void AddInvalid(const Rect& rect);

	static Ptr<Ctrl>      desktop;
	static Vector<Ctrl *> topctrl;
	static ImageBuffer    framebuffer;
	static Vector<Rect>   invalid;

	static Point fbCursorPos;
	static Image fbCursorImage;
	
	static Point fbCursorBakPos;
	static Image fbCursorBak;
	
	static Rect  fbCaretRect;
	static Image fbCaretBak;
	static int   fbCaretTm;
	
	static int   renderingMode;
	
	static bool  fbEndSession;

	static Image GetBak(Rect& tr);
	static void RemoveCursor();
	static void RemoveCaret();
	static void CursorSync();
	
	int FindTopCtrl() const;
	static Rect GetClipBound(const Vector<Rect>& inv, const Rect& r);
	static void DoPaint(const Vector<Rect>& invalid);
	static void SyncTopWindows();
	
	void NewTop()                       { top = new Top; top->owner_window = NULL; }
	void PutForeground();
	static void MouseEventFB(Ptr<Ctrl> t, int event, Point p, int zdelta);

	friend struct PaintProxy__;
	friend class TopWindowFrame;

public:
	static void DoMouseFB(int event, Point p, int zdelta = 0);
	static bool DoKeyFB(dword key, int cnt);
	
	void  SetOpen(bool b)               { isopen = b; }

	static void InitFB();
	static void ExitFB();
	static void EndSession();

	static void  SetDesktop(Ctrl& q);
	static Ctrl *GetDesktop()                  { return desktop; }
	static void  SetFramebufferSize(Size sz);
	
	static const ImageBuffer& GetFrameBuffer() { return framebuffer; }
	
	static void SetRenderingMode(int mode);

//$ };
