class DisplayPopup : public Pte<DisplayPopup> {
	bool                  usedisplaystdsize = false;

	static Rect           screen_rect;
	static Ptr<Ctrl>      ctrl;
	static Rect           item;
	static Value          value;
	static Color          paper, ink;
	static dword          style;
	static const Display *display;
	static int            margin;
	static bool           usedisplaystdsize_s;

	static bool StateHook(Ctrl *, int reason);
	static bool MouseHook(Ctrl *, bool, int, Point, int, dword);
	static void PaintHook(Ctrl *ctrl, Draw& w, const Rect&);

	static Rect Check(Ctrl *ctrl, const Rect& item, const Value& value, const Display *display, int margin);
	static void Sync();
	static void RefreshRect();

	typedef DisplayPopup CLASSNAME;

public:
	void Set(Ctrl *ctrl, const Rect& item, const Value& v, const Display *display,
	         Color ink, Color paper, dword style, int margin = 0);
	void Cancel();
	bool IsOpen();
	bool HasMouse();
	void UseDisplayStdSize();

	DisplayPopup();
};
