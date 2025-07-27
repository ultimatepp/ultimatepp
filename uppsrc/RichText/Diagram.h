struct Point2 : Moveable<Point2> {
	Pointf  pt[2];

	void    Offset(Pointf p)         { pt[0] += p; pt[1] += p; }
	Point2  Offseted(Pointf p) const { Point2 r = *this; r.Offset(p); return r; }
	void    Normalize();
	Rectf   GetRect() const          { return Rectf(pt[0], pt[1]).Normalized(); }
	String  ToString() const         { return String() << pt[0] << " - " << pt[1]; }
	
	void    Serialize(Stream& s)     { s % pt[0] % pt[1]; }
};

struct DiagramItem : Point2 {
	int    shape;
	String qtf;
	double width;
	Color  ink;
	Color  paper;
	Sizef  size;
	String data;
	
	enum {
		SHAPE_LINE,
		SHAPE_RECT,
		SHAPE_ROUNDRECT,
		SHAPE_ELLIPSE,
		SHAPE_DIAMOND,
		SHAPE_OVAL,
		SHAPE_PARALLELOGRAM,
		SHAPE_CYLINDER,
		SHAPE_TRIANGLE,
		SHAPE_ITRIANGLE,
		SHAPE_ARROWLEFT,
		SHAPE_ARROWRIGHT,
		SHAPE_ARROWHORZ,
		SHAPE_ARROWDOWN,
		SHAPE_ARROWUP,
		SHAPE_ARROWVERT,
		SHAPE_SVGPATH,
		SHAPE_COUNT,
	};
	
	enum LineEnding {
		CAP_NONE,
		CAP_ARROW,
		CAP_CIRCLE,
		
		CAP_COUNT
	};
	
	enum Dash {
		DASH_COUNT = 5
	};
	
	enum {
		EDITOR = 0x8000,
		GRID   = 0x4000,
		DARK   = 0x2000,
	};
	
	int cap[2] = { CAP_NONE, CAP_NONE };
	int dash = 0;

	void Paint(Painter& w, dword style = 0, const Index<Pointf> *conn = nullptr) const;
	
	bool IsLine() const              { return shape == SHAPE_LINE; }
	
	Vector<Pointf> GetConnections() const;

	bool IsClick(Point p) const;
	bool IsTextClick(Point p) const;
	Rect GetTextEditRect() const;

	void FixPosition();

	void Serialize(Stream& s)        { Point2::Serialize(s); s % shape % ink % paper % qtf % width % cap[0] % cap[1] % dash % size % data; }

	void Reset();
	void Save(StringBuffer& r) const;
	void Load(CParser& p);
	
	DiagramItem() { Reset(); }

private:
	static Index<String> LineCap;
	static Index<String> Shape;
};

struct Diagram {
	Size               size = Null; // Null - auto
	Array<DiagramItem> item;
	Image img;
	bool  img_hd = false;
	
	struct PaintInfo {
		bool       editor = false;
		bool       display_grid = false;
		int        cursor = -1;
		bool       dark = false;
		Index<int> sel;
	};
	
	Size  GetSize() const;
	void  Paint(Painter& w, const PaintInfo& pi) const;
	void  Serialize(Stream& s);
	void  Save(StringBuffer& r) const;
	void  Load(CParser& p);

	static Zoom TextZoom() { return Zoom(96, 600); }
};
