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
Painter& Painter::Close()
{
	CloseOp();
	return *this;
}

inline
Painter& Painter::Fill(const RGBA& color)
{
	FillOp(color);
	return *this;
}

inline
Painter& Painter::Stroke(double width, const RGBA& color)
{
	StrokeOp(width, color);
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

inline Painter& Painter::NoAA(bool noaa)
{
	NoAAOp(noaa);
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
