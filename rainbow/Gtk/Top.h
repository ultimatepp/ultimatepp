//$ class TopWindow : Ctrl {
	ImageGdk gdk_icon;
	bool     topmost;

	void     CenterRect(Ctrl *owner);
	void     SetMode(int mode);
	void     SyncTopMost();

	static gboolean StateEvent(GtkWidget *widget, GdkEventWindowState *event, gpointer user_data);

	friend class Ctrl;
//$ };