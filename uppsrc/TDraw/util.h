NAMESPACE_UPP

static const int    DOTS_PER_METER_INT = 23622;
static const double DOTS_PER_METER_DBL = 600e3 / 25.4;

SystemDraw&  ScreenInfo();

Size   GetPixelsPerMeter(const Draw& draw);
int    GetHorzPixelsPerMeter(const Draw& draw);
int    GetVertPixelsPerMeter(const Draw& draw);
int    GetAvgPixelsPerMeter(const Draw& draw);

double GetHorzPixelsPerDot(const Draw& draw);
double GetVertPixelsPerDot(const Draw& draw);
double GetAvgPixelsPerDot(const Draw& draw);

Font   GetPixelFont(Font font, const Draw& draw); // converts dot height to pixel height

int    DotsToPixels(const Draw& draw, int dots);
Size   DotsToPixels(const Draw& draw, Size size);
int    HorzDotsToPixels(const Draw& draw, int dots);
int    VertDotsToPixels(const Draw& draw, int dots);

int    PixelsToDots(const Draw& draw, int pixels);
Size   PixelsToDots(const Draw& draw, Size size);
int    HorzPixelsToDots(const Draw& draw, int dots);
int    VertPixelsToDots(const Draw& draw, int dots);

int    PixelsToPoints(const Draw& draw, int pixels);
Size   PixelsToPoints(const Draw& draw, Size pixels);
int    PointsToPixels(const Draw& draw, int points);
Size   PointsToPixels(const Draw& draw, Size points);

void   RGBtoHSV(Color c, double& h, double& s, double& v);
Color  HSVtoRGB(double h, double s, double v);

Color  GetColorGradient(Color c1, Color c2, int ratio1, int ratio2, double gamma = 2.5);

void   DrawDragDropRect(Draw& draw, const Rect& rc_old, const Rect& rc_new, int width, Color c1 = White, Color c2 = Black);
void   DrawDragDropRect(Draw& draw, const Rect& rc_old, const Rect& rc_new, Color c1 = White, Color c2 = Black);

void   PaintDragHorzLine(Draw& draw, const Rect& rc, Color c1, Color c2, Color bgnd, int mingap = 3);
void   PaintDragVertLine(Draw& draw, const Rect& rc, Color c1, Color c2, Color bgnd, int mingap = 3);
void   PaintDragRect(Draw& draw, const Rect& rc, Color c1, Color c2, Color bgnd, int width);

//void   DrawEllipse(Draw& draw, const Rect& rc, Color color, Color outline = Null, int outline_width = 0);
//void   DrawArc(Draw& draw, const Rect& rc, Point start, Point end, Color color, int width);

/*
void   DrawPolyPolyline(Draw& draw,
	const Point *vertices, int vertex_count, const int *counts, int count_count,
	int width = 0, Color color = Black, Color doxor = Null);
void   DrawPolyPolyline(Draw& draw,
	const Vector<Point>& vertices, const Vector<int>& counts,
	int width = 0, Color color = Black, Color doxor = Null);
void   DrawPolyline(Draw& draw, const Point *vertices, int count,
	int width = 0, Color color = Black, Color doxor = Null);
void   DrawPolyline(Draw& draw, const Vector<Point>& vertices,
	int width = 0, Color color = Black, Color doxor = Null);
*/

