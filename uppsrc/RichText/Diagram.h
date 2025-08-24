struct Point2 : Moveable<Point2> {
	Pointf  pt[2];

	void    Offset(Pointf p)         { pt[0] += p; pt[1] += p; }
	Point2  Offseted(Pointf p) const { Point2 r = *this; r.Offset(p); return r; }
	void    Normalize();
	Rectf   GetRect() const          { return Rectf(pt[0], pt[1]).Normalized(); }
	String  ToString() const         { return String() << pt[0] << " - " << pt[1]; }
	
	void    Serialize(Stream& s)     { s % pt[0] % pt[1]; }
};

struct Diagram;

struct DiagramItem : Point2 {
	int    shape;
	String qtf;
	double width;
	Color  ink;
	Color  paper;
	String blob_id;
	bool   flip_horz;
	bool   flip_vert;
	bool   aspect_ratio;
	double rotate;
	
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
		SHAPE_ARC,

		SHAPE_SVGPATH, // must be last - 1
		SHAPE_IMAGE, // must be last
		SHAPE_COUNT,
	};
	
	enum LineEnding {
		CAP_NONE,
		CAP_ARROW,
		CAP_CIRCLE,
		CAP_DISC,
		CAP_DIM,
		CAP_T,
		
		CAP_COUNT
	};
	
	enum Dash {
		DASH_COUNT = 10
	};
	
	enum {
		EDITOR = 0x8000,
		GRID   = 0x4000,
		DARK   = 0x2000,
		FAST   = 0x1000,
	};
	
	int cap[2] = { CAP_NONE, CAP_NONE };
	int dash = 0;

	void  Paint(Painter& w, const Diagram& diagram, dword style = 0, const Index<Pointf> *conn = nullptr) const;
	Sizef GetStdSize(const Diagram& diagram) const;

	static const Vector<double>& GetDash(int i);
	
	bool IsLine() const               { return shape == SHAPE_LINE; }
	
	Vector<Pointf> GetConnections() const;

	bool IsClick(Point p, const Diagram& diagram, bool relaxed = false) const;
	bool IsTextClick(Point p) const;
	Rect GetTextEditRect() const;

	void FixPosition();
	
	Xform2D Rotation(int d = 1) const { return Xform2D::Rotation(d * M_PI * rotate / 180); }

	void Serialize(Stream& s)         { Point2::Serialize(s); s % shape % ink % paper % qtf % width % cap[0] % cap[1] % dash % blob_id % flip_horz % flip_vert % aspect_ratio; }

	void Reset();
	void Save(StringBuffer& r) const;
	void Load(CParser& p, const Diagram& diagram);
	
	DiagramItem() { Reset(); }

private:
	static Index<String> LineCap;
	static Index<String> Shape;
};

struct Diagram {
	Size                      size = Null; // Null - auto
	Array<DiagramItem>        item;
	Image                     img;
	bool                      img_hd = false;
	VectorMap<String, String> blob;
	
	struct PaintInfo {
		bool       editor = false;
		bool       display_grid = false;
		int        cursor = -1;
		bool       dark = false;
		bool       fast = false;
		Index<int> sel;
	};

	Size   GetEditSize() const;
	Size   GetSize() const;
	void   Paint(Painter& w, const PaintInfo& pi) const;
	String AddBlob(const String& data);
	String GetBlob(const String& id) const;
	Image  GetBlobImage(const String& id) const;
	Rectf  GetBlobSvgPathBoundingBox(const String& id) const;
	void   Serialize(Stream& s);
	void   Save(StringBuffer& r) const;
	void   Load(CParser& p);

	static Zoom TextZoom() { return Zoom(96, 600); }
	
private:
	void GetSize0(Pointf& tl, Pointf& br) const;
};
