//$ class Ctrl {
	void Create(Ctrl *owner, bool popup);

	static gboolean GtkProc(GtkWidget *widget, GdkEvent *event, gpointer user_data);
	static void     IMCommit(GtkIMContext *context, gchar *str, gpointer user_data);

	bool Proc(GdkEvent *event);
	void GtkMouseEvent(int action, GdkEvent *event);

	GdkWindow *gdk() { return top->window->window; }
	
	struct Win : Moveable<Win> {
		GtkWidget *gtk;
		GdkWindow *gdk;
		Ctrl      *ctrl;
	};

	static Vector<Win> wins;
	
	int FindCtrl(Ctrl *ctrl);
	int FindGtkWindow(GtkWidget *gtk);
	int FindGdkWindow(GdkWindow *gdk);
	
	static void FocusSync();

public:
	static void      EndSession()              {}
	static bool      IsEndSession()            { return false; }
//$ };
