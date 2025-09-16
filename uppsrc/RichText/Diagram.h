struct Diagram;

struct DiagramItem {
	Pointf pos;
	Sizef  size;
	
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
		SHAPE_ARROWRIGHT,
		SHAPE_ARROWHORZ,
		SHAPE_ARROWDOWN,
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

		CAP_ARROWL,
		CAP_CIRCLEL,
		CAP_DISCL,
		CAP_DIML,
		CAP_TL,
		
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

	void    Offset(Pointf p)         { pos += p; }
	void    Normalize();
	Rectf   GetRect() const          { return IsLine() ? Rectf(pos, size) : Rectf(pos - size, pos + size); }
	String  ToString() const         { return String() << pos << " " << size; }

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

	void Serialize(Stream& s);

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
	void   SweepBlobs(const Index<String>& keep_ids);
	Image  GetBlobImage(const String& id) const;
	Rectf  GetBlobSvgPathBoundingBox(const String& id) const;
	void   Serialize(Stream& s);
	void   Save(StringBuffer& r) const;
	void   Load(CParser& p);

	static Zoom TextZoom() { return Zoom(96, 600); }
	
private:
	void GetSize0(Pointf& tl, Pointf& br) const;
};
