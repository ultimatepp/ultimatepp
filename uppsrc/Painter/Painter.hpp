inline
void Painter::Clear(const RGBA& color)
{
	ClearOp(color);
}

inline
Painter& Painter::Move(const Pointf& p, bool rel)
{
	MoveOp(p, rel);
	return *this;
}

inline
Painter& Painter::Line(const Pointf& p, bool rel)
{
	LineOp(p, rel);
	return *this;
}

inline
Painter& Painter::Quadratic(const Pointf& p1, const Pointf& p, bool rel)
{
	QuadraticOp(p1, p, rel);
	return *this;
}

inline
Painter& Painter::Quadratic(const Pointf& p, bool rel)
{
	QuadraticOp(p, rel);
	return *this;
}

inline
Painter& Painter::Cubic(const Pointf& p1, const Pointf& p2, const Pointf& p, bool rel)
{
	CubicOp(p1, p2, p, rel);
	return *this;
}

inline
Painter& Painter::Cubic(const Pointf& p2, const Pointf& p, bool rel)
{
	CubicOp(p2, p, rel);
	return *this;
}

inline
Painter& Painter::Arc(const Pointf& c, const Pointf& r, double angle, double sweep, bool rel)
{
	ArcOp(c, r, angle, sweep, rel);
	return *this;
}

inline
Painter& Painter::SvgArc(const Pointf& r, double xangle, bool large, bool sweep, const Pointf& p, bool rel)
{
	SvgArcOp(r, xangle, large, sweep, p, rel);
	return *this;
}

inline
Painter& Painter::Close()
{
	CloseOp();
	return *this;
}

inline
Painter& Painter::Div()
{
	DivOp();
	return *this;
}

inline
Painter& Painter::Fill(const RGBA& color)
{
	FillOp(color);
	return *this;
}

inline
Painter& Painter::Fill(const Image& image, const Xform2D& transsrc, dword flags)
{
	FillOp(image, transsrc, flags);
	return *this;
}

inline
Painter& Painter::Fill(const Pointf& p1, const RGBA& color1, const Pointf& p2, const RGBA& color2, int style)
{
	FillOp(p1, color1, p2, color2, style);
	return *this;
}

inline
Painter& Painter::Fill(const Pointf& f, const RGBA& color1, const Pointf& c, double r, const RGBA& color2, int style)
{
	FillOp(f, color1, c, r, color2, style);
	return *this;
}

inline
Painter& Painter::Stroke(double width, const RGBA& color)
{
	StrokeOp(width, color);
	return *this;
}

inline
Painter& Painter::Stroke(double width, const Image& image, const Xform2D& transsrc, dword flags)
{
	StrokeOp(width, image, transsrc, flags);
	return *this;
}

inline
Painter& Painter::Stroke(double width, const Pointf& p1, const RGBA& color1, const Pointf& p2, const RGBA& color2, int style)
{
	StrokeOp(width, p1, color1, p2, color2, style);
	return *this;
}

inline
Painter& Painter::Stroke(double width, const Pointf& f, const RGBA& color1, const Pointf& c, double r, const RGBA& color2, int style)
{
	StrokeOp(width, f, color1, c, r, color2, style);
	return *this;
}

inline
Painter& Painter::Clip()
{
	ClipOp();
	return *this;
}

inline Painter& Painter::ColorStop(double pos, const RGBA& color)
{
	ColorStopOp(pos, color);
	return *this;
}

inline Painter& Painter::ClearStops()
{
	ClearStopsOp();
	return *this;
}

inline Painter& Painter::Opacity(double o)
{
	OpacityOp(o);
	return *this;
}

inline Painter& Painter::LineCap(int linecap)
{
	LineCapOp(linecap);
	return *this;
}

inline Painter& Painter::LineJoin(int linejoin)
{
	LineJoinOp(linejoin);
	return *this;
}

inline Painter& Painter::MiterLimit(double l)
{
	MiterLimitOp(l);
	return *this;
}

inline Painter& Painter::EvenOdd(bool evenodd)
{
	EvenOddOp(evenodd);
	return *this;
}

inline Painter& Painter::Invert(bool b)
{
	InvertOp(b);
	return *this;
}

inline
Painter& Painter::ImageFilter(int filter)
{
	ImageFilterOp(filter);
	return *this;
}

inline Painter& Painter::Dash(const Vector<double>& dash, double start)
{
	if(dash.GetCount() & 1) {
		Vector<double> dash1;
		dash1.Append(dash);
		dash1.Append(dash);
		DashOp(dash1, start);
	}
	else
		DashOp(dash, start);
	return *this;
}

inline Painter& Painter::Dash(const char *dash, double start)
{
	DashOp(dash, start);
	return *this;
}

