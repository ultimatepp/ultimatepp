NAMESPACE_UPP

class RulerCtrl : public FrameCtrl<Ctrl>
{
public:
	RulerCtrl();
	virtual ~RulerCtrl();

	void            Horz(bool bottom = false)         { is_vert = false; is_right = bottom; Refresh(); }
	void            Vert(bool right = false)          { is_vert = true; is_right = right; Refresh(); }
	bool            IsVert() const                    { return is_vert; }
	bool            IsBottomRight() const             { return is_right; }
	int             GetLength() const                 { return is_vert ? GetSize().cy : GetSize().cx; }

	void            SetWidth(int w)                   { width = w; RefreshParentLayout(); }
	int             GetWidth() const                  { return width; }

	void            SetZoom(double scale, double delta);

	void            SetScale(double scale)            { SetZoom(scale, GetDelta()); }
	double          GetScale() const                  { return scale; }

	void            SetDelta(double delta)            { SetZoom(GetScale(), delta); }
	double          GetDelta() const                  { return delta; }

	void            MinMax(double min, double max);
	double          GetMin() const                    { return min_range; }
	double          GetMax() const                    { return max_range; }

	void            SetCursor(double pos);
	void            KillCursor()                      { SetCursor(Null); }
	double          GetCursor() const                 { return cursor; }

	void            AutoSelect()                      { autoselect = true; }
	void            NoAutoSelect()                    { autoselect = false; }

	void            PickSmallStep(pick_ Vector<double>& pos, double repeat = Null);
	void            SetSmallStep(double delta);

	void            PickTextStep(pick_ Vector<double>& pos, pick_ Vector<String>& texts, double repeat, const Convert& convert = NoConvert());
	void            SetTextStep(double repeat, const Convert& convert = NoConvert());

	void            SetFont(Font f)                   { font = f; Refresh(); }
	Font            GetFont() const                   { return font; }

	void            SetBackground(Color bg)           { background = bg; Refresh(); }
	Color           GetBackground() const             { return background; }

	void            SetOuterColor(Color bg)           { outer_color = bg; Refresh(); }
	Color           GetOuterColor() const             { return outer_color; }

	double          FromClient(double client) const   { return (client - GetRawDelta()) / scale; }
	double          ToClientf(double pos) const       { return pos * scale + GetRawDelta(); }
	int             ToClient(double pos) const        { return fround(ToClientf(pos)); }

	double          Snap(double v) const;

	virtual void    Layout();
	virtual void    FrameLayout(Rect& rc);
	virtual void    FrameAddSize(Size& sz);
	virtual void    Paint(Draw& draw);

	void            SetLabel(const char *s)           { label_text = s; Refresh(); }

	virtual void    LeftDown(Point pt, dword keyflags);
	virtual void    LeftUp(Point pt, dword keyflags);
	virtual void    RightDown(Point pt, dword keyflags);
	virtual void    MouseMove(Point pt, dword keyflags);

	static int      GetStdWidth();
	void            RefreshCursor(double cursor);

	virtual void    SetData(const Value& v);
	virtual Value   GetData() const;

public:
	Callback1<Bar&> WhenBar;
	Callback1<double> WhenSelect;
	Callback        WhenLeftClick;

private:
	double          GetRawDelta() const;

private:
	int             width;
	double          scale, delta;
	mutable double  raw_delta;
	double          min_range, max_range;
	Vector<double>  small_step;
	double          small_repeat;
	Vector<double>  text_step;
	Vector<String>  text_value;
	const Convert  *text_convert;
	double          text_repeat;
	double          cursor;
	Font            font;
	bool            is_vert;
	bool            is_right;
	bool            autoselect;
	String          label_text;
	Color           background;
	Color           outer_color;

	enum
	{
		ARROW_DIAM  = 7,
		ARROW_RAD   = (ARROW_DIAM + 1) >> 1,
		ARROW_WIDTH = 5,
	};
};

class HRulerCtrl : public RulerCtrl { public: HRulerCtrl() { Horz(); } };
class VRulerCtrl : public RulerCtrl { public: VRulerCtrl() { Vert(); } };

END_UPP_NAMESPACE
