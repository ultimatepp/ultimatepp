struct BoundsPainter : public NilPainter {
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
	
	virtual void   ClipOp();

	virtual void   TextOp(const Pointf& p, const wchar *text, Font fnt, int n = -1,
	                      const double *dx = NULL);
	virtual void   CharacterOp(const Pointf& p, int ch, Font fnt);

	virtual void   TransformOp(const Xform2D& m);
	virtual void   BeginOp();
	virtual void   EndOp();

	
	void Finish(double width = 0);

	Painter& sw;
	Rectf    boundingbox;
	Pointf   current, qcontrol, ccontrol;

	Array<Xform2D> mtx;
	Rectf      svg_boundingbox;
	NilPainter nil;

	void  Bounds(Pointf p);

	void  Quadratic(const Pointf& p1, const Pointf& p);
	void  Cubic(const Pointf& p1, const Pointf& p2, const Pointf& p);

	void  New();
	const Rectf& Get() { return boundingbox; }
	
	bool  compute_svg_boundingbox;

	BoundsPainter(Painter& sw) : sw(sw) { New(); mtx.Add(); svg_boundingbox = Null; compute_svg_boundingbox = false; }
};

struct SvgParser {
	Painter& sw;

	void ParseSVG(const char *svg, const char *folder);

	struct Stop : Moveable<Stop> {
		RGBA   color;
		double offset;
	};
	
	struct Gradient {
		bool   resolved;
		bool   radial;
		Pointf a, b, c, f;
		double r;
		int    style;
		bool   user_space; // TODO: Cascade!
		String transform;

		Vector<Stop> stop;
		
		String href;
	};
	
	ArrayMap<String, Gradient> gradient;
	
	struct State {
		double opacity;
	
		int    fill_gradient;
		Color  fill;
		double fill_opacity;
		
		int    stroke_gradient;
		Color  stroke;
		double stroke_width;
		double stroke_opacity;
		
		String dash_array;
		double dash_offset;
		
		int    text_anchor; // 0 left, 1 middle, 2 right
		
		Font   font;
		
		const XmlNode *n;
	};
	
	const XmlNode *current = NULL;
	Array<State>   state;
	bool           closed;
	Pointf         prev;
	Xform2D        lastTransform;
	BoundsPainter  bp;
	VectorMap<String, const XmlNode*> idmap;
	VectorMap<String, String> classes;
	Color          currentColor = SBlack();

	void Reset();

	Color GetTextColor(const String& color);
	Color GetColor(const String& text);
	
	void    ProcessValue(const String& key, const String& value);
	void    Style(const char *style);
	Xform2D Transform(const char *transform);
	
	String Txt(const char *id)                  { return current ? current->Attr(id) : String(); }
	double Dbl(const char *id, double def = 0)  { return Nvl(StrDbl(Txt(id)), def); }
	
	void StartElement(const XmlNode& n);
	void EndElement();
	void StrokeFinishElement();
	void FinishElement();
	void DoGradient(int gi, bool stroke);
	void Poly(const XmlNode& n, bool line);
	void Items(const XmlNode& n, int depth);
	void Element(const XmlNode& n, int depth, bool dosymbols = false);
	void ParseGradient(const XmlNode& n, bool radial);
	void ResolveGradient(int i);
	void MapIds(const XmlNode& n);

	bool   ReadBool(CParser& p);
	double ReadDouble(CParser& p);
	Pointf ReadPoint0(CParser& p, bool rel);
	Pointf ReadPoint(CParser& p, bool rel);
	void   Path(const char *s);
	
	bool Parse(const char *xml);

	Event<String, String&> resloader;
	
	SvgParser(Painter& sw);
};
