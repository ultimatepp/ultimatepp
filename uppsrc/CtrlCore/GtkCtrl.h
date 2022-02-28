//$ namespace Upp {
//$ class Ctrl {
	void Create(Ctrl *owner, bool popup);

	static gboolean GtkProc(GtkWidget *widget, GdkEvent *event, gpointer user_data);
	static void     IMCommit(GtkIMContext *context, gchar *str, gpointer user_data);
	static void     IMPreedit(GtkIMContext *context, gpointer user_data);
	static void     IMPreeditEnd(GtkIMContext *context, gpointer user_data);
	static void     IMLocation(Ctrl *w);

	static int      DoButtonEvent(GdkEvent *event, bool press);
	static void     AddEvent(gpointer user_data, int type, const Value& value, GdkEvent *event);
	static void     FetchEvents(bool may_block);

/*
_DBG_
	static void     AddEvent(gpointer user_data, int type, const Value& value);
	static void     DoMouseEvent(int state, Point pos);
*/

	bool   DispatchMouseIn(int act, int zd);
	Image  GtkMouseEvent(int action, int act, int zd);
	void   GtkButtonEvent(int action);

	static bool IsWaitingEvent0(bool fetch);
	static bool ProcessEvent0(bool *quit, bool fetch);
	static bool ProcessEvents0(bool *quit, bool fetch);

	static void CaptureSync();

	static gboolean GtkEvent(GtkWidget *widget, GdkEvent *key, gpointer user_data);
	static gboolean GtkDraw(GtkWidget *widget, cairo_t *cr, gpointer data);

	static Point GetMouseInfo(GdkWindow *win, GdkModifierType& mod);
	static GdkDevice *GetMouseDevice();
#if GTK_CHECK_VERSION(3, 20, 0)
	static GdkSeat   *GetSeat();
