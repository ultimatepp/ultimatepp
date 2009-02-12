enum {
	PAINTING_EOF,
	PAINTING_CLEAR,
	
	PAINTING_MOVE,
	PAINTING_MOVE_REL,
	PAINTING_LINE,
	PAINTING_LINE_REL,
	PAINTING_QUADRATIC,
	PAINTING_QUADRATIC_REL,
	PAINTING_QUADRATIC_S,
	PAINTING_QUADRATIC_S_REL,
	PAINTING_CUBIC,
	PAINTING_CUBIC_REL,
	PAINTING_CUBIC_S,
	PAINTING_CUBIC_S_REL,
	PAINTING_CLOSE,
	
	PAINTING_FILL_SOLID,
	PAINTING_FILL_IMAGE,
	PAINTING_FILL_GRADIENT,
	PAINTING_FILL_RADIAL,
	
	PAINTING_STROKE_SOLID,
	PAINTING_STROKE_IMAGE,
	PAINTING_STROKE_GRADIENT,
	PAINTING_STROKE_RADIAL,
	
	PAINTING_CLIP,
	
	PAINTING_CHARACTER,
	PAINTING_TEXT,
	
	PAINTING_COLORSTOP,
	PAINTING_CLEARSTOPS,
	PAINTING_OPACITY,
	PAINTING_LINECAP,
	PAINTING_LINEJOIN,
	PAINTING_MITERLIMIT,
	PAINTING_EVENODD,
	PAINTING_DASH,
	PAINTING_NOAA,
	
	PAINTING_TRANSFORM,
	PAINTING_BEGIN,
	PAINTING_END,
	PAINTING_BEGINMASK,
};

class PaintingPainter : public Painter {
	StringStream cmd;
	ValueArray   data;
	Sizef        size;

	void Put(int c)                { cmd.Put(c); }
	void Put32(int c)              { cmd.Put32(c); }
	void Put(const RGBA& c)        { cmd.Put(&c, sizeof(RGBA)); }
	void Putf(double d)            { cmd.Put(&d, sizeof(double)); }
	void Putf(double x, double y)  { Putf(x); Putf(y); }
	void Putf(const Matrix2D& m)   { cmd.Put(&m, sizeof(Matrix2D)); }
	void Put(const Font& f)        { cmd.Put(&f, sizeof(Font)); }

protected:
	virtual void   ClearOp(const RGBA& color);

	virtual void   MoveOp(double x, double y, bool rel);
	virtual void   LineOp(double x, double y, bool rel);
	virtual void   QuadraticOp(double x1, double y1, double x, double y, bool rel);
	virtual void   QuadraticOp(double x, double y, bool rel);
	virtual void   CubicOp(double x1, double y1, double x2, double y2, double x, double y, bool rel);
	virtual void   CubicOp(double x2, double y2, double x, double y, bool rel);
	virtual void   CloseOp();

	virtual void   FillOp(const RGBA& color);
	virtual void   FillOp(const Image& image, const Matrix2D& transsrc, dword flags);
	virtual void   FillOp(double x1, double y1, const RGBA& color1,
	                      double x2, double y2, const RGBA& color2,
	                      int style);
	virtual void   FillOp(double fx, double fy, const RGBA& color1, 
	                      double x1, double y1, double r, const RGBA& color2,
	                      int style);
	
	virtual void   StrokeOp(double width, const RGBA& rgba);
	virtual void   StrokeOp(double width, const Image& image, const Matrix2D& transsrc,
	                        dword flags);
	virtual void   StrokeOp(double width, double x1, double y1, const RGBA& color1,
	                        double x2, double y2, const RGBA& color2,
	                        int style);
	virtual void   StrokeOp(double width, double fx, double fy, const RGBA& color1, 
	                        double x, double y, double r, const RGBA& color2,
	                        int style);

	virtual void   ClipOp();

	virtual void   CharacterOp(double x, double y, int ch, Font fnt);
	virtual void   TextOp(double x, double y, const wchar *text, Font fnt, int n = -1, double *dx = NULL);
	
	virtual void   ColorStopOp(double pos, const RGBA& color);
	virtual void   ClearStopsOp();
	
	virtual void   OpacityOp(double o);
	virtual void   LineCapOp(int linecap);
	virtual void   LineJoinOp(int linejoin);
	virtual void   MiterLimitOp(double l);
	virtual void   EvenOddOp(bool evenodd);
	virtual void   DashOp(const Vector<double>& dash, double start);
	virtual void   NoAAOp(bool noaa);

	virtual void   TransformOp(const Matrix2D& m);

	virtual void   BeginOp();
	virtual void   EndOp();
	
	virtual void   BeginMaskOp();

public:
	Painting GetResult();
	operator Painting()                              { return GetResult(); }
	
	void Create(double cx, double cy);
	void Create(Sizef sz);
	
	Sizef GetSize() const                            { return size; }
	
	PaintingPainter()                                {}
	PaintingPainter(double cx, double cy)            { Create(cx, cy); }
	PaintingPainter(Sizef sz)                        { Create(sz); }
};
