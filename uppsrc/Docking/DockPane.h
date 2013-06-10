class DockPane : public Splitter {
public:
	typedef DockPane CLASSNAME;

private:
	struct DummyCtrl : public Ctrl 	{
		DummyCtrl() { minsize = stdsize = Null; Transparent(); NoWantFocus(); }
		Size minsize;
		Size stdsize;
		virtual Size GetMinSize() const { return minsize; }
		virtual Size GetStdSize() const { return stdsize; }
	};
	DummyCtrl 	dummy;
	int 		animtick;
	int 		animinterval;
	int 		animmaxticks;
	Vector<int>	animpos;
	Vector<int>	savedpos;

	void 	StartAnimate(int ix, Size sz, bool restore);
	void 	AnimateTick();
	void 	EndAnimate();
	
	void 	SmartReposUp(Vector<int>& p, int ix, int sz);
	void 	SmartReposDown(Vector<int>& p, int ix);
	void 	SimpleRepos(Vector<int>& p, int ix, int sz);
	int 	GetMinPos(int notix);
	void 	FixChildSizes();
	
	int 	NormalPos(int ix) const			{ return (ix > 0) ? pos[ix] - pos[ix-1] : pos[ix]; }
	void	CumulativePos(Vector<int>& p) const;
	void	NonCumulativePos(Vector<int>& p) const;
	
	enum { TIMEID_ACTION_CHECK = Ctrl::TIMEID_COUNT,
		   TIMEID_ANIMATE,
           TIMEID_COUNT };	
public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	int 	GetCount() const						{ return pos.GetCount(); }
	
	void	Dock(Ctrl& newctrl, Size sz, int pos, bool animate, bool save = false);
	void 	Undock(Ctrl& newctrl, bool animate, bool restore = false);
	
	void	SavePos()								{ savedpos <<= pos; }
	void	RestorePos()							{ ASSERT(savedpos.GetCount() == pos.GetCount()); pos <<= savedpos; Layout(); }
	
	int 	GetCtrlSize(int i) const				{ return NormalPos(i); }
	void	SetCtrlSize(int i, int sz)				{ pos[i] = (i == 0) ? sz : sz + pos[i-1]; }
	
	void 	Clear()									{ pos.Clear(); savedpos.Clear(); }
	
	bool	IsAnimating() const	 					{ return animpos.GetCount(); }
	void	SetAnimateRate(int ticks, int ms) 		{ animmaxticks = max(1, ticks);	animinterval = max(0, ms); }
	int		GetAnimMaxTicks() const					{ return animmaxticks; }
	int		GetAnimInterval() const					{ return animinterval; }
	Rect	GetFinalAnimRect(Ctrl& ctrl);
	
	virtual void Serialize(Stream& s)				{ s % pos; if (s.IsLoading()) FixChildSizes(); }
	
	DockPane();
};
