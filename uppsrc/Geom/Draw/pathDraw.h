namespace Upp {

class PlotterTool;

#define CATCH_PATH_MAP 0 // 1 = turn path catching on

#if CATCH_PATH_MAP
	#define PATH_MAP_NULL NULL
#else
	#define PATH_MAP_NULL &PathStyleMap::App()
#endif

const double STD_CHAMFER = 1.5;

class PathStyleMisc
{
public:
	PathStyleMisc() : width(1), begin(0), segment(1), end(0),
		miter(MITER_SHARP), chamfer(STD_CHAMFER) {}

public:
	double begin, segment, end;
	double width;
	double chamfer; // relative chamfer size
	enum { MITER_ROUND, MITER_SHARP, MITER_FLAT };
	int    miter;
};

class PathStyle : public PathStyleMisc, MoveableAndDeepCopyOption<PathStyle>
{
public:
	PathStyle() {}
	PathStyle(pick_ PathStyle& style);
	PathStyle(const PathStyle& ps, int) : PathStyleMisc(ps), traces(ps.traces, 0) {}

	PathStyle&    operator = (pick_ PathStyle& style);

	String        Encode() const;
	const char   *Decode(const char *s); // returns NULL on error, otherwise end of part

	bool          IsEmpty() const             { return traces.IsEmpty(); }
	bool          IsSolid(double& wd, Color& co) const;

	void          Serialize(Stream& stream);

	static const PathStyle& empty();
	static const PathStyle& solid();
	static const PathStyle& dash();
	static const PathStyle& dot();
	static const PathStyle& dash_dot();
	static const PathStyle& dash_dot_dot();

public:
	enum // track styles
	{
		LEFT         = 0x00000001,
		LEFT_TOP     = 0x00000002,
		LEFT_BOTTOM  = 0x00000004,
		RIGHT        = 0x00000008,
		RIGHT_TOP    = 0x00000010,
		RIGHT_BOTTOM = 0x00000020,

		PREC         = 5,
	};

	class Trace {
	public:
		Trace() {}
		Trace(double left, double left_top, double left_bottom,
			double right, double right_top, double right_bottom,
			Color color = Null)
			: left(left), left_top(left_top), left_bottom(left_bottom)
			, right(right), right_top(right_top), right_bottom(right_bottom)
			, color(color) {}

		void          Serialize(Stream& stream);

		String        Encode() const;
		const char   *Decode(const char *p);

		Rectf         GetExtent() const                    { return Rectf(left, Top(), right, Bottom()); }
		void          Inflate(Pointf amount);
		bool          Contains(Pointf pt) const;
		bool          ContainsHorz(double x) const         { return left <= x && right >= x; }
		bool          ContainsHorz(double l, double r) const { return left <= l && right >= r; }
		double        GetDistance(Pointf pt) const;
		int           GetTrackStyle(Pointf pt, Pointf tolerance, Pointf& start) const;
		static Image  GetTrackCursor(int trackinfo);
		Trace&        Track(Pointf delta, int style); // -1 = move
		Trace&        Move(Pointf delta)                      { return Track(delta, -1); }
		Trace&        Move(double dx, double dy)           { return Track(Pointf(dx, dy), -1); }
		Trace&        Bind(const Rectf& rc);
		Trace&        TraceColor(Color c)                  { color = c; return *this; }
		Trace&        Crop(double l, double r);
		Trace&        CropRel(double l, double r)          { return Crop(l, r).Move(-l, 0); }
		Trace         GetTrack(Pointf delta, int style) const { Trace t(*this); t.Track(delta, style); return t; }
		Trace         GetMove(Pointf delta) const             { return GetTrack(delta, -1); }
		Trace         GetMove(double dx, double dy) const  { Trace t(*this); t.Track(Pointf(dx, dy), -1); return t; }
		Trace         GetTraceColor(Color c) const         { Trace t(*this); t.TraceColor(c); return t; }
		Trace         GetCrop(double l, double r) const    { Trace t(*this); t.Crop(l, r); return t; }
		Trace         GetCropRel(double l, double r) const { Trace t(*this); t.CropRel(l, r); return t; }

		void          Paint(PlotterTool& tool, bool reduce = false, UPP::Color outline = Null) const;

		double        Width() const                        { return right - left; }
		double        Top() const                          { return min(left_top, right_top); }
		double        Bottom() const                       { return max(left_bottom, right_bottom); }
		double        LeftHeight() const                   { return left_bottom - left_top; }
		double        RightHeight() const                  { return right_bottom - right_top; }
		double        TopHeight() const                    { return right_top - left_top; }
		double        BottomHeight() const                 { return right_bottom - left_bottom; }

		bool          IsHorzEmpty() const                  { return right <= left; }
		bool          IsVertEmpty() const                  { return left_bottom <= left_top && right_bottom <= right_top; }
		bool          IsBothEmpty() const                  { return IsHorzEmpty() && IsVertEmpty(); }
		bool          IsAreaEmpty() const                  { return IsHorzEmpty() || IsVertEmpty(); }

		Pointf        LeftTop() const                      { return Pointf(left, left_top); }
		Pointf        LeftCenter() const                   { return Pointf(left, (left_top + left_bottom) / 2); }
		Pointf        LeftBottom() const                   { return Pointf(left, left_bottom); }
		Pointf        CenterTop() const                    { return Pointf((left + right) / 2, (left_top + right_top) / 2); }
		Pointf        CenterBottom() const                 { return Pointf((left + right) / 2, (left_bottom + right_bottom) / 2); }
		Pointf        RightTop() const                     { return Pointf(right, right_top); }
		Pointf        RightCenter() const                  { return Pointf(right, (right_top + right_bottom) / 2); }
		Pointf        RightBottom() const                  { return Pointf(right, right_bottom); }

		bool          IsEqual(const Trace& t) const;

	public:
		double        left, left_top, left_bottom, right, right_top, right_bottom;
		Color         color; // Null = default line color
	};

	Array<Trace>      traces;

	class Clip
	{
	public:
		Clip() {}

		void          Serialize(Stream& stream);

		Rectf         GetExtent() const;

		void          Write()   { WriteClipboardFormat(*this); }
		bool          Read()    { return ReadClipboardFormat(*this); }
		static bool   IsAvail() { return IsClipboardFormatAvailable<Clip>(); }

		Array<Trace>  traces;
	};

private:
	void          Encode(String& out, char tag, double size, const Array<Trace>& list) const;
	const char   *Decode(const char *s, char tag, double& size, Array<Trace>& list);
};

inline bool operator == (const PathStyle::Trace& a, const PathStyle::Trace& b) { return  a.IsEqual(b); }
inline bool operator != (const PathStyle::Trace& a, const PathStyle::Trace& b) { return !a.IsEqual(b); }

class PathStyleMap : Moveable<PathStyleMap>, DeepCopyOption<PathStyleMap>
{
public:
	PathStyleMap() : dirty(false) {}
	PathStyleMap(const PathStyleMap& m, int)
		: name(m.name), map(m.map, 0), dirty(m.dirty) {}

	void             Serialize(Stream& stream);

	void             SetName(String n)                                { name = n; } // todo: dirty ?
	String           GetName() const                                  { return name; }

	void             Set(String sn, const PathStyle& style)           { map.GetAdd(sn) <<= style; sort.Clear(); }
	void             Remove(String sn)                                { int i = map.Find(sn); if(i >= 0) { map.Remove(i); sort.Clear(); } }
	void             Rename(int i, String new_name)                   { map.SetKey(i, new_name); sort.Clear(); }

	void             Touch(bool t = true)                             { dirty = t; }
	bool             IsDirty() const                                  { return dirty; }

	PathStyle        Scan(String name, const PathStyle& dflt = PathStyle::solid()) const;
	// accepts NULL this

	const PathStyle& Get(String name, const PathStyle& dflt = PathStyle::solid()) const { return map.Get(name, dflt); }
	const PathStyle& operator [] (String name) const                  { return Get(name); }

	bool             IsEmpty() const                                  { return map.IsEmpty(); }
	int              GetCount() const                                 { return map.GetCount(); }
	const Vector<int>& GetSort() const;
	int              GetSort(int si) const                            { return GetSort()[si]; }
	String           GetSortName(int si) const                        { return map.GetKey(GetSort(si)); }
	const PathStyle& GetSortStyle(int si) const                       { return map[GetSort(si)]; }

	int              FindSortName(String name, int exclude = -1) const;
	String           GetUniqueName(String prefix) const;

	String           Export() const;
	void             Import(String s, bool update_existing = true); // throw Exc

	static PathStyleMap& App();

public:
	String           name;
	ArrayMap<String, PathStyle> map;
	mutable Vector<int> sort;
	bool             dirty;
};

//#define DEBUG_DRAW // comment to turn draw debugging off

class PathDraw
{
public:
	PathDraw();
	PathDraw(Draw& draw, const PathStyle& style, Color color = Black, double width = -10, double dash = Null, bool closed = false);

	void                Clear();
	void                Set(Draw& draw, const PathStyle& style, Color color = Black, double width = -10, double dash = Null, bool closed = false);

	bool                IsEmpty() const                      { return empty; }

	bool                SetExtent(const Rect& rc);
	void                ClearExtent()                        { SetExtent(Null); }

	void                MoveTo(Point pt)                     { (this ->* moveto)(pt); }
	void                MoveTo(int x, int y)                 { (this ->* moveto)(Point(x, y)); }
	void                LineTo(Point pt)                     { (this ->* lineto)(pt); }
	void                LineTo(int x, int y)                 { (this ->* lineto)(Point(x, y)); }

	void                Line(Point p, Point q)               { MoveTo(p); LineTo(q); }
	void                Line(int x1, int y1, int x2, int y2) { MoveTo(Point(x1, y1)); LineTo(Point(x2, y2)); }

	void                Rectangle(const Rect& rc);
	void                Rectangle(int x, int y, int cx, int cy) { Rectangle(RectC(x, y, cx, cy)); }

	void                ArcTo(Point pt, int bulge);
	void                ArcTo(int x, int y, int bulge)       { ArcTo(Point(x, y), bulge); }
	void                Arc(Point p, Point q, int bulge)     { MoveTo(p); ArcTo(q, bulge); }
	void                Arc(int x1, int y1, int x2, int y2, int bulge) { MoveTo(Point(x1, y1)); ArcTo(Point(x2, y2), bulge); }

	void                Circle(Point pt, int radius);

	void                Paint();

public:
	enum { LINE_BATCH = 6000, POLY_BATCH = 6000 };

	class Output
	{
	public:
		Output(Color color, int width, Draw& draw) : color(color), width(width), draw(draw) {}

		bool          MustFlush() const  { return vertices.GetCount() >= LINE_BATCH; }
		void          CheckFlush()       { if(MustFlush()) Flush(); }
		void          Flush();
		void          AddFirst(Point pt);
		void          AddNext(Point pt)  { if(vertices.Top() != pt) { vertices.Add(pt); counts.Top()++; } }
		void          AddNext(const Vector<Point>& list);
		void          AddThick(void (*ln)(Output& out, Point a, Point b), Point start, const Vector<Point>& list);
		Point        *AddSeg(int segments, int counts);
		Point        *AddSeg(int count);

	public:
		Color         color;
		int           width;
		Draw&         draw;

	private:
		Vector<Point> vertices;
		Vector<int>   counts;
	};

private:
	struct Split;
	struct Segment;
	enum PART { PART_BEGIN, PART_OPEN, PART_SEGMENT, PART_SKIPEND, PART_END };

	void                MoveToSimple(Point pt);
	void                LineToSimple(Point pt);
	void                MoveToThick(Point pt);
	void                LineToThick(Point pt);
	void                MoveToFull(Point pt);
	void                LineToFull(Point pt);

	void                CalcLineBegin();
	void                AddSegment(const PathStyle::Trace& trace, bool infinite);
	void                Restart();
	void                FlushLines();
	void                LineToRaw();
	Split               SplitLine();
	void                SetSegment(const Segment& s, PART part);
	void                AddPos()                             { pos[3] = pos[2]; pos[2] = pos[1]; pos[1] = pos[0]; }
	void                AddPos(Point p)                      { AddPos(); pos[0] = p; }
	void                FlushSplit(double keep_length);
	void                AddSplit()                           { CheckSplit(); Split s = SplitLine(); split_buffer.AddTail(s); part_total += s.total; CheckSplit(); }
	void                CheckSplit();
	void                RecurseArc(Point next, int length, int bulge, int levels);

	bool                DoBegin();

private:
	const PathStyle    *style;
	Draw               *draw;
	Color               std_color;
	struct Trace
	{
		int    start, left_top, left_bottom, right_top, right_bottom, width;
		int    index; // output stream index
		Color  color;
		double pos, top_step, bottom_step;
		Point  last_top, last_bottom;
		void (*hline)(Output& out, Point a, Point b);
		bool   vline, left_ground, right_ground;
	};
	struct Linear
	{
		Linear(int top = 0, int bottom = 0, Color color = Null, int index = 0)
			: top(top), bottom(bottom), color(color), index(index), hline(0) {}
		int    top, bottom;
		void (*hline)(Output& out, Point a, Point b);
		Point  last_top, last_bottom;
		Color  color;
		int    index;
		bool   ground;
	};
	struct Segment
	{
		Segment() {}
		Segment(int top, int bottom, int start, int end, double length)
			: top(top), bottom(bottom), start(start), end(end), length(length) {}

		bool   IsEmpty() const { return start == end; }

		int    top;    // top y
		int    bottom; // bottom y
		int    start;  // start index in traces array
		int    end;    // end index in traces array
		double length;
	};
	struct Split
	{
		Split() {}
		Split(double begin, double end, double total, Point pos)
			: begin(begin), end(end), total(total), pos(pos) {}

		double GetStartPos(double excess) const { return 1 - (excess - end) / (total - begin - end); }
		double GetEndPos(double excess) const   { return GetStartPos(excess >= total - begin ? total : excess); }
		double begin, end, total;
		Point  pos; // segment end vertex
	};

	void    (PathDraw::*moveto)(Point pt);
	void    (PathDraw::*lineto)(Point pt);
	void              (*one_thick)(Output& out, Point a, Point b);
	Output             *one_output;
	BiArray<Split>      split_buffer;
//	double              avail_length;
	double              width; // pixel width
	double              dash; // pixel dash length
	double              chamfer_ratio; // a^b < -chamfer_ratio * fabs(a%b) -> chamfer
	double              chamfer_dist;  // B' = B + y*a0.Left() + chamfer_dist*a0
	int                 clip_radius;   // maximum line radius (distance from original line)
	Rect                clip_rect;
	PART                part, init_part;
	bool                closed;
	bool                empty, init_empty;
	bool                calc_miter;
	Array<Trace>        traces;
	Segment             seg_begin, seg_segment, seg_end, seg, init_seg;
	int                 next_index, clip_index;
	int                 miter;
	Array<Linear>       segment_lines;
	Vector<int>         open_traces;
	double              part_total;
	double              skip_size; // pixels left until beginning of next trace in pixels
	Point              *pos, raw_pos[4], split_pos[4]; // previous positions
	Point               close_line[3];
	Pointf              old_up;
	double              old_ulen;
	bool                old_axb_left;
	ArrayMap<Color, Output> outputs;
};

class PathStyleDisplay : public Display
{
public:
	PathStyleDisplay(const PathStyleMap *path_map = PATH_MAP_NULL);
	virtual ~PathStyleDisplay();

	PathStyleDisplay&   PathMap(const PathStyleMap *pm) { path_map = pm; return *this; }
	const PathStyleMap *GetPathMap() const              { return path_map; }

	virtual void        Paint(Draw& draw, const Rect& rc, const Value& v, Color i, Color p, dword s) const;

protected:
	const PathStyleMap *path_map;
};

}
