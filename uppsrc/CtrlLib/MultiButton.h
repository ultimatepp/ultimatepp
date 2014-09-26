#ifndef _CtrlLib_MultiButton_h_
#define _CtrlLib_MultiButton_h_

class MultiButton : public Ctrl {
public:
	virtual void  Paint(Draw& w);
	virtual void  MouseMove(Point p, dword flags);
	virtual void  LeftDown(Point p, dword flags);
	virtual void  LeftUp(Point p, dword flags);
	virtual void  MouseLeave();
	virtual void  CancelMode();
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  SetData(const Value& data);
	virtual Value GetData() const;
	virtual Size  GetMinSize() const;
	virtual int   OverPaint() const;

public:
	struct Style : public ChStyle<Style> {
		Value edge[4];
		bool  activeedge;
		Value look[4];
		Value left[4];
		Value lmiddle[4];
		Value right[4];
		Value rmiddle[4];
		Value simple[4];
		int   border;
		Value trivial[4];
		int   trivialborder;
		Color monocolor[4];
		Color fmonocolor[4];
		Point pressoffset;
		Value sep1, sep2;
		int   sepm;
		int   stdwidth;
		bool  trivialsep;
		bool  usetrivial;
		Rect  margin;
		int   overpaint;
		int   loff, roff;
		Color error;
	};

	class SubButton {
		friend class MultiButton;

		String       tip;
		MultiButton *owner;
		Image        img;
		int          cx;
		bool         main;
		bool         left;
		bool         monoimg;
		bool         enabled;
		bool         visible;

		String       label;

		void Refresh();

	public:
		Callback WhenPush;
		Callback WhenClick;

		SubButton& SetImage(const Image& m);
		SubButton& SetMonoImage(const Image& m);
		SubButton& SetStdImage();
		SubButton& SetLabel(const char *text);
		SubButton& Left(bool b = true);
		SubButton& Width(int w);
		SubButton& Enable(bool b);
		SubButton& Show(bool b);
		SubButton& Tip(const char *s)                     { tip = s; return *this; }
		SubButton& Main(bool b = true);

		Callback operator<<=(Callback cb)                 { WhenPush = cb; return cb; }

		SubButton();
	};

private:
	enum {
		MAIN = -1,
		LB_IMAGE = 5, // image <-> text space
		LB_MARGIN = 10
	};

	virtual bool Frame();

	const Display   *display;
	const Convert   *convert;
	Value            value;
	Value            error;
	int              valuecy;
	bool             push;
	bool             nobg;
	String           tip;
	Rect             pushrect;

	Array<SubButton> button;
	int              hl;

	const Style *style;

	int  FindButton(int px);
	void Margins(int& l, int& r);
	Rect ContentRect();
	bool GetPos(SubButton& b,  int& lx, int& rx, int& x, int& cx, int px = -1);
	void GetPos(int ii, int& x, int& cx);
	int  ChState(int i);
	Rect GetMargin();
	void Lay(Rect& r);
	bool ComplexFrame();
	bool Metrics(int& border, int& lx, int &rx, const Rect& r);
	bool Metrics(int& border, int& lx, int &rx);

	friend class SubButton;
	friend class MultiButtonFrame;

public:
	Callback WhenPush;
	Callback WhenClick;

	static const Style& StyleDefault();
	static const Style& StyleFrame();

	bool IsTrivial() const;

	void Reset();

	void PseudoPush(int bi);
	void PseudoPush();

	SubButton& AddButton();
	SubButton& InsertButton(int i);
	void       RemoveButton(int i);
	int        GetButtonCount() const                { return button.GetCount(); }
	SubButton& GetButton(int i)                      { return button[i]; }
	SubButton& MainButton();

	Rect  GetPushScreenRect() const                  { return pushrect; }

	const Display& GetDisplay() const                { return *display; }
	const Convert& GetConvert() const                { return *convert; }
	const Value&   Get() const                       { return value; }
	
	void  Error(const Value& v)                      { error = v; Refresh(); }

	MultiButton& SetDisplay(const Display& d);
	MultiButton& NoDisplay();
	MultiButton& SetConvert(const Convert& c);
	MultiButton& SetValueCy(int cy);
	MultiButton& Set(const Value& v);
	MultiButton& Tip(const char *s)                  { tip = s; return *this; }
	MultiButton& NoBackground(bool b = true);

	MultiButton& SetStyle(const Style& s)            { style = &s; Refresh(); return *this; }

	MultiButton();
};

class MultiButtonFrame : public MultiButton, public CtrlFrame {
public:
	virtual void  FrameLayout(Rect& r);
	virtual void  FrameAddSize(Size& sz);
	virtual void  FrameAdd(Ctrl& parent);
	virtual void  FrameRemove();

private:
	virtual bool Frame();

public:
	void AddTo(Ctrl& w);
};

#endif
