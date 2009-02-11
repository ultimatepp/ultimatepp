Pointf Mid(const Pointf& a, const Pointf& b);
Pointf Orthogonal(const Pointf& p);
double Squared(const Pointf& p);
double Length(const Pointf& p);
double Bearing(const Pointf& p);
double Distance(const Pointf& p1, const Pointf& p2);
double SquaredDistance(const Pointf& p1, const Pointf& p2);
Pointf Polar(double a);
Pointf Polar(const Pointf& p, double r, double a);

struct LinearPathConsumer {
	virtual void Move(const Pointf& p) = 0;
	virtual void Line(const Pointf& p) = 0;
	virtual void End();
};

void ApproximateQuadratic(LinearPathConsumer& t,
                          const Pointf& p1, const Pointf& p2, const Pointf& p3,
                          double tolerance);
void ApproximateCubic(LinearPathConsumer& t,
                      const Pointf& x0, const Pointf& x1, const Pointf& x2, const Pointf& x,
                      double tolerance);
void ApproximateArc(LinearPathConsumer& t, const Pointf& p, const Pointf& r,
                    double angle, double sweep, double tolerance);

struct LinearPathFilter : LinearPathConsumer {
	virtual void End();

	LinearPathConsumer *target;

	void PutMove(const Pointf& p)               { target->Move(p); }
	void PutLine(const Pointf& p)               { target->Line(p); }
	void PutEnd()                               { target->End(); }
};

class Stroker : public LinearPathFilter {
public:
	virtual void Move(const Pointf& p);
	virtual void Line(const Pointf& p);
	virtual void End();

private:
	double w2;
	double qmiter;
	double fid;

	Pointf p0, v0, o0, a0, b0;
	Pointf p1, v1, o1, a1, b1;
	Pointf p2;
	int    linecap;
	int    linejoin;
	
	void   Finish();
	void   Round(const Pointf& p, const Pointf& v1, const Pointf& v2, double r);
	void   Cap(const Pointf& p0, const Pointf& v0, const Pointf& o0,
	           const Pointf& a0, const Pointf& b0);

public:	
	void Init(double width, double miterlimit, double tolerance, int linecap, int linejoin);
};

class Dasher : public LinearPathFilter {
public:
	virtual void Move(const Pointf& p);
	virtual void Line(const Pointf& p);

private:
	const Vector<double> *pattern;
	int            patterni;
	double         sum, rem;
	bool           flag;
	Pointf         p0;

	void    Put(const Pointf& p);

public:
	void Init(const Vector<double>& pattern, double distance);
};

struct Transformer : public LinearPathFilter {
public:
	virtual void Move(const Pointf& p);
	virtual void Line(const Pointf& p);

private:
	const Xform2D& xform;

public:
	Transformer(const Xform2D& xform) : xform(xform) {}
};

inline RGBA Mul8(const RGBA& s, int mul)
{
	RGBA t;
	t.r = (mul * s.r) >> 8;
	t.g = (mul * s.g) >> 8;
	t.b = (mul * s.b) >> 8;
	t.a = (mul * s.a) >> 8;
	return t;
}

inline void AlphaBlend(RGBA& t, const RGBA& c)
{
	int alpha = 256 - (c.a + (c.a >> 7));
	t.r = c.r + (alpha * t.r >> 8);
	t.g = c.g + (alpha * t.g >> 8);
	t.b = c.b + (alpha * t.b >> 8);
	t.a = c.a + (alpha * t.a >> 8);
}

inline void AlphaBlendCover8(RGBA& t, const RGBA& c, int cover)
{
	int a = c.a * cover >> 8;
	int alpha = 256 - (a + (a >> 7));
	t.r = (c.r * cover >> 8) + (alpha * t.r >> 8);
	t.g = (c.g * cover >> 8) + (alpha * t.g >> 8);
	t.b = (c.b * cover >> 8) + (alpha * t.b >> 8);
	t.a = a + (alpha * t.a >> 8);
}

