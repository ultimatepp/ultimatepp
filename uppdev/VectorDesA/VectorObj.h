#ifndef _VectorDes_VectorObj_h_
#define _VectorDes_VectorObj_h_

template <class B, class T> struct Newer { static One<B> New() { return new T; } };

template <class T> struct VType          { enum { VTYPE = ERROR_V }; };
template <>        struct VType<int>     { enum { VTYPE = INT_V }; };
template <>        struct VType<int64>   { enum { VTYPE = INT64_V }; };
template <>        struct VType<double>  { enum { VTYPE = DOUBLE_V }; };
template <>        struct VType<String>  { enum { VTYPE = STRING_V }; };
template <>        struct VType<WString> { enum { VTYPE = WSTRING_V }; };
template <>        struct VType<Date>    { enum { VTYPE = DATE_V }; };
template <>        struct VType<Time>    { enum { VTYPE = TIME_V }; };
//template <>        struct VType<Point>   { enum { VTYPE = POINT_V }; };
//template <>        struct VType<Rect>    { enum { VTYPE = RECT_V }; };
template <>        struct VType<Color>   { enum { VTYPE = COLOR_V }; };
template <>        struct VType<Font>    { enum { VTYPE = FONT_V }; };

void DrawHotRect(Draw& draw, int x, int y, int cx, int cy, bool hot, Color color);

inline int AlignCoord(int pos, int size, Alignment align)
{
	return align == ALIGN_RIGHT ? pos - size : align == ALIGN_CENTER ? pos - (size >> 1) : pos;
}

inline Point AlignPoint(Point pos, Size size, Alignment halign, Alignment valign)
{
	return Point(AlignCoord(pos.x, size.cx, halign), AlignCoord(pos.y, size.cy, valign));
}

String FormatVectorFont(Font f);
Font   ParseVectorFont(CParser& parser);

struct Scaling
{
	Scaling(Size image_size = Size(1, 1), Size pixel_size = Size(1, 1)) : image_size(image_size), pixel_size(pixel_size) {}

	Size image_size;
	Size pixel_size;

	int X(int x) const { return iscale(x, pixel_size.cx, image_size.cx); }
	int Y(int y) const { return iscale(y, pixel_size.cy, image_size.cy); }
};

inline Point operator * (Point pt, const Scaling& sc) { return iscale(pt, sc.pixel_size, sc.image_size); }
inline Size  operator * (Size sz, const Scaling& sc)  { return iscale(sz, sc.pixel_size, sc.image_size); }
extern Rect  operator * (Rect rc, const Scaling& sc);

inline Point operator / (Point pt, const Scaling& sc) { return iscale(pt, sc.image_size, sc.pixel_size); }
inline Size  operator / (Size sz, const Scaling& sc)  { return iscale(sz, sc.image_size, sc.pixel_size); }
extern Rect  operator / (Rect rc, const Scaling& sc);

struct ScalingDraw : public Scaling
{
	ScalingDraw(Draw *draw = NULL, Scaling scaling = Scaling())
		: Scaling(scaling), draw(draw) {}

	Draw *draw;
};

void DrawLineRect(Draw& draw, const Rect& rc, int width = 0, Color color = Black());

class VectorObject;

class VectorArg
{
public:
	enum {
		EDIT_VALUE,
		EDIT_DOC,
		EDIT_LINE_WIDTH,
		EDIT_LINE_STYLE,
		EDIT_ALIGN_HORZ,
		EDIT_ALIGN_VERT,
		EDIT_RECT_STYLE,
		EDIT_LINE_CAP,
	};

	VectorArg(String name, String vm_name, int vtype, Value dflt, int editor = EDIT_VALUE)
		: name(name), vm_name(vm_name), vtype(vtype), dflt(dflt), editor(editor) {}
	virtual ~VectorArg() {}

	virtual Value    Get(const VectorObject *obj) const = 0;
	virtual void     Set(VectorObject *obj, Value v) const = 0;

	static One<Ctrl> CreateEditor(int vtype, int editor);

public:
	String           name;
	String           vm_name;
	int              vtype;
	Value            dflt;
	int              editor;
};

template <class O, class A>
class VectorArgFor : public VectorArg
{
public:
	VectorArgFor(A O::*ref, String name, String vm_name, Value dflt, int editor = EDIT_VALUE)
	: VectorArg(name, vm_name, VType<A>::VTYPE, dflt, editor), ref(ref) {}

