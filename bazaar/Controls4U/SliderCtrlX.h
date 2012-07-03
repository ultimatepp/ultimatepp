NAMESPACE_UPP

class SliderCtrlX : public Ctrl {
	static const int BORDER_SIZE = 2;
	static const int BORDER1 = BORDER_SIZE;
	static const int BORDER2 = 2 * BORDER_SIZE;
	
	typedef enum { HORZ, VERT } HOVE;
	
	typedef enum { MAJOR, MINOR } MAJORMINOR;

public:
	typedef enum { TOP, MIDDLE_TOP, MIDDLE_BOTTOM, BOTTOM, 
		RIGHT = TOP, LEFT = BOTTOM, MIDDLE_LEFT = MIDDLE_BOTTOM, MIDDLE_RIGHT = MIDDLE_TOP } TICK_ALIGNMENT;
		
	typedef enum {CONTROL, INDICATOR} SLIDERTYPES;

	typedef enum {TYPE_0, TYPE_1, TYPE_2, TYPE_3, TYPE_4} THUMB_TYPES;

private:
	int				border2;			//
	bool			m_bInverted;		// True if origin is right or down 
	bool		  	m_bJump;			// Jumps directly to mouse pos if click on the scale
	bool          	m_bRound_step;		// Uses the defined Step
	bool			m_bUseCustomThumbs;
	Color			m_FillColor;		// what color to use for filling the slider
	int           	m_nMax;
	// eg a slider from -180 to 180, setting MajorTix to 30 would put Major tix
	// on the endpoints, as well as every 30, so: -180, -150, -120, -90, -60, -30, 0, 30...
	// Text Labels are drawn on all Major Tick points.
	// Setting MinorTix	to 5 would draw minor tix every 5, eg: -175, -170, -165...
	int				m_nMajorTicks;		// This is the "increment" for major tix
	int				m_nMajorTickSize; 	// Percent of the space from the line to the edge, eg 30
	int           	m_nMin;
	int				m_nMinorTicks;		// This is the "increment" for minor tix
	int				m_nMinorTickSize; 	// Percent of the space from the line to the edge, eg 20
	int				m_nSliderType;		// either a CONTROL / INDICATOR
	int           	m_nStep;			// Value of steps to inc/dec
	int				m_nTextAlign;
	int				m_nThickness;		// Thickness of the thumb guide
	int				m_nThumbNumber;		// Number of the thumb
	int				m_nThumbType;		// Index of thumb in Image list
	int				m_nTickPosition;
	Image			m_ThumbImg;			// Image of the thumb
	Image			m_ThumbFImg;		// Image of the focused thumb
	Size			m_ThumbSize;		// Size of the thumb;
	Vector<Ctrl*>	m_vctrlOutput;
	Vector<int>		m_vValues;
	Vector<Image>	m_vThumbImgs;
	Vector<Image>	m_vThumbImgsFocus;
	int				nHalfThumb;			
	
	int				ClientToSlider(int x) const;
	int				HoVe(int  x, int  y) const;
	int&			HoVeR(int& x, int& y) const;
	int				Max()	const	{ return Upp::max( m_nMin, m_nMax ); };
	int				Min()	const	{ return Upp::min( m_nMin, m_nMax ); };
	int				SliderToClient(int value) const;

protected:

	void			DrawTick( Draw &w, MAJORMINOR Type, HOVE Orientation, int nPos, int nVal );

public:
	typedef SliderCtrl CLASSNAME;

	Callback		WhenSlideFinish;
	Callback2<String&, int> LabelFormat;		
	
					SliderCtrlX();
	virtual 		~SliderCtrlX();

	SliderCtrlX&	AddOutCtrl( Ctrl* c );	
	void			Dec();
	SliderCtrlX&	FillColor( Color c )				{ m_FillColor = c; return *this; };
	virtual Value	GetData() const;
	virtual Value	GetData( int nIndex ) const;
	int				GetMax() const                  	{ return m_nMax; }
	int				GetMin() const                  	{ return m_nMin; }
	int				GetStep() const                 	{ return m_nStep; }
	virtual void	GotFocus();
	void			Inc();
	bool			IsInverted() const					{ return m_bInverted; };
	bool			IsRoundStep() const             	{ return m_bRound_step; }
	bool			IsVert() const;
	SliderCtrlX&	Jump(bool v = false)		  		{ m_bJump = v; return *this; }
	virtual bool	Key(dword key, int repcnt);
	virtual void	LeftDown(Point pos, dword keyflags);
	virtual void	LeftRepeat(Point pos, dword keyflags);
	virtual void	LeftUp(Point pos, dword keyflags);
	virtual void	LostFocus();
	virtual void	MouseMove(Point pos, dword keyflags);
	SliderCtrlX&	MinMax(int _min, int _max);
	virtual void	Paint(Draw& draw);
	SliderCtrlX&	Range(int max)                  	{ return MinMax(0, max); }
	SliderCtrlX&	SetCustomThumb( Image i, int nIndex = 0 ) { m_vThumbImgs.At( nIndex ) = i; m_vThumbImgsFocus.At( nIndex ) = i; return *this; };
	virtual void	SetData(const Value& value);
	SliderCtrlX&	SetInverted(bool b)					{ m_bInverted = b; return *this; };
	SliderCtrlX&	SetMajorTicks( int n )				{ m_nMajorTicks = n; return *this; };
	SliderCtrlX&	SetMajorTicksSize( int n )			{ m_nMajorTickSize = n; return *this; };
	SliderCtrlX&	SetMaxi( int n=100 )				{ m_nMax = n; return *this; };
	SliderCtrlX&	SetMini( int n=0 )					{ m_nMin = n; return *this; };
	SliderCtrlX&	SetMinorTicks( int n )				{ m_nMinorTicks = n; return *this; };
	SliderCtrlX&	SetMinorTicksSize( int n )			{ m_nMinorTickSize = n; return *this; };
	SliderCtrlX&	SetSliderType(int n=0)				{ m_nSliderType = n; return *this;};
	SliderCtrlX&	SetThickness( int n = 2 )			{ m_nThickness = n; border2 = m_nThickness * BORDER_SIZE; return *this; };
	SliderCtrlX&	SetTickPosition(int n )				{ m_nTickPosition = n; return *this; };
	Value			SetValue( const Value& v, int nIndex = 0 );
	SliderCtrlX&	Step(int _step, bool _r = true) 	{ m_nStep = _step; m_bRound_step = _r; return *this; }
	SliderCtrlX&	SetThumbType(int n=0);
	SliderCtrlX&	UseCustomThumbs( bool b = true )	{ m_bUseCustomThumbs = b; return *this; };
};

END_UPP_NAMESPACE