inline int Q8(double x)
{
	return int(x * 256 + 0.5);
}

class Rasterizer : public LinearPathConsumer {
public:
	virtual void Move(const Pointf& p);
	virtual void Line(const Pointf& p);

private:
	struct Cell : MoveableWithSwap<Cell> {
		int16 x;
		int16 cover;
		int   area;

		bool operator<(const Cell& b) const { return x < b.x; }
    };

	Rectf                   cliprect;
	Pointf                  p0;
	Buffer< Vector<Cell> >  cell;
	int                     min_y;
	int                     max_y;
	Size                    sz;

	void  Init();
	Cell *AddCells(int y, int n);
	void  RenderHLine(int ey, int x1, int y1, int x2, int y2);
	void  LineClip(double x1, double y1, double x2, double y2);
	int   CvX(double x);
	int   CvY(double y);
	void  CvLine(double x1, double y1, double x2, double y2);
	bool  BeginRender(int y, const Cell *&c, const Cell *&e);

public:
	struct Target {
		virtual void Start(int x, int len) = 0;
		virtual void Render(int val) = 0;
		virtual void Render(int val, int len) = 0;
	};

	void LineRaw(int x1, int y1, int x2, int y2);
	
	void SetClip(const Rectf& rect);

	int  MinY() const                         { return min_y; }
	int  MaxY() const                         { return max_y; }
	void Render(int y, Target& g, bool evenodd);

	void Reset();
	
	Rasterizer(int cx, int cy);
};

struct ScanLine {
	int          xmin, xmax;
	Buffer<byte> data;
	int          datalen;

	bool IsFull();
	bool IsEmpty();

	String ToString() const;
};

struct SpanSource {
	virtual void Get(RGBA *span, int x, int y, unsigned len) = 0;
};

struct SolidFiller : Rasterizer::Target {
	RGBA *t;
	RGBA  c;
	
	void Start(int minx, int maxx)      { t += minx; }
	void Render(int val)                { AlphaBlendCover8(*t++, c, val); } 
	void Render(int val, int len);
};

struct SpanFiller : Rasterizer::Target {
	RGBA       *t;
	const RGBA *s;
	int         y;
	RGBA       *buffer;
	SpanSource *ss;
	int         alpha;
	
	void Start(int minx, int maxx);
	void Render(int val);
	void Render(int val, int len);
};

struct RecFiller : Rasterizer::Target {
	Buffer<byte> buffer;
	byte        *t;
	int          x;
	int          maxx;
	int          maxlen;
	int          cx;
	bool         empty;
	
	void Span(int c, int len);

	virtual void Render(int val);
	virtual void Render(int val, int len);
	virtual void Start(int x, int len);
	void Finish();
	void GetResult(Buffer<byte>& tgt, int& maxx, int& maxlen);
	
	RecFiller(int cx);
};

Image MipMap(const Image& img);
Image MakeMipMap(const Image& img, int level);

class LinearInterpolator {
	struct Dda2 {
		int count, lift, rem, mod, p;
		
		void  Set(int a, int b, int len);
		int   Get();
	};

	Xform2D xform;
	Dda2    ddax, dday;
	
public:
	void   Set(const Xform2D& m)                    { xform = m; }

	void   Begin(int x, int y, int len);
	Point  Get();
};

class BufferPainter : public Painter {
protected:
	virtual void   ClearOp(const RGBA& color);

	virtual void   MoveOp(const Pointf& p, bool rel);
	virtual void   LineOp(const Pointf& p, bool rel);
	virtual void   QuadraticOp(const Pointf& p1, const Pointf& p, bool rel);
	virtual void   QuadraticOp(const Pointf& p, bool rel);
	virtual void   CubicOp(const Pointf& p1, const Pointf& p2, const Pointf& p, bool rel);
	virtual void   CubicOp(const Pointf& p2, const Pointf& p, bool rel);
	virtual void   ArcOp(const Pointf& c, const Pointf& r, double angle, double sweep, bool rel);
	virtual void   CloseOp();
	virtual void   DivOp();