inline Painter& Painter::Transform(const Xform2D& m)
{
	TransformOp(m);
	return *this;
}

inline void Painter::Begin()
{
	BeginOp();
}

inline void Painter::End()
{
	EndOp();
}

inline void Painter::BeginMask()
{
	BeginMaskOp();
}

inline void Painter::BeginOnPath(double q, bool abs)
{
	BeginOnPathOp(q, abs);
}

inline Painter& Painter::Character(const Pointf& p, int ch, Font fnt)
{
	CharacterOp(p, ch, fnt);
	return *this;
}

inline
Painter& Painter::Text(const Pointf& p, const wchar *text, Font fnt, int n, const double *dx)
{
	TextOp(p, text, fnt, n, dx);
	return *this;
}

inline
Painter& Painter::Move(const Pointf& p)
{
	Move(p, false);
	return *this;
}

inline
Painter& Painter::Line(const Pointf& p)
{
	Line(p, false);
	return *this;
}

inline
Painter& Painter::Quadratic(const Pointf& p1, const Pointf& p)
{
	QuadraticOp(p1, p, false);
	return *this;
}

inline
Painter& Painter::Quadratic(const Pointf& p)
{
	QuadraticOp(p, false);
	return *this;
}

inline
Painter& Painter::Cubic(const Pointf& p1, const Pointf& p2, const Pointf& p)
{
	CubicOp(p1, p2, p, false);
	return *this;
}

inline
Painter& Painter::Cubic(const Pointf& p2, const Pointf& p)
{
	CubicOp(p2, p, false);
	return *this;
}

inline
Painter& Painter::RelMove(const Pointf& p)
{
	MoveOp(p, true);
	return *this;
}

inline
Painter& Painter::RelLine(const Pointf& p)
{
	LineOp(p, true);
	return *this;
}

inline
Painter& Painter::RelQuadratic(const Pointf& p1, const Pointf& p)
{
	QuadraticOp(p1, p, true);
	return *this;
}

inline
Painter& Painter::RelQuadratic(const Pointf& p)
{
	QuadraticOp(p, true);
	return *this;
}

inline
Painter& Painter::RelCubic(const Pointf& p1, const Pointf& p2, const Pointf& p)
{
	CubicOp(p1, p2, p, true);
	return *this;
}

inline
Painter& Painter::RelCubic(const Pointf& p2, const Pointf& p)
{
	CubicOp(p2, p, true);
	return *this;
}

inline
Painter& Painter::Move(double x, double y, bool rel)
{
	MoveOp(Pointf(x, y), rel);
	return *this;
}

inline
Painter& Painter::Line(double x, double y, bool rel)
{
	LineOp(Pointf(x, y), rel);
	return *this;
}

inline
Painter& Painter::Quadratic(double x1, double y1, double x, double y, bool rel)
{
	QuadraticOp(Pointf(x1, y1), Pointf(x, y), rel);
	return *this;
}

inline
Painter& Painter::Quadratic(double x, double y, bool rel)
{
	QuadraticOp(Pointf(x, y), rel);
	return *this;
}

inline
Painter& Painter::Cubic(double x1, double y1, double x2, double y2, double x, double y, bool rel)
{
	CubicOp(Pointf(x1, y1), Pointf(x2, y2), Pointf(x, y), rel);
	return *this;
}

inline
Painter& Painter::Cubic(double x2, double y2, double x, double y, bool rel)
{
	CubicOp(Pointf(x2, y2), Pointf(x, y), rel);
	return *this;
}

inline
Painter& Painter::Move(double x, double y)
{
	Move(x, y, false);
	return *this;
}

inline
Painter& Painter::Line(double x, double y)
{
	Line(x, y, false);
	return *this;
}

inline
Painter& Painter::Quadratic(double x1, double y1, double x, double y)
{
	Quadratic(x1, y1, x, y, false);
	return *this;
}

inline
Painter& Painter::Quadratic(double x, double y)
{
	Quadratic(x, y, false);
	return *this;
}

inline
Painter& Painter::Cubic(double x1, double y1, double x2, double y2, double x, double y)
{
	Cubic(x1, y1, x2, y2, x, y, false);
	return *this;
}

inline
Painter& Painter::Cubic(double x2, double y2, double x, double y)
{
	Cubic(x2, y2, x, y, false);
	return *this;
}

inline
Painter& Painter::RelMove(double x, double y)
{
	Move(x, y, true);
	return *this;
}

inline
Painter& Painter::RelLine(double x, double y)
{
	Line(x, y, true);
	return *this;
}

