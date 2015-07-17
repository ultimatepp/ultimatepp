//$ class Upp::Ctrl {
private:
	static Ptr<Ctrl>      desktop;
	static Vector<Ctrl *> topctrl;

	static Point fbCursorPos;
	static Image fbCursorImage;
	
	static Rect  fbCaretRect;
	static int   fbCaretTm;
	
	static bool  fbEndSession;
	static int64 fbEventLoop;
	static int64 fbEndSessionLoop;

	static Ctrl& Desktop();

	static void TimerAndPaint();
	static bool ProcessEvent(const String& event);

	static bool        quit;
	static int         main_pid;
	static Vector<int> pid;
	static TcpSocket   socket;
	static WebSocket   websocket;
	static int64       update_serial;
	static int64       recieved_update_serial;
	
	static Vector<Rect> invalid;

	static BiVector<String> event_queue;
	static Size DesktopSize;
	
	static void AddInvalid(const Rect& r);
	       void AddInvalid()                     { AddInvalid(GetScreenRect()); }
	
	static void InvalidateDesktop();
	
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

	static void ReadKeyMods(CParser& p);
	static void DoMouseButton(int event, CParser& p);

	static void DoMouseFB(int event, Point p, int zdelta, CParser& cp);

	static void Output();

	friend struct PaintProxy__;
	friend class TopWindowFrame;
	friend class SystemDraw;
	friend struct DnDLoop;

	void  SetOpen(bool b)               { isopen = b; }

	static void Signal(int signal);
	static void Broadcast(int signal);

	static bool DoKeyFB(dword key, int cnt);

	static TurtleStream                  turtle_stream;
	static Stream& OutputStream()        { turtle_stream.SetDataFlag(); return turtle_stream; }

	friend void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip,
	                         int n, Color color, int type, int animation);
	friend void DrawDragLine(SystemDraw& w, bool horz, int x, int y, int len, int n, int animation);
	
	static int   serial_time0;
	static int64 serial_0;

	friend struct ImageSysData;

public:
	enum Command {
		RECT = 0,
		IMAGE = 1,
		SETIMAGE = 2,
		INVERTRECT = 3,
		STD_CURSORIMAGE = 4,
		SETCURSORIMAGE = 5,
		MOUSECURSOR = 6,
		DISABLESENDING = 7,
		UPDATESERIAL = 8,
	
		IMAGEPP = 9,
		IMAGENP = 10,
		IMAGEPN = 11,
		IMAGENN = 12,
	
		RECTPP = 13,
		RECTNP = 14,
		RECTPN = 15,
		RECTNN = 16,
	
		SETCARET = 17,
		
		HORZDRAGLINE = 18,
		VERTDRAGLINE = 19,
		
		OPENLINK = 20,
	};

	static void Put8(int x)              { turtle_stream.SetDataFlag(); turtle_stream.Put(x); }
	static void Put16(int x);
	static void Put32(int x);
	static void Put(Point p);
	static void Put(Size sz);
	static void Put(const Rect& r);
	static void Put(const String& s);

	static void SyncClient();
	
	static String   host;
	static int      port;
	static bool     debugmode;
	static String   ip;
	static int      connection_limit;
	
	static Time     stat_started;
	static int64    stat_data_send;
	static int      stat_putrect;
	static int      stat_putimage;
	static int      stat_setimage;
	static int64    stat_setimage_len;
	static int      stat_roundtrip_ms;
	static int      stat_client_ms;

	static bool     StartSession();
	static void     EndSession();

	static Callback2<int, String> WhenConnect;
	static Callback1<int>         WhenTerminate;

	static Callback               WhenDisconnect;


	static void  SetDesktop(Ctrl& q);
	static Ctrl *GetDesktop()                  { return desktop; }
	static void  SetDesktopSize(Size sz);
	static Size  GetDesktopSize()              { return DesktopSize; }
	
	void DragRectDraw(const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
	                  Color color, int type, int animation);

	static Ctrl *FindMouseTopCtrl();

	static void PaintScene(SystemDraw& draw);
	
	enum { DRAWDRAGRECT_SCREEN = 0x8000 };
//$ };
