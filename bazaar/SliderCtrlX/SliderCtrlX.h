
class SliderCtrlX : public Ctrl {
	
	typedef enum { HORZ, VERT } HOVE;
	
	typedef enum { MAJOR, MINOR } MAJORMINOR;

public:
	typedef enum { TOP, MIDDLE_TOP, MIDDLE_BOTTOM, BOTTOM, 
		RIGHT = TOP, LEFT = BOTTOM, MIDDLE_LEFT = MIDDLE_BOTTOM, MIDDLE_RIGHT = MIDDLE_TOP } TICK_ALIGNMENT;
		
private:
	Vector<int>			m_vValues;
	Vector<Image>		m_vThumbImgs;
	Vector<Image>		m_vThumbImgsFocus;
	
	int           	m_nMin, m_nMax, m_nStep;
	bool          	m_bRound_step;
	bool		  	m_bJump;
	bool			m_bUseCustomThumbs;

	int				SliderToClient(int value) const;
	int				ClientToSlider(int x) const;

	int				HoVe(int  x, int  y) const;
	int&			HoVeR(int& x, int& y) const;
	
	int				Min()	const	{ return Upp::min( m_nMin, m_nMax ); };
	int				Max()	const	{ return Upp::max( m_nMin, m_nMax ); };
	
	Vector<Ctrl*>	m_vctrlOutput;

	// eg a slider from -180 to 180, setting MajorTix to 30 would put Major tix
	// on the endpoints, as well as every 30, so: -180, -150, -120, -90, -60, -30, 0, 30...
	// Text Labels are drawn on all Major Tick points.
	// Setting MinorTix	to 5 would draw minor tix every 5, eg: -175, -170, -165...
	int				m_nMajorTicks;		// This is the "increment" for major tix
	int				m_nMinorTicks;		// This is the "increment" for minor tix
	int				m_nMajorTickSize; 	// Percent of the space from the line to the edge, eg 30
	int				m_nMinorTickSize; 	// Percent of the space from the line to the edge, eg 20
	TICK_ALIGNMENT	m_TextAlign, m_TickPosition;
	
protected:

	void			DrawTick( Draw &w, MAJORMINOR Type, HOVE Orientation, int nPos, int nVal );

public:
	typedef SliderCtrl CLASSNAME;

	Callback		WhenSlideFinish;
	Callback2<String&, int> LabelFormat;		
	
					SliderCtrlX();
	virtual 		~SliderCtrlX();

	void			Inc();
	void			Dec();

	SliderCtrlX&		MinMax(int _min, int _max);
	SliderCtrlX&		Range(int max)                  	{ return MinMax(0, max); }

	int				GetMin() const                  	{ return m_nMin; }
	int				GetMax() const                  	{ return m_nMax; }

	bool			IsVert() const;

	SliderCtrlX&		Step(int _step, bool _r = true) 	{ m_nStep = _step; m_bRound_step = _r; return *this; }
	int				GetStep() const                 	{ return m_nStep; }
	bool			IsRoundStep() const             	{ return m_bRound_step; }

	void			Jump(bool v = true)			  		{ m_bJump = v; }

	virtual void	Paint(Draw& draw);
	virtual bool	Key(dword key, int repcnt);
	virtual void	LeftDown(Point pos, dword keyflags);
	virtual void	LeftRepeat(Point pos, dword keyflags);
	virtual void	LeftUp(Point pos, dword keyflags);
	virtual void	MouseMove(Point pos, dword keyflags);
	virtual void	GotFocus();
	virtual void	LostFocus();

	virtual void	SetData(const Value& value);
	Value			SetValue( const Value& v, int nIndex = 0 );	
	virtual Value	GetData( int nIndex ) const;
	virtual Value	GetData() const;

	SliderCtrlX&		AddOutCtrl( Ctrl* c );	
	SliderCtrlX&		SetMajorTicks( int n )				{ m_nMajorTicks = n; return *this; };
	SliderCtrlX&		SetMinorTicks( int n )				{ m_nMinorTicks = n; return *this; };
	SliderCtrlX&		SetMajorTicksSize( int n )			{ m_nMajorTickSize = n; return *this; };
	SliderCtrlX&		SetMinorTicksSize( int n )			{ m_nMinorTickSize = n; return *this; };
	SliderCtrlX&		SetTickPosition( TICK_ALIGNMENT n )	{ m_TickPosition = n; return *this; };
	SliderCtrlX&		UseCustomThumbs( bool b = true )	{ m_bUseCustomThumbs = b; return *this; };
	SliderCtrlX&		SetCustomThumb( Image i, int nIndex = 0 ) { m_vThumbImgs.At( nIndex ) = i; m_vThumbImgsFocus.At( nIndex ) = i; return *this; };
};