#endif
	static bool MouseIsGrabbed();
	bool GrabMouse();
	static void UngrabMouse();
	
	static int scale; // in case GUI is scaling (e.g. in UHD mode)

	enum {
		EVENT_NONE = -15321,
		EVENT_TEXT,
		EVENT_FOCUS_CHANGE,
	};
	
	struct GEvent0 {
		int        time;
		int        windowid;
		int        type;
		Value      value;
		Point      mousepos;
		guint      state;
		int        count;

		bool       pen;
		bool       pen_barrel;
		bool       pen_inverted;
		bool       pen_eraser;
		double     pen_pressure;
		double     pen_rotation;
		Pointf     pen_tilt;
	};
	
	struct GEvent : Moveable<GEvent, GEvent0> {
		GdkEvent  *event;

		void Free();
		void Set(const GEvent& e);
		GEvent(const GEvent& e);
		void operator=(const GEvent& e);

		GEvent();
		~GEvent();
	};
	
	struct Win : Moveable<Win> {
		int          id;
		GtkWidget   *gtk;
		GdkWindow   *gdk;
		Ptr<Ctrl>    ctrl;
		Vector<Rect> invalid; // areas invalidated to be processed at next opportunity
	};

	void   Proc();
	bool   SweepConfigure(bool wait);
	bool   SweepFocus(bool wait);
	void   SyncWndRect(const Rect& rect);

	static BiVector<GEvent>  Events;
	static Vector<Ptr<Ctrl>> activePopup; // created with 'activate' flag - usually menu
	static Vector<Ptr<Ctrl>> visiblePopup; // any popup visible on screen
	static Vector<Win>       wins;
	static int               WndCaretTime;
	static bool              WndCaretVisible;
	static Ptr<Ctrl>         grabwindow;
	static Ptr<Ctrl>         grabpopup;
	static Ptr<Ctrl>         sel_ctrl;
	static Ptr<Ctrl>         activeCtrl;
	static bool              invalids; // there are active invalid areas

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
	static void DoCancelPreedit();
	
	static Rect frameMargins;
	static Rect GetFrameMargins();

	static Index<String>   dnd_targets;
	static String          dnd_text_target;
	static String          dnd_image_target;
	static String          dnd_files_target;
	static String          dnd_data;
	static String          dnd_data_fmt;
	static bool            dnd_data_wait;
	static bool            dnd_events;
	static GtkWidget      *dnd_widget;
	static GdkDragContext *dnd_context;
	static guint           dnd_time;

	static Ptr<Ctrl>                          dnd_source;
	static const VectorMap<String, ClipData> *dnd_source_data;
	static Vector<String>                     dnd_fmts;
	static int                                dnd_result;
	static Image                              dnd_icon;
	
	static void            GtkSelectionDataSet(GtkSelectionData *selection_data, const String& fmt, const String& data);
	static void            GtkGetClipData(GtkClipboard *clipboard, GtkSelectionData *selection_data, guint info, gpointer user_data);
	static void            AddFmt(GtkTargetList *list, const String& fmt, int info);
	static GtkTargetList  *CreateTargetList(const VectorMap<String, ClipData>& target);
	static String          GtkDataGet(GtkSelectionData *s);

	       void DndInit();
	       void DndExit();

	static void GtkDragBegin(GtkWidget *widget, GdkDragContext *context, gpointer user_data);
	static void GtkDragDelete(GtkWidget *widget, GdkDragContext *context, gpointer user_data);
	static void GtkDragGetData(GtkWidget *widget, GdkDragContext *context, GtkSelectionData *data, guint info, guint time, gpointer user_data);
	static void GtkDragDataReceived(GtkWidget *widget, GdkDragContext *context, gint x, gint y, GtkSelectionData *data, guint info, guint time, gpointer user_data);
	static gboolean GtkDragDrop(GtkWidget *widget, GdkDragContext *context, gint x, gint y, guint time, gpointer user_data);
	static void GtkDragEnd(GtkWidget *widget, GdkDragContext *context, gpointer user_data);
	static gboolean GtkDragFailed(GtkWidget *widget, GdkDragContext *context, GtkDragResult result, gpointer user_data);
	static void GtkDragLeave(GtkWidget *widget, GdkDragContext *context, guint time, gpointer user_data);
	static gboolean GtkDragMotion(GtkWidget *widget, GdkDragContext *context, gint x, gint y, guint time, gpointer user_data);

	static Ctrl  *DragWnd(gpointer user_data);
	static void   DndTargets(GdkDragContext *context);
	static bool   IsDragAvailable(const char *fmt);
	static String DragGet(const char *fmt);
	static PasteClip GtkDnd(GtkWidget *widget, GdkDragContext *context, gint x, gint y,
	                        guint time, gpointer user_data, bool paste);
	static bool   ProcessInvalids();

	friend void InitGtkApp(int argc, char **argv, const char **envptr);
	friend void GuiPlatformGripResize(TopWindow *q);

public: // really private:
	struct Gclipboard {
		VectorMap<String, ClipData> target;
		GtkClipboard *clipboard;

		String Get(const String& fmt);
		void   Put(const String& fmt, const ClipData& data);
		bool   IsAvailable(const String& fmt);
		void   Clear();

		Gclipboard(GdkAtom type);
	};

	static Gclipboard& gclipboard();
	static Gclipboard& gselection();
	static String      RenderPrimarySelection(const Value& fmt);

	static Vector<Event<>> hotkey;
	static Vector<dword>   keyhot;
	static Vector<dword>   modhot;
	static guint           MouseState;

	static int    SCL(int x)                        { return scale * x; }
	static Rect   SCL(int x, int y, int cx, int cy) { return RectC(SCL(x), SCL(y), SCL(cx), SCL(cy)); }
	static double LSC(int x)                        { return (double)x / scale; }
            
public:
	static void      EndSession()              {}
	static bool      IsEndSession()            { return false; }
	static void      PanicMsgBox(const char *title, const char *text);
	
	static Point     CurrentMousePos;
	static guint     CurrentState;
	static guint32   CurrentTime;
	static GEvent    CurrentEvent;

	GdkWindow *gdk() const { return top ? gtk_widget_get_window(top->window) : NULL; }
	GtkWindow *gtk() const { return top ? (GtkWindow *)top->window : NULL; }

	static GdkFilterReturn RootKeyFilter(GdkXEvent *xevent, GdkEvent *event, gpointer data);

//$ }};
