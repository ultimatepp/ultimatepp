//$ class TopWindow {
private:
	class Frame : public Ctrl {
	public:
		virtual void  Layout();
		virtual void  Paint(Draw& w);
		virtual Image CursorImage(Point p, dword keyflags);
		virtual void  LeftDown(Point p, dword keyflags);
		virtual void  MouseMove(Point p, dword keyflags);
	
	private:
		String title;
		
		Point dir;
		Point startpos;
		Rect  startrect;
	
		Point GetDragMode(Point p);
	
	public:
		void SetTitle(const String& s)           { title = s; Refresh(); }
		Rect GetClient() const;
		void SetClient(Rect r);
		void GripResize();
		
		Callback WhenLayout;
	};

	Frame frame;
	
	void SyncRect();
	void SyncFrameRect(const Rect& r)            { frame.SetClient(r); }
	void DestroyFrame()                          { if(frame.IsOpen()) frame.Close(); }
	
	friend class Ctrl;

public:
	void GripResize()                            { frame.GripResize(); }
//$ };