	virtual Value Get(const VectorObject *obj) const    { return dynamic_cast<const O *>(obj)->*ref; }
	virtual void  Set(VectorObject *obj, Value v) const { dynamic_cast<O *>(obj)->*ref = v; }

private:
	A             O::*ref;
};

template <class O, class A>
VectorArgFor<O, A> *VectorArgDef(A O::*ref, String name, String vm_name, Value dflt = Null, int editor = VectorArg::EDIT_VALUE)
{ return new VectorArgFor<O, A>(ref, name, vm_name, dflt, editor); }

template <class A>
class VectorGeomArg {
public:
	VectorGeomArg() {}
	virtual ~VectorGeomArg() {}

	virtual A    Get(const VectorObject *obj, int index) const = 0;
	virtual void Set(VectorObject *obj, int index, const A& a) const = 0;

	virtual int  GetCount(const VectorObject *obj) const = 0;
	virtual bool CanSetCount() const { return false; }
	virtual void SetCount(VectorObject *obj, int c) const { NEVER(); }
};

template <class O, class A, int N>
class VectorGeomArgFixed : public VectorGeomArg<A>
{
public:
	VectorGeomArgFixed(A O::**ref_) { for(int i = 0; i < N; i++) ref[i] = ref_[i]; }

	virtual A    Get(const VectorObject *obj, int i) const       { ASSERT(i >= 0 && i < N); return dynamic_cast<const O *>(obj)->*(ref[i]); }
	virtual void Set(VectorObject *obj, int i, const A& a) const { ASSERT(i >= 0 && i < N); dynamic_cast<O *>(obj)->*(ref[i]) = a; }

	virtual int  GetCount(const VectorObject *obj) const         { return N; }

private:
	A            O::*ref[N];
};

template <class O, class A>
inline VectorGeomArgFixed<O, A, 1> *VectorGeomArgDef(A O::*ref)
{ A O::*args[] = { ref }; return new VectorGeomArgFixed<O, A, 1>(args); }

template <class O, class A>
inline VectorGeomArgFixed<O, A, 2> *VectorGeomArgDef(A O::*ra, A O::*rb)
{ A O::*args[] = { ra, rb }; return new VectorGeomArgFixed<O, A, 2>(args); }

template <class O, class A>
inline VectorGeomArgFixed<O, A, 3> *VectorGeomArgDef(A O::*ra, A O::*rb, A O::*rc)
{ A O::*args[] = { ra, rb, rc }; return new VectorGeomArgFixed<O, A, 3>(args); }

template <class O, class A>
class VectorGeomArgFloat : public VectorGeomArg<A>
{
public:
	VectorGeomArgFloat(Vector<A> O::*ref) : ref(ref) {}

	virtual A    Get(const VectorObject *obj, int i) const       { return (dynamic_cast<const O *>(obj)->*ref)[i]; }
	virtual void Set(VectorObject *obj, int i, const A& a) const { (dynamic_cast<O *>(obj)->*ref)[i] = a; }

	virtual int  GetCount(const VectorObject *obj) const         { return (dynamic_cast<const O *>(obj)->*ref).GetCount(); }
	virtual bool CanSetCount() const                             { return true; }
	virtual void SetCount(VectorObject *obj, int c) const        { (dynamic_cast<O *>(obj)->*ref).SetCount(c); }

private:
	Vector<A>    O::*ref;
};

template <class O, class A>
inline VectorGeomArgFloat<O, A> *VectorGeomArgDef(Vector<A> O::*ref)
{ return new VectorGeomArgFloat<O, A>(ref); }

class VectorInfo
{
public:
	VectorInfo(String type, String vm_type, One<VectorObject> (*newfunc)(),
		One< VectorGeomArg<Rect> > rect_args, One< VectorGeomArg<Point> > point_args, Array<VectorArg> args);

	VectorInfo&       Name(int lng, String n)         { name.GetAdd(lng) = n; return *this; }
	VectorInfo&       DefaultName(String n)           { default_name = n; return *this; }
	VectorInfo&       Icon(Image i)                   { icon = i; return *this; }

	One<VectorObject> New() const                     { return (*newfunc)(); }