inline
Painter& Painter::RelQuadratic(double x1, double y1, double x, double y)
{
	Quadratic(x1, y1, x, y, true);
	return *this;
}

inline
Painter& Painter::RelQuadratic(double x, double y)
{
	Quadratic(x, y, true);
	return *this;
}

inline
Painter& Painter::RelCubic(double x1, double y1, double x2, double y2, double x, double y)
{
	Cubic(x1, y1, x2, y2, x, y, true);
	return *this;
}

inline
Painter& Painter::RelCubic(double x2, double y2, double x, double y)
{
	Cubic(x2, y2, x, y, true);
	return *this;
}

inline
Painter& Painter::Arc(const Pointf& c, double rx, double ry, double angle, double sweep, bool rel)
{
	return Arc(c, Pointf(rx, ry), angle, sweep, rel);
}

inline
Painter& Painter::Arc(const Pointf& c, double r, double angle, double sweep, bool rel)
{
	return Arc(c, Pointf(r, r), angle, sweep, rel);
}

inline
Painter& Painter::Arc(double x, double y, double rx, double ry, double angle, double sweep, bool rel)
{
	return Arc(Pointf(x, y), rx, ry, angle, sweep, rel);
}

inline
Painter& Painter::Arc(double x, double y, double r, double angle, double sweep, bool rel)
{
	return Arc(Pointf(x, y), r, angle, sweep, rel);
}

inline
Painter& Painter::Arc(const Pointf& c, const Pointf& r, double angle, double sweep)
{
	return Arc(c, r, angle, sweep, false);
}

inline
Painter& Painter::Arc(const Pointf& c, double rx, double ry, double angle, double sweep)
{
	return Arc(c, rx, ry, angle, sweep, false);
}

inline
Painter& Painter::Arc(const Pointf& c, double r, double angle, double sweep)
{
	return Arc(c, r, angle, sweep, false);
}

inline
Painter& Painter::Arc(double x, double y, double rx, double ry, double angle, double sweep)
{
	return Arc(x, y, rx, ry, angle, sweep, false);
}

inline
Painter& Painter::Arc(double x, double y, double r, double angle, double sweep)
{
	return Arc(x, y, r, angle, sweep, false);
}


inline
Painter& Painter::RelArc(const Pointf& c, const Pointf& r, double angle, double sweep)
{
	return Arc(c, r, angle, sweep, true);
}

inline
Painter& Painter::RelArc(const Pointf& c, double rx, double ry, double angle, double sweep)
{
	return Arc(c, rx, ry, angle, sweep, true);
}

inline
Painter& Painter::RelArc(const Pointf& c, double r, double angle, double sweep)
{
	return Arc(c, r, angle, sweep, true);
}

inline
Painter& Painter::RelArc(double x, double y, double rx, double ry, double angle, double sweep)
{
	return Arc(x, y, rx, ry, angle, sweep, true);
}

inline
Painter& Painter::RelArc(double x, double y, double r, double angle, double sweep)
{
	return Arc(x, y, r, angle, sweep, true);
}

inline
Painter& Painter::SvgArc(double rx, double ry, double xangle, bool large, bool sweep, const Pointf& p, bool rel)
{
	return SvgArc(Pointf(rx, ry), xangle, large, sweep, p, rel);
}

inline
Painter& Painter::SvgArc(double rx, double ry, double xangle, bool large, bool sweep, double x, double y, bool rel)
{
	return SvgArc(Pointf(rx, ry), xangle, large, sweep, Pointf(x, y), rel);
}

inline
Painter& Painter::SvgArc(const Pointf& r, double xangle, bool large, bool sweep, const Pointf& p)
{
	return SvgArc(r, xangle, large, sweep, p, false);
}

inline
Painter& Painter::SvgArc(double rx, double ry, double xangle, bool large, bool sweep, const Pointf& p)
{
	return SvgArc(rx, ry, xangle, large, sweep, p, false);
}

inline
Painter& Painter::SvgArc(double rx, double ry, double xangle, bool large, bool sweep, double x, double y)
{
	return SvgArc(rx, ry, xangle, large, sweep, x, y, false);
}

inline
Painter& Painter::RelSvgArc(const Pointf& r, double xangle, bool large, bool sweep, const Pointf& p)
{
	return SvgArc(r, xangle, large, sweep, p, true);
}

inline
Painter& Painter::RelSvgArc(double rx, double ry, double xangle, bool large, bool sweep, const Pointf& p)
{
	return SvgArc(rx, ry, xangle, large, sweep, p, true);
}

inline
Painter& Painter::RelSvgArc(double rx, double ry, double xangle, bool large, bool sweep, double x, double y)
{
	return SvgArc(rx, ry, xangle, large, sweep, x, y, true);
}
