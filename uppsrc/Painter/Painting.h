enum {
	PAINTING_CLEAR,
	
	PAINTING_MOVE = 4,
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
	PAINTING_ARC,
	PAINTING_ARC_REL,
	PAINTING_SVGARC,
	PAINTING_SVGARC_REL,
	PAINTING_CLOSE,
	PAINTING_DIV,
	
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
	
	PAINTING_TRANSFORM,
	PAINTING_BEGIN,
	PAINTING_END,
	PAINTING_BEGINMASK,
	PAINTING_BEGINONPATH,
	
	PAINTING_INVERT,

	PAINTING_FILL_GRADIENT_X,
	PAINTING_STROKE_GRADIENT_X,

	PAINTING_FILL_RADIAL_X,
	PAINTING_STROKE_RADIAL_X,
	
	PAINTING_IMAGE_FILTER,
};

class PaintingPainter : public Painter {
	StringStream cmd;
	ValueArray   data;
	Sizef        size;

	void Put(int c)                { cmd.Put(c); }
	void Put32(int c)              { cmd.Put32(c); }
	void Put(const RGBA& c)        { cmd.Put(&c, sizeof(RGBA)); }
	void Putf(const double& d)     { ASSERT(!IsNaN(d)); cmd.Put(&d, sizeof(double)); }
	void Putf(const Pointf& p)     { ASSERT(!IsNaN(p.x) && !IsNaN(p.y)); cmd.Put(&p, sizeof(p)); }
	void Putf(const Xform2D& m)    { cmd.Put(&m, sizeof(m)); }
	void Put(const Font& f)        { cmd.Put(&f, sizeof(Font)); }

protected:
	virtual void   ClearOp(const RGBA& color);

	virtual void   MoveOp(const Pointf& p, bool rel);
	virtual void   LineOp(const Pointf& p, bool rel);
	virtual void   QuadraticOp(const Pointf& p1, const Pointf& p, bool rel);
	virtual void   QuadraticOp(const Pointf& p, bool rel);
	virtual void   CubicOp(const Pointf& p1, const Pointf& p2, const Pointf& p, bool rel);
	virtual void   CubicOp(const Pointf& p2, const Pointf& p, bool rel);
	virtual void   ArcOp(const Pointf& c, const Pointf& r, double angle, double sweep, bool rel);
	virtual void   SvgArcOp(const Pointf& r, double xangle, bool large, bool sweep,
	                        const Pointf& p, bool rel);
	virtual void   CloseOp();
	virtual void   DivOp();

	virtual void   FillOp(const RGBA& color);
	virtual void   FillOp(const Image& image, const Xform2D& transsrc, dword flags);
	virtual void   FillOp(const Pointf& p1, const RGBA& color1,
	                      const Pointf& p2, const RGBA& color2,
	                      int style);
	virtual void   FillOp(const RGBA& color1, const RGBA& color2, const Xform2D& transsrc,
	                      int style);
	virtual void   FillOp(const Pointf& f, const RGBA& color1, 
	                      const Pointf& c, double r, const RGBA& color2,
	                      int style);
	virtual void   FillOp(const Pointf& f, const RGBA& color1, const RGBA& color2,
	                      const Xform2D& transsrc, int style);

	virtual void   StrokeOp(double width, const RGBA& rgba);
	virtual void   StrokeOp(double width, const Image& image, const Xform2D& transsrc,
	                        dword flags);
	virtual void   StrokeOp(double width, const Pointf& p1, const RGBA& color1,
	                        const Pointf& p2, const RGBA& color2,
	                        int style);
	virtual void   StrokeOp(double width, const RGBA& color1, const RGBA& color2,
	                        const Xform2D& transsrc, int style);
	virtual void   StrokeOp(double width, const Pointf& f, const RGBA& color1, 
	                        const Pointf& c, double r, const RGBA& color2,
	                        int style);
	virtual void   StrokeOp(double width, const Pointf& f,
	                        const RGBA& color1, const RGBA& color2,
	                        const Xform2D& transsrc, int style);

	virtual void   ClipOp();

	virtual void   CharacterOp(const Pointf& p, int ch, Font fnt);
	virtual void   TextOp(const Pointf& p, const wchar *text, Font fnt, int n = -1, 
	                      const double *dx = NULL);

	virtual void   ColorStopOp(double pos, const RGBA& color);
	virtual void   ClearStopsOp();
	
	virtual void   OpacityOp(double o);
	virtual void   LineCapOp(int linecap);
	virtual void   LineJoinOp(int linejoin);
	virtual void   MiterLimitOp(double l);
	virtual void   EvenOddOp(bool evenodd);
	virtual void   DashOp(const Vector<double>& dash, double start);
	virtual void   InvertOp(bool invert);
	virtual void   ImageFilterOp(int filter);

	virtual void   TransformOp(const Xform2D& m);

	virtual void   BeginOp();
	virtual void   EndOp();

	virtual void   BeginMaskOp();
	virtual void   BeginOnPathOp(double q, bool abs);

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
