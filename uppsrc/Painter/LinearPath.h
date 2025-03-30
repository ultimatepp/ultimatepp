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
	double tw;

	Pointf p0, v0, o0, a0, b0;
	Pointf p1, v1, o1, a1, b1;
	Pointf p2;
	int    linecap;
	int    linejoin;
	Rectf  preclip;
	int    lines;
	
	void   Finish();
	void   Round(const Pointf& p, const Pointf& v1, const Pointf& v2, double r);
	void   Cap(const Pointf& p0, const Pointf& v0, const Pointf& o0,
	           const Pointf& a0, const Pointf& b0);
	bool   PreClipped(Pointf p2, Pointf p3);

public:
	void Init(double width, double miterlimit, double tolerance, int linecap, int linejoin, const Rectf& preclip);
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

class Rasterizer : public LinearPathConsumer {
public:
	virtual void Move(const Pointf& p);
	virtual void Line(const Pointf& p);

private:
	struct Cell : Moveable<Cell> {
		int16 x;
		int16 cover;
		int   area;

		bool operator<(const Cell& b) const { return x < b.x; }
    };
    
    enum { SVO_ALLOC = 15 };
    
	struct CellArray {
		int    count;
		int    alloc;
		union {
			Cell *ptr;
			Cell  svo[SVO_ALLOC];
		};
		
		Cell *Get()         { return alloc == SVO_ALLOC ? svo : ptr; }
		
		CellArray()         { count = 0; alloc = SVO_ALLOC; }
	};

	Rectf                   cliprect;
	Pointf                  p0;
	Buffer<CellArray>       cell;

	int                     min_y;
	int                     max_y;
	Size                    sz;
	int                     mx;

	void  Init();
	Cell *AddCells(int y, int n);
	void  AddCells2(CellArray& a, int n);
	void  RenderHLine(int ey, int x1, int y1, int x2, int y2);
	void  LineClip(double x1, double y1, double x2, double y2);
	int   CvX(double x);
	int   CvY(double y);
	void  CvLine(double x1, double y1, double x2, double y2);
	bool  BeginRender(int y, const Cell *&c, const Cell *&e);
	void  Free();

	static int Q8Y(double y) { return int(y * 256 + 0.5); }
	int Q8X(double x)        { return int(x * mx + 0.5); }

public:
	struct Filler {
		virtual void Start(int x, int len) = 0;
		virtual void Render(int val) = 0;
		virtual void Render(int val, int len) = 0;
		virtual void End();
	};

	void LineRaw(int x1, int y1, int x2, int y2);
	
	void  SetClip(const Rectf& rect);
	Rectf GetClip() const                     { return cliprect; }

	bool IsValid() const                      { return min_y < max_y; }
	int  MinY() const                         { return min_y; }
	int  MaxY() const                         { return max_y; }
	bool NotEmpty(int y)                      { return cell[y].count; }
	void Render(int y, Filler& g, bool evenodd);

	void Reset();

	void Create(int cx, int cy, bool subpixel);
	
	Rasterizer(int cx, int cy, bool subpixel) { Create(cx, cy, subpixel); }
	Rasterizer()                              { sz = Size(0, 0); }
	~Rasterizer()                             { Free(); }
};

void ApproximateChar(LinearPathConsumer& t, Pointf at, int ch, Font fnt, double tolerance);
