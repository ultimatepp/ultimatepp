	ImageGdk gdk_icon;
	ImageGdk gdk_largeicon;
	bool     topmost;

	struct CustomBarIcon : public Ctrl {
		void Paint(Draw& w) override;
		void MouseMove(Point p, dword keyflags) override;
		void LeftDown(Point p, dword keyflags) override;
		void LeftUp(Point, dword keyflags) override;
		void MouseLeave() override;
		
		Image img;
		
		void Set(const Image& m)    { if(!m.IsSame(img)) { img = m; Refresh(); }}
	};
	
	struct BarCtrl : public Ctrl {
		void LeftDouble(Point p, dword keyflags) override;
	};

	One<FrameTop<Ctrl>>   custom_bar_frame;
	One<FrameRight<Ctrl>> custom_bar_icons;
	One<BarCtrl>          custom_bar;
	CustomBarIcon         minicon, maxicon, closeicon;
	Color                 custom_titlebar_bk = SColorFace();
	int                   custom_titlebar_cy = -1;
	
	enum { FULLSCREEN = 99 };

	void     CenterRect(Ctrl *owner);
	void     SetMode(int mode);
	void     SyncTopMost();

	void     SyncCustomBar();
	void     SyncIcons();
	bool     IsCustomTitleBar__() const;
	Ctrl    *MakeCustomTitleBar__(Color bk, int mincy);
	static   void Init();

	static gboolean StateEvent(GtkWidget *widget, GdkEventWindowState *event, gpointer user_data);

	friend class Ctrl;
	friend bool InitGtkApp(int argc, char **argv, const char **envptr);