	int               FindArg(String name) const;
	int               FindVMArg(String vm_name) const { return vm_index.Find(vm_name); }
	String            GetType() const                 { return type; }
	String            GetVMType() const               { return vm_type; }
	String            GetName(int lang) const;
	int               GetCount() const                { return args.GetCount(); }
	const VectorArg&  operator [] (int i) const       { return args[i]; }
	const VectorGeomArg<Rect> *RectArgs() const       { return ~rects; }
	const VectorGeomArg<Point> *PointArgs() const     { return ~points; }

public:
	String            type;
	String            vm_type;
	VectorMap<int, String> name;
	String            default_name;
	One<VectorObject> (*newfunc)();
	Image             icon;
	Index<String>     vm_index;
	Array<VectorArg>  args;
	One< VectorGeomArg<Rect> > rects;
	One< VectorGeomArg<Point> > points;
};

template <class T>
VectorInfo VectorInfoFor(String type, String vm_type,
	One< VectorGeomArg<Rect> > rects, One< VectorGeomArg<Point> > points, Array<VectorArg> args)
{ return VectorInfo(type, vm_type, &Newer<VectorObject, T>::New, rects, points, args); }

class VectorObject
{
public:
	VectorObject() {}
	virtual ~VectorObject() {}

	virtual One<VectorObject>       Copy() const;
	virtual void                    Paint(ScalingDraw& draw) const = 0;
	virtual void                    DesignPaint(ScalingDraw& draw) const;
	virtual void                    DragPaint(ScalingDraw& draw) const;
	virtual void                    SelPaint(ScalingDraw& draw, const Index<int>& sel, bool is_hot) const;
	virtual Value                   GetLink(Point pt, Size sz) const;
	virtual const VectorInfo&       Info() const = 0;

	int                             GetArgCount() const     { return Info().GetCount(); }
	int                             GetArgType(int i) const { return Info()[i].vtype; }
	Value                           GetArg(int i) const     { return Info()[i].Get(this); }
	void                            SetArg(int i, Value v)  { Info()[i].Set(this, v); }
	virtual Rect                    GetBox() const;

	enum Consts {
		TRACK_MASK          = 0x03F,
		RECT_MASK           = 0x040,
		ARG_MASK            = 0x080,
		INDEX_SHIFT         = 8,

		TRACK_RECT_LEFT     = 0x01,
		TRACK_RECT_TOP      = 0x02,
		TRACK_RECT_RIGHT    = 0x04,
		TRACK_RECT_BOTTOM   = 0x08,
		TRACK_RECT_SIDES    = 0x0F,
		TRACK_RECT          = 0x10,
		TRACK_POINT         = 0x11,
		TRACK_MOVE          = 0x12,
		TRACK_IMAGE_SIZE    = 0x13,
		TRACK_NEW           = 0x14,
		TRACK_SELECT        = 0x15,
		TRACK_SIZE          = 0x16,
	};

	virtual Index<int>              Contains(const Scaling& scaling, Rect rc) const;
	virtual int                     Nearest(const Scaling& scaling, Point pt, int& maxdistsquared) const;
	virtual Point                   TrackPoint(int track) const;

	virtual void                    Create(Point from, Point to, dword keyflags);
	virtual void                    Track(int track, Point start, Point end, dword keyflags);
	virtual void                    Align(Alignment halign, Alignment valign, Point anchor);
	virtual void                    EqualSize(Size sz);
	virtual Image                   Cursor(int track, dword keyflags) const;

	String                          ToString() const;
	static One<VectorObject>        Load(CParser& parser);

	virtual String                  FormatCoords() const;

	static VectorMap<String, const VectorInfo *>& RegisteredObjects();
	static VectorMap<String, const VectorInfo *>& RegisteredVMIndex();
	static void                     RegisterInfo(const VectorInfo& vi);
};

class VectorImage : Moveable<VectorImage>
{
public:
	struct Data : public RefBase {
		Data() : size(100, 100) {}

		String              name;
		Size                size;
		Array<VectorObject> objects;
	};

public:
	VectorImage();
	VectorImage(RefPtr<Data> data) : data(data)   {}
	VectorImage(const Value& value);

	operator            Value () const            { return RawToValue(*this); }

	void                Clone();