	virtual void   FillOp(const RGBA& color);
	virtual void   FillOp(const Image& image, const Xform2D& transsrc, dword flags);
	virtual void   FillOp(const Pointf& p1, const RGBA& color1,
	                      const Pointf& p2, const RGBA& color2,
	                      int style);
	virtual void   FillOp(const Pointf& f, const RGBA& color1, 
	                      const Pointf& c, double r, const RGBA& color2,
	                      int style);

	virtual void   StrokeOp(double width, const RGBA& rgba);
	virtual void   StrokeOp(double width, const Image& image, const Xform2D& transsrc,
	                        dword flags);
	virtual void   StrokeOp(double width, const Pointf& p1, const RGBA& color1,
	                        const Pointf& p2, const RGBA& color2,
	                        int style);
	virtual void   StrokeOp(double width, const Pointf& f, const RGBA& color1, 
	                        const Pointf& c, double r, const RGBA& color2,
	                        int style);

	virtual void   ClipOp();

	virtual void   ColorStopOp(double pos, const RGBA& color);
	virtual void   ClearStopsOp();
	
	virtual void   OpacityOp(double o);
	virtual void   LineCapOp(int linecap);
	virtual void   LineJoinOp(int linejoin);
	virtual void   MiterLimitOp(double l);
	virtual void   EvenOddOp(bool evenodd);
	virtual void   DashOp(const Vector<double>& dash, double start);
	virtual void   NoAAOp(bool noaa);

	virtual void   TransformOp(const Xform2D& m);

	virtual void   BeginOp();
	virtual void   EndOp();

public:
	enum {
		MOVE, LINE, QUADRATIC, CUBIC, ARC, DIV
	};
	struct LinearData {
		Pointf p;
	};	
	struct QuadraticData : LinearData {
		Pointf p1;
	};	
	struct CubicData : QuadraticData {
		Pointf p2;
	};
	struct ArcData : LinearData {
		Pointf r;
		double angle, sweep;
		
		Pointf EndPoint() const;
	};
	struct Path {	
		Vector<byte> type;
		Vector<byte> data;
	};
	struct Attr : Moveable<Attr> {
		Xform2D                         mtx;
		double                          tolerance;
		bool                            evenodd;
		byte                            join;
		byte                            cap;
		double                          miter_limit;
		WithDeepCopy< Vector<double> >  dash;
		WithDeepCopy< Vector<double> >  stop;
		WithDeepCopy< Vector<RGBA> >    stop_color;
		double                          dash_start;
		double                          opacity;
		int                             cliplevel;
		bool                            hasclip;
		bool                            mask;
		bool                            noaa;
	};
	
	ImageBuffer& ib;

	Attr         attr;
	Attr         pathattr;
	Array<Attr>  attrstack;

	Path         path;
	Pointf       current, ccontrol, qcontrol, move;
	Rectf        pathrect;
	
	Rasterizer   rasterizer;
	Buffer<RGBA> span;

	void        *PathAddRaw(int type, int size);
	template <class T> T& PathAdd(int type) { return *(T *)PathAddRaw(type, sizeof(T)); }

	Pointf       PathPoint(const Pointf& p, bool rel);
	Pointf       EndPoint(const Pointf& p, bool rel);
	void         DoMove0();
	void         ClearPath();
	void         RenderPath(double width, SpanSource *ss, const RGBA& color);
	void         RenderImage(double width, const Image& image, const Xform2D& transsrc, dword flags);
	void         RenderRadial(double width, const Pointf& f, const RGBA& color1,
	                          const Pointf& c, double r, const RGBA& color2, int style);
	void         MakeGradient(RGBA *t, RGBA color1, RGBA color2, int cx);
	Image        GetGradient(const RGBA& color1, const RGBA& color2, const Pointf& p1, const Pointf& p2);
	void         ColorStop0(Attr& a, double pos, const RGBA& color);

public:
	BufferPainter(ImageBuffer& ib);
};
