class ToolWin : public TopWindow, public CtrlFrame {
public:
	virtual void   FrameAddSize(Size& sz);
	virtual void   FrameLayout(Rect& r);
	virtual void   FramePaint(Draw& w, const Rect& r);

	virtual void   MouseMove(Point p, dword keyflags);
	virtual void   LeftUp(Point p, dword keyflags);
	virtual Image  CursorImage(Point p, dword keyflags);
	virtual Image  FrameMouseEvent(int event, Point p, int zdelta, dword keyflags);

private:		
	Point      p0;
	Rect       rect0;
	Point      dragdir;
	ToolButton close;

	void DoClose();
	
	Point GetDir(Point p) const;
	int   GetTitleCy() const;
	int   GetBorder() const;
	Rect  GetMargins() const;
	Size  AddMargins(Size sz) const;
	
	void StartMouseDrag0();

	typedef ToolWin CLASSNAME;

public:
	virtual void MoveBegin();
	virtual void Moving();
	virtual void MoveEnd();

	void StartMouseDrag();
	void SetClientRect(Rect r);

	void PlaceClientRect(Rect r);
	
	ToolWin();
};