	void                Clear()                   { data = new Data; }
	bool                IsEmpty() const           { return data->objects.IsEmpty(); }
	int                 GetCount() const          { return data->objects.GetCount(); }
	const VectorObject& operator [] (int i) const { return data->objects[i]; }
	VectorObject&       operator [] (int i)       { return data->objects[i]; }

	void                Add(One<VectorObject> o)  { data->objects.Add(-o); }
	void                Insert(int pos, One<VectorObject> o) { data->objects.Insert(pos, -o); }
	void                Remove(int i)             { data->objects.Remove(i); }
	One<VectorObject>   Detach(int i)             { return data->objects.Detach(i); }
	void                Move(int i, int j)        { data->objects.Move(i, j); }
	void                Reserve(int i)            { data->objects.Reserve(i); }

	void                Paint(ScalingDraw& draw) const;
	void                DesignPaint(ScalingDraw& draw) const;
	void                DragPaint(ScalingDraw& draw) const;

	Size                GetSize() const           { return data->size; }
	void                SetSize(Size sz)          { data->size = sz; }

	String              GetName() const           { return data->name; }
	void                SetName(String n)         { data->name = n; }

	String              SaveVec(int index) const;
	void                LoadVec(CParser& parser);

	void                SaveVM(Stream& data) const;
	void                LoadVM(Stream& data);
	void                SerializeVM(Stream& stream);

	VectorMap< int, Index<int> > Contains(const Scaling& scaling, Rect rc) const;
	Point               Nearest(const Scaling& scaling, Point pt, int maxdistsquared) const;
	Point               Nearest(const Scaling& scaling, Point pt, int maxdistsquared, const Vector<int>& selection) const;
	Point               TrackPoint(Point track) const;
//	Point               FindNearest(Point pt, int maxdistsquared, const VectorMap<int, int>& selection) const;
	Value               GetLink(Point pt, Size sz) const;

	Image               Cursor(Point track, dword keyflags) const;

public:
	RefPtr<Data>        data;
};

struct VectorLineData
{
	VectorLineData&         SetPoint(Vector<Point> pt)   { points = pt; return *this; }
	VectorLineData&         LineColor(Color c)           { line_color = c; return *this; }
	VectorLineData&         LineWidth(int w)             { line_width = w; return *this; }
	VectorLineData&         BeginCap(int c)              { begin_cap = c; return *this; }
	VectorLineData&         BeginSize(int s)             { begin_size = s; return *this; }
	VectorLineData&         BeginColor(Color c)          { begin_color = c; return *this; }
	VectorLineData&         EndCap(int c)                { end_cap = c; return *this; }
	VectorLineData&         EndSize(int s)               { end_size = s; return *this; }
	VectorLineData&         EndColor(Color c)            { end_color = c; return *this; }
	VectorLineData&         Link(String l)               { link = l; return *this; }

	void                    Paint(ScalingDraw& draw) const;
	void                    PaintCap(ScalingDraw& draw, int cap, int size, Color color, Point from, Point to) const;
	Point                   ShrinkCap(ScalingDraw& draw, int cap, int size, Point from, Point to) const;

	Vector<Point>           points;
	Color                   line_color;
	int                     line_width;
	enum { CAP_NONE, CAP_ARROW, CAP_CIRCLE, CAP_END_CIRCLE, CAP_SQUARE, CAP_THIN_ARROW, CAP_COUNT };
	int                     begin_cap;
	int                     begin_size;
	Color                   begin_color;
	int                     end_cap;
	int                     end_size;
	Color                   end_color;
	String                  link;
};

const Display& VectorLineCapDisplay();

class VectorLine : public VectorObject, public VectorLineData
{
public:
	virtual void                    Paint(ScalingDraw& draw) const       { VectorLineData::Paint(draw); }
	virtual void                    DragPaint(ScalingDraw& draw) const;
	virtual const VectorInfo&       Info() const                         { return ClassInfo(); }
	virtual int                     Nearest(const Scaling& scaling, Point pt, int& maxdistsquared) const;
	virtual void                    Create(Point from, Point to, dword keyflags);
	virtual void                    SelPaint(ScalingDraw& draw, const Index<int>& sel, bool is_hot) const;
	virtual Image                   Cursor(int track, dword keyflags) const;
	virtual void                    Track(int track, Point start, Point end, dword keyflags);

	static const VectorInfo&        ClassInfo();
	static void                     Register()                           { RegisterInfo(ClassInfo()); }