void   DrawPolyPolyPolygon(Draw& draw,
	const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
	Color color = Black, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
void   DrawPolyPolyPolygon(Draw& draw,
	const Vector<Point>& vertices, const Vector<int>& subpolygon_counts, const Vector<int>& disjunct_polygon_counts,
	Color color = Black, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
void   DrawPolyPolygon(Draw& draw,
	const Point *vertices, int vertex_count, const int *subpolygon_counts, int subpolygon_count_count,
	Color color = Black, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
void   DrawPolyPolygon(Draw& draw, const Vector<Point>& vertices, const Vector<int>& subpolygon_counts,
	Color color = Black, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
void   DrawPolygons(Draw& draw,
	const Point *vertices, int vertex_count, const int *polygon_counts, int polygon_count_count,
	Color color = Black, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
void   DrawPolygons(Draw& draw, const Vector<Point>& vertices, const Vector<int>& polygon_counts,
	Color color = Black, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
void   DrawPolygon(Draw& draw, const Point *vertices, int vertex_count,
	Color color = Black, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);
void   DrawPolygon(Draw& draw, const Vector<Point>& vertices,
	Color color = Black, int width = 0, Color outline = Null, uint64 pattern = 0, Color doxor = Null);

//////////////////////////////////////////////////////////////////////

inline PaintRect WhiteDisplay()   { return PaintRect(ColorDisplay(), White()); }
inline PaintRect BlackDisplay()   { return PaintRect(ColorDisplay(), Black()); }
inline PaintRect SWhiteDisplay()  { return PaintRect(ColorDisplay(), SWhite()); }
inline PaintRect SLtGrayDisplay() { return PaintRect(ColorDisplay(), SLtGray()); }
inline PaintRect SBlackDisplay()  { return PaintRect(ColorDisplay(), SBlack()); }

//////////////////////////////////////////////////////////////////////

/*
class ImageDisplay : public Display
{
public:
	enum LAYOUT { SINGLE, STRETCH, TILE };

	ImageDisplay(LAYOUT layout = SINGLE,
		Alignment halign = ALIGN_CENTER,
		Alignment valign = ALIGN_CENTER)
		: layout(layout), halign(halign), valign(valign) {}

	ImageDisplay&  Layout(LAYOUT _layout)    { layout = _layout; return *this; }
	ImageDisplay&  Single()                  { return Layout(SINGLE); }
	ImageDisplay&  Stretch()                 { return Layout(STRETCH); }
	ImageDisplay&  Tile()                    { return Layout(TILE); }
	LAYOUT         GetLayout() const         { return layout; }

	ImageDisplay&  HorzAlign(Alignment _a)   { halign = _a; return *this; }
	ImageDisplay&  Left()                    { return HorzAlign(ALIGN_LEFT); }
	ImageDisplay&  HCenter()                 { return HorzAlign(ALIGN_CENTER); }
	ImageDisplay&  Right()                   { return HorzAlign(ALIGN_RIGHT); }
	Alignment      GetHorzAlign() const      { return halign; }

	ImageDisplay&  VertAlign(Alignment _a)   { valign = _a; return *this; }
	ImageDisplay&  Top()                     { return VertAlign(ALIGN_TOP); }
	ImageDisplay&  VCenter()                 { return VertAlign(ALIGN_CENTER); }
	ImageDisplay&  Bottom()                  { return VertAlign(ALIGN_BOTTOM); }
	Alignment      GetVertAlign() const      { return valign; }

	ImageDisplay&  Center()                  { return HCenter().VCenter(); }

	virtual Size   GetStdSize(const Value& q) const;
	virtual void   Paint(Draw& w, const Rect& r, const Value& q, Color i, Color p, dword s) const;

protected:
	LAYOUT         layout;
	Alignment      halign, valign;
};
*/

//////////////////////////////////////////////////////////////////////

//const ImageDisplay& CenterImageDisplay();
//const ImageDisplay& StretchImageDisplay();
//const ImageDisplay& TileImageDisplay();

//////////////////////////////////////////////////////////////////////

class GradientDisplay : public Display
{
public:
	GradientDisplay() : a(0, 0), ca(White), b(-1, -1), cb(LtGray), gamma(2.5) {}
	GradientDisplay(Point a, Color ca, Point b, Color cb, double gamma = 2.5);

	void             Set(Point a_, Color ca_, Point b_, Color cb_) { a = a_; ca = ca_; b = b_; cb = cb_; }
	Point            GetPos1() const                               { return a; }
	Color            GetColor1() const                             { return ca; }
	Point            GetPos2() const                               { return b; }
	Color            GetColor2() const                             { return cb; }

	void             Gamma(double g)                               { gamma = g; }
	double           GetGamma() const                              { return gamma; }

	virtual void     Paint(Draw& w, const Rect& r, const Value& q, Color i, Color p, dword s) const;

protected:
	Point            a, b;
	Color            ca, cb;
	double           gamma;

	static Point     GetRectPoint(Point P, const Rect& rc);
};

//////////////////////////////////////////////////////////////////////

class PrinterInfo
{
public:
	PrinterInfo();

	operator bool () const { return page_size.cx > 0 && page_size.cy > 0; }

public:
#ifdef PLATFORM_WIN32
	const DEVMODE *devmode;
#endif
	String         driver;
	String         device;
	String         output;
	Size           page_size;   // dots

private:
	String         devmode_data;
};


//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
int GdiGetFreeSpace();
#endif

END_UPP_NAMESPACE
