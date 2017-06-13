class DisplayPopup : public Ctrl, public Link<DisplayPopup> {
	virtual void  Paint(Draw& w);
	virtual void  LeftDown(Point p, dword);
	virtual void  LeftDrag(Point p, dword);
	virtual void  LeftDouble(Point p, dword);
	virtual void  RightDown(Point p, dword);
	virtual void  LeftUp(Point p, dword);
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void  MouseLeave();
	virtual void  MouseMove(Point p, dword);

private:
	Ptr<Ctrl>      ctrl;
	Rect           item;
	Rect           slim;

	Value          value;
	Color          paper, ink;
	dword          style;
	const Display *display;
	int            margin;
	bool           usedisplaystdsize = false;

	Point   Op(Point p);
	void    Sync();

	static Link<DisplayPopup> *all();
	static bool StateHook(Ctrl *, int reason);
	static bool MouseHook(Ctrl *, bool, int, Point, int, dword);
	static void SyncAll();
	
	typedef DisplayPopup CLASSNAME;

public:
	void Set(Ctrl *ctrl, const Rect& item, const Value& v, const Display *display,
	         Color ink, Color paper, dword style, int margin = 0);
	void Cancel();
	bool IsOpen();
	bool HasMouse();
	void UseDisplayStdSize()                             { usedisplaystdsize = true; }

	DisplayPopup();
	~DisplayPopup();
};
