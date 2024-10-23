class DisplayPopup : public Pte<DisplayPopup> {
private:
	struct Pop : Pte<Pop> {
		struct PopCtrl : public Ctrl {
			virtual void  Paint(Draw& w);
			
			struct Pop *p;
		};
		
		Ptr<Ctrl>      ctrl;
		Rect           item;
	
		Value          value;
		Color          paper, ink;
		dword          style;
		const Display *display;
		int            margin;
		bool           usedisplaystdsize = false;
	
		PopCtrl   view;
		PopCtrl   frame;
	
		Callback WhenClose;
	
		void Set(Ctrl *ctrl, const Rect& item, const Value& v, const Display *display,
		         Color ink, Color paper, dword style, int margin = 0);
		void Sync();

		static Vector<Pop *>& all();
	
		Pop();
		~Pop();
	};
	
	One<Pop>     popup;
	bool         usedisplaystdsize = false;

	static bool StateHook(Ctrl *, int reason);
	static bool MouseHook(Ctrl *, bool, int, Point, int, dword);
	static void SyncAll();
	static Rect Check(Ctrl *ctrl, const Rect& item, const Value& value, const Display *display, int margin);

	typedef DisplayPopup CLASSNAME;

public:
	void Set(Ctrl *ctrl, const Rect& item, const Value& v, const Display *display,
	         Color ink, Color paper, dword style, int margin = 0);
	void Cancel();
	bool IsOpen();
	bool HasMouse();
	void UseDisplayStdSize();

	DisplayPopup();
	~DisplayPopup();
};
