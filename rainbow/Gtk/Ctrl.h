//$ class Ctrl {
	void Create(bool popup);
	static gboolean GtkProc(GtkWidget *widget, GdkEvent *event, gpointer user_data);
	bool Proc(GdkEvent *event);
	
	GdkWindow *win() { return top->window->window; }
	
public:
	static void      EndSession()              {}
	static bool      IsEndSession()            { return false; }
//$ };
