class SliderCtrl : public Ctrl {
	int           value;
	int           min, max, step;
	bool          round_step;
	bool		  jump;

	int           SliderToClient(int value) const;
	int           ClientToSlider(int x) const;

	int           HoVe(int  x, int  y) const;

	int           Min() const     { return Upp::min(min, max); };
	int           Max() const     { return Upp::max(min, max); };
	int           ThumbSz() const;
	int           SliderSz() const;

public:
	typedef SliderCtrl CLASSNAME;

	Event<>       WhenSlideFinish;
	
	SliderCtrl();
	virtual ~SliderCtrl();

	virtual void  Paint(Draw& draw);
	virtual bool  Key(dword key, int repcnt);
	virtual void  LeftDown(Point pos, dword keyflags);
	virtual void  LeftRepeat(Point pos, dword keyflags);
	virtual void  LeftUp(Point pos, dword keyflags);
	virtual void  MouseMove(Point pos, dword keyflags);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  MouseEnter(Point p, dword keyflags);
	virtual void  MouseLeave();

	virtual void  SetData(const Value& value);
	virtual Value GetData() const;

	void          Inc();
	void          Dec();

	SliderCtrl&   MinMax(int _min, int _max);
	SliderCtrl&   Range(int max)                  { return MinMax(0, max); }
	int           GetMin() const                  { return min; }
	int           GetMax() const                  { return max; }

	bool          IsVert() const;
	SliderCtrl&   Jump(bool v = true)			  { jump = v; return *this; }

	SliderCtrl&   Step(int _step, bool _r = true) { step = _step; round_step = _r; return *this; }
	int           GetStep() const                 { return step; }
	bool          IsRoundStep() const             { return round_step; }
};