	enum {
		TRACK_LINE_INSERT = 0x20,
		TRACK_LINE_REMOVE = 0x21,
	};

private:
	Size                            GetOutVector(const Scaling& scaling, int i, int factor) const;
	void                            GetAddRmPoint(const Scaling& scaling, int i, Point& addpt, Point& rmpt) const;
};

struct VectorRectData
{
	VectorRectData&                 SetRect(Rect r)        { rect = r; return *this; }
	VectorRectData&                 FillColor(Color c)     { fill_color = c; return *this; }
	VectorRectData&                 OutlineColor(Color o)  { outline_color = o; return *this; }
	VectorRectData&                 OutlineWidth(int w)    { outline_width = w; return *this; }
	VectorRectData&                 Rounding(int r)        { rounding = r; return *this; }
	VectorRectData&                 Style(int s)           { style = s; return *this; }
	VectorRectData&                 TextColor(Color c)     { text_color = c; return *this; }
	VectorRectData&                 SetFont(Font f)        { font = f; return *this; }
	VectorRectData&                 Text(WString s)        { text = s; return *this; }
	VectorRectData&                 AlignVert(Alignment v) { valign = v; return *this; }
	VectorRectData&                 AlignHorz(Alignment h) { halign = h; return *this; }
	VectorRectData&                 MarginHorz(int m)      { hmargin = m; return *this; }
	VectorRectData&                 MarginVert(int m)      { vmargin = m; return *this; }
	VectorRectData&                 Link(String l)         { link = l; return *this; }

	void                            Paint(ScalingDraw& draw) const;
	Vector<Point>                   GetPoly(ScalingDraw& draw, int line_width) const;
	Font                            GetScaledFont(const ScalingDraw& draw) const;

	Rect                            rect;
	Color                           text_color;
	Color                           fill_color;
	Color                           outline_color;
	int                             outline_width;
	int                             rounding;
	enum { STYLE_RECT, STYLE_ROUNDED_RECT, STYLE_ELLIPSE };
	int                             style;
	Font                            font;
	WString                         text;
	int                             halign, valign;
	int                             hmargin, vmargin;
	String                          link;
};

class VectorRect : public VectorObject, public VectorRectData
{
public:
	virtual void                    Paint(ScalingDraw& draw) const       { VectorRectData::Paint(draw); }
	virtual void                    DragPaint(ScalingDraw& draw) const;
	virtual const VectorInfo&       Info() const                         { return ClassInfo(); }
//	virtual int                     Nearest(Point pt, int& maxdistsquared) const;

	static const VectorInfo&        ClassInfo();
	static void                     Register()                           { RegisterInfo(ClassInfo()); }
};

struct VectorArcData
{
	VectorArcData&                  SetRect(Rect r)              { rect = r; return *this; }
//	VectorArcData&                  SetColor(Color c)            { color = c; return *this; }
	VectorArcData&                  SetPoint(Point p1, Point p2) { from = p1; to = p2; return *this; }
	VectorArcData&                  LineColor(Color o)           { line_color = o; return *this; }
	VectorArcData&                  LineWidth(int w)             { line_width = w; return *this; }
	VectorArcData&                  Link(String l)               { link = l; return *this; }

	void                            Paint(ScalingDraw& draw) const;

	Rect                            rect;
	Point                           from;
	Point                           to;
//	Color                           color;
	Color                           line_color;
	int                             line_width;
	String                          link;
};

class VectorArc : public VectorObject, public VectorArcData
{
public:
	virtual void                    Paint(ScalingDraw& draw) const       { VectorArcData::Paint(draw); }
	virtual void                    DesignPaint(ScalingDraw& draw) const;
	virtual void                    DragPaint(ScalingDraw& draw) const;
	virtual int                     Nearest(const Scaling& scaling, Point pt, int& maxdistsquared) const;
	virtual const VectorInfo&       Info() const                         { return ClassInfo(); }

	static const VectorInfo&        ClassInfo();
	static void                     Register()                           { RegisterInfo(ClassInfo()); }
};

inline void RegisterStdVectorObjects()
{
	VectorLine::Register();
	VectorRect::Register();
//	VectorRectRounded::Register();
//	VectorEllipse::Register();
	VectorArc::Register();
//	VectorText::Register();
//	VectorTextRect::Register();
//	VectorTextRectJoin::Register();
}

#endif
