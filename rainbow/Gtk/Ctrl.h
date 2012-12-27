//$ class Ctrl {
	void Create(Ctrl *owner, bool popup);

	static gboolean GtkProc(GtkWidget *widget, GdkEvent *event, gpointer user_data);
	static void     IMCommit(GtkIMContext *context, gchar *str, gpointer user_data);


	void   GtkMouseEvent(int action, GdkEvent *event);
	void   GtkKeyEvent(GdkEventKey *key, bool pressed);

	virtual bool Proc(GdkEvent *event);

	static gboolean GtkEvent(GtkWidget *widget, GdkEvent *key, gpointer user_data);

	void     GtkConnect();

	GdkWindow *gdk() const { return top ? top->window->window : NULL; }
	GtkWindow *gtk() const { return top ? (GtkWindow *)top->window : NULL; }
	
	struct Win : Moveable<Win> {
		GtkWidget *gtk;
		GdkWindow *gdk;
		Ptr<Ctrl>  ctrl;
	};

	static Vector<Ctrl *>      activePopup;
	static Vector<Win>         wins;
	static int                 WndCaretTime;
	static bool                WndCaretVisible;
	static Ptr<Ctrl>           grabwindow;
	static Ptr<Ctrl>           grabpopup;

	int FindCtrl(Ctrl *ctrl);
	int FindGtkWindow(GtkWidget *gtk);
	int FindGdkWindow(GdkWindow *gdk);

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
//$ };
