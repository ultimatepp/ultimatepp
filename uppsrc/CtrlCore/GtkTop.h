	ImageGdk gdk_icon;
	ImageGdk gdk_largeicon;
	bool     topmost;

	One<FrameTop<Ctrl>>   custom_bar_frame;
	One<Ctrl>             custom_bar;
	Color                 custom_titlebar_bk = SColorFace();
	int                   custom_titlebar_cy = -1;
	
	enum { FULLSCREEN = 99 };

	void     CenterRect(Ctrl *owner);
	void     SetMode(int mode);
	void     SyncTopMost();

	void     SyncCustomBar();
	bool     IsCustomTitleBar__() const;
	Ctrl    *MakeCustomTitleBar__(Color bk, int mincy);
	static   void Init();

	static gboolean StateEvent(GtkWidget *widget, GdkEventWindowState *event, gpointer user_data);

	friend class Ctrl;
	friend bool InitGtkApp(int argc, char **argv, const char **envptr);

