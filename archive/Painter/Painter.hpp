NAMESPACE_UPP

inline void Painter::Clear(const RGBA& color)
{
	ClearOp(color);
}

inline Painter& Painter::Move(double x, double y, bool rel)
{
	MoveOp(x, y, rel);
	return *this;
}

inline Painter& Painter::Line(double x, double y, bool rel)
{
	LineOp(x, y, rel);
	return *this;
}

inline Painter& Painter::Quadratic(double x1, double y1, double x, double y, bool rel)
{
	QuadraticOp(x1, y1, x, y, rel);
	return *this;
}

inline Painter& Painter::Quadratic(double x, double y, bool rel)
{
	QuadraticOp(x, y, rel);
	return *this;
}

inline Painter& Painter::Cubic(double x1, double y1, double x2, double y2, double x, double y, bool rel)
{
	CubicOp(x1, y1, x2, y2, x, y, rel);
	return *this;
}

inline Painter& Painter::Cubic(double x2, double y2, double x, double y, bool rel)
{
	CubicOp(x2, y2, x, y, rel);
	return *this;
}

inline Painter& Painter::Close()
{
	CloseOp();
	return *this;
}

inline Painter& Painter::Fill(const RGBA& color)
{
	FillOp(color);
	return *this;
}

inline Painter& Painter::Fill(const Image& image, const Matrix2D& transsrc, dword flags)
{
	FillOp(image, transsrc, flags);
	return *this;
}

inline Painter& Painter::Fill(double x1, double y1, const RGBA& color1,
                       double x2, double y2, const RGBA& color2, int style)
{
	FillOp(x1, y1, color1, x2, y2, color2, style);
	return *this;
}

inline Painter& Painter::Fill(double fx, double fy, const RGBA& color1, double x1, double y1, double r, const RGBA& color2, int style)
{
	FillOp(fx, fy, color1, x1, y1, r, color2, style);
	return *this;
}

inline Painter& Painter::Fill(double x1, double y1, const RGBA& color1, double r, const RGBA& color2, int style)
{
	return Fill(x1, y1, color1, x1, y1, r, color2, style);
}

inline Painter& Painter::Stroke(double width, const RGBA& color)
{
	StrokeOp(width, color);
	return *this;
}

inline Painter& Painter::Stroke(double width, const Image& image, const Matrix2D& transsrc, dword flags)
{
	StrokeOp(width, image, transsrc, flags);
	return *this;
}

inline Painter& Painter::Stroke(double width, double x1, double y1, const RGBA& color1,
                         double x2, double y2, const RGBA& color2, int style)
{
	StrokeOp(width, x1, y1, color1, x2, y2, color2, style);
	return *this;
}

inline Painter& Painter::Stroke(double width, double fx, double fy, const RGBA& color1,
                         double x1, double y1, double r, const RGBA& color2, int style)
{
	StrokeOp(width, fx, fy, color1, x1, y1, r, color2, style);
	return *this;
}

inline Painter& Painter::Stroke(double width, double x1, double y1, const RGBA& color1, double r, const RGBA& color2, int style)
{
	return Stroke(width, x1, y1, color1, x1, y1, r, color2, style);
}

inline Painter& Painter::Clip()
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

inline void Painter::Transform(const Matrix2D& m)
{
	TransformOp(m);
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

END_UPP_NAMESPACE
