//$ class Ctrl {
	void Create(Ctrl *owner, bool popup);

	static gboolean GtkProc(GtkWidget *widget, GdkEvent *event, gpointer user_data);
	static void     IMCommit(GtkIMContext *context, gchar *str, gpointer user_data);

	static void     DoMouseEvent(int state, Point pos);
	static int      DoButtonEvent(GdkEvent *event, bool press);
	static void     AddEvent(gpointer user_data, int type, const Value& value);
	static void     FetchEvents(bool may_block);

	bool   DispatchMouseIn(int act, int zd);
	void   GtkMouseEvent(int action, int act, int zd);
	void   GtkButtonEvent(int action);

	void   Proc();
	bool   SweepConfigure(bool wait);

	static gboolean GtkEvent(GtkWidget *widget, GdkEvent *key, gpointer user_data);

	enum {
		EVENT_NONE = -15321,
		EVENT_TEXT,
	};
	
	struct Event : Moveable<Event> {
		int        time;
		int        windowid;
		int        type;
		Value      value;
		Point      mousepos;
		guint      state;
		int        count;
	};
	
	struct Win : Moveable<Win> {
		int        id;
		GtkWidget *gtk;
		GdkWindow *gdk;
		Ptr<Ctrl>  ctrl;
	};

	static bool                EventMouseValid;
	static Point               EventMousePos;
	static guint               EventState; 
	static BiVector<Event>     Events;
	static Vector< Ptr<Ctrl> > activePopup;
	static Vector<Win>         wins;
	static int                 WndCaretTime;
	static bool                WndCaretVisible;
	static Ptr<Ctrl>           grabwindow;
	static Ptr<Ctrl>           grabpopup;

	static int FindId(int id);
	static int FindCtrl(Ctrl *ctrl);
	static int FindGtkWindow(GtkWidget *gtk);
	static int FindGdkWindow(GdkWindow *gdk);
	
	static Ctrl *GetTopCtrlFromId(int id);
	static Ctrl *GetTopCtrlFromId(gpointer user_data) { return GetTopCtrlFromId((uint32)(uintptr_t)user_data); }

	static void SyncPopupCapture();
	void ReleasePopupCapture();
	
	static void FocusSync();
	static void AnimateCaret();
	static gboolean TimeHandler(GtkWidget *);
	static void InvalidateMousePos();
	static void StopGrabPopup();
	static void StartGrabPopup();
	static bool ReleaseWndCapture0();

	friend void InitGtkApp(int argc, char **argv, const char **envptr);
	friend void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                             Color color, int type, int animation);
	friend void FinishDragRect(Ctrl& q);
	friend void GuiPlatformGripResize(TopWindow *q);

public:
	struct Gclipboard {
		ArrayMap<String, ClipData> target;
		GtkClipboard *clipboard;

		String Get(const String& fmt);
		void   Put(const String& fmt, const ClipData& data);
		bool   IsAvailable(const String& fmt);
		void   Clear();

		Gclipboard(GdkAtom type);
	};

	static Gclipboard& gclipboard();
                  
public:
	static void      EndSession()              {}
	static bool      IsEndSession()            { return false; }
	static void      PanicMsgBox(const char *title, const char *text);
	
	static Point     CurrentMousePos;
	static guint     CurrentState; 
	static guint32   CurrentTime;
	static Event     CurrentEvent;

	GdkWindow *gdk() const { return top ? top->window->window : NULL; }
	GtkWindow *gtk() const { return top ? (GtkWindow *)top->window : NULL; }
//$ };
