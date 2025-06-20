struct Point2 : Moveable<Point2> {
	Pointf p1;
	Pointf p2;

	void    Offset(Pointf p)         { p1 += p; p2 += p; }
	Point2  Offseted(Pointf p) const { Point2 r = *this; r.Offset(p); return r; }
	void    Normalize();
	Rectf   GetRect() const          { return Rect(p1, p2).Normalized(); }
	String  ToString() const         { return String() << p1 << " - " << p2; }
	
	void    Serialize(Stream& s)     { s % p1 % p2; }
};

struct DiagramItem : Point2 {
	int    shape;
	String qtf;
	double width;
	Color  ink;
	Color  paper;
	
	enum {
		SHAPE_LINE,
		SHAPE_RECT,
		SHAPE_ROUNDRECT,
		SHAPE_ELLIPSE,
		SHAPE_DIAMOND,
		SHAPE_OVAL,
		SHAPE_PARALLELOGRAM,
		SHAPE_COUNT,
	};
	
	enum LineEnding {
		CAP_NONE,
		CAP_ARROW,
		CAP_CIRCLE,
		
		CAP_COUNT
	};
	
	enum Dash {
		DASH_COUNT = 4
	};
	
	enum {
		EDITOR = 0x8000
	};
	
	int line_start = CAP_NONE;
	int line_end = CAP_NONE;
	int dash = 0;

	void Paint(Painter& w, dword style = 0) const;
	
	bool IsLine() const              { return shape == SHAPE_LINE; }

	bool IsClick(Point p) const;
	bool IsTextClick(Point p) const;
	Rect GetTextEditRect() const;

	void Serialize(Stream& s)        { Point2::Serialize(s); s % shape % ink % paper % qtf % width % line_start % line_end % dash; }

	void Reset();
	void Save(StringBuffer& r) const;
	void Load(CParser& p);
	
	DiagramItem() { Reset(); }

private:
	static Index<String> LineCap;
	static Index<String> Shape;
};

struct Diagram {
	Array<DiagramItem> item;
	Image img;
	bool  img_hd = false;
	
	struct PaintInfo {
		bool       editor = false;
		int        cursor = -1;
		Index<int> sel;
	};
	
	Size  GetSize() const;
	void  Paint(Painter& w, const PaintInfo& pi) const;
	void  Serialize(Stream& s);
	void  Save(StringBuffer& r) const;
	void  Load(CParser& p);

	static Zoom TextZoom() { return Zoom(96, 600); }
};
