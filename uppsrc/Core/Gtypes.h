template <class T> struct Point_;
template <class T> struct Size_;
template <class T> struct Rect_;

template <class T>
struct Size_ : Moveable< Size_<T> > {
	T             cx, cy;

	void          Clear()                      { cx = cy = 0; }
	bool          IsEmpty() const              { return cx == 0 || cy == 0; }

	void          SetNull()                    { cx = cy = Null; }
	bool          IsNullInstance() const       { return UPP::IsNull(cx); }

	Size_&        operator+=(Size_ p)          { cx  += p.cx; cy  += p.cy; return *this; }
	Size_&        operator+=(T t)              { cx  += t;    cy  += t;    return *this; }
	Size_&        operator-=(Size_ p)          { cx  -= p.cx; cy  -= p.cy; return *this; }
	Size_&        operator-=(T t)              { cx  -= t;    cy  -= t;    return *this; }
	Size_&        operator*=(Size_ p)          { cx  *= p.cx; cy  *= p.cy; return *this; }
	Size_&        operator*=(T t)              { cx  *= t;    cy  *= t;    return *this; }
	Size_&        operator/=(Size_ p)          { cx  /= p.cx; cy  /= p.cy; return *this; }
	Size_&        operator/=(T t)              { cx  /= t;    cy  /= t;    return *this; }
	Size_&        operator<<=(int sh)          { cx <<= sh;   cy <<= sh;   return *this; }
	Size_&        operator>>=(int sh)          { cx >>= sh;   cy >>= sh;   return *this; }

	Size_&        operator++()                 { ++cx; ++cy; return *this; }
	Size_&        operator--()                 { --cx; --cy; return *this; }

	friend Size_  operator+(Size_ s)           { return s; }
	friend Size_  operator-(Size_ s)           { return Size_(-s.cx, -s.cy); }

	friend Size_  operator+(Size_ a, Size_ b)  { return a += b; }
	friend Size_  operator+(Size_ a, T t)      { return a += t; }
	friend Size_  operator+(T t, Size_ b)      { return b += t; }
	friend Size_  operator-(Size_ a, Size_ b)  { return a -= b; }
	friend Size_  operator-(Size_ a, T t)      { return a -= t; }
	friend Size_  operator-(T t, Size_ b)      { b = -b; return b += t; }
	friend Size_  operator*(Size_ a, Size_ b)  { return a *= b; }
	friend Size_  operator*(Size_ a, T b)      { return a *= b; }
	friend Size_  operator*(T a, Size_ b)      { return b *= a; }
	friend Size_  operator/(Size_ a, Size_ b)  { return a /= b; }
	friend Size_  operator/(Size_ a, T b)      { return a /= b; }
	friend Size_  operator<<(Size_ a, int sh)  { return a <<= sh; }
	friend Size_  operator>>(Size_ a, int sh)  { return a >>= sh; }

	friend bool   operator==(Size_ a, Size_ b) { return a.cx == b.cx && a.cy == b.cy; }
	friend bool   operator!=(Size_ a, Size_ b) { return !(a == b); }

	friend Size_  min(Size_ a, Size_ b)        { return Size_(min(a.cx, b.cx), min(a.cy, b.cy)); }
	friend Size_  max(Size_ a, Size_ b)        { return Size_(max(a.cx, b.cx), max(a.cy, b.cy)); }

	friend Size_  Nvl(Size_ a, Size_ b)        { return IsNull(a) ? b : a; }

	friend T      ScalarProduct(Size_ a, Size_ b) { return a.cx * b.cx + a.cy * b.cy; }
	friend T      VectorProduct(Size_ a, Size_ b) { return a.cx * b.cy - a.cy * b.cx; }
	friend T      Squared(Size_ a)             { return a.cx * a.cx + a.cy * a.cy; }
	friend double Length(Size_ a)              { return hypot(a.cx, a.cy); }

	unsigned      GetHashValue() const         { return CombineHash(cx, cy); }

	String        ToString() const;

	Size_() {}
	Size_(T cx, T cy) : cx(cx), cy(cy) {}

	Size_(const Size_<int>& sz)  : cx((T)sz.cx), cy((T)sz.cy) {}
	Size_(const Size_<short>& sz)  : cx((T)sz.cx), cy((T)sz.cy) {}
	Size_(const Size_<double>& sz)  : cx((T)sz.cx), cy((T)sz.cy) {}
	Size_(const Size_<int64>& sz) : cx((T)sz.cx), cy((T)sz.cy) {}

	Size_(const Point_<T>& pt) : cx(pt.x), cy(pt.y) {}

	Size_(const Nuller&)                       { cx = cy = Null; }

	operator Value() const                     { return FitsSvoValue<Size_>() ? SvoToValue(*this) : RichToValue(*this); }
	Size_(const Value& src)                    { *this = src.Get<Size_>(); }

	operator Ref()                             { return AsRef(*this); }

	void Serialize(Stream& s)                  { s % cx % cy; }
	void Jsonize(JsonIO& jio)                  { jio("cx", cx)("cy", cy); }
	void Xmlize(XmlIO& xio)                    { xio.Attr("cx", cx).Attr("cy", cy); }
	
	int  Compare(const Size_&) const           { NEVER(); return 0; }
	int  PolyCompare(const Value&) const       { NEVER(); return 0; }

#ifdef PLATFORM_WIN32
	operator SIZE*()                           { ASSERT(sizeof(*this) == sizeof(SIZE)); return (SIZE*)this; }
	operator const SIZE*() const               { ASSERT(sizeof(*this) == sizeof(SIZE)); return (SIZE*)this; }
	operator SIZE() const                      { SIZE sz; sz.cx = cx; sz.cy = cy; return sz; }
	LONG     GetLONG() const                   { return MAKELONG(cx, cy); }

	Size_(POINT pt) : cx((T)pt.x), cy((T)pt.y)  {}
	Size_(SIZE sz) : cx((T)sz.cx), cy((T)sz.cy) {}
	explicit Size_(LONG lParam) { cx = (T)(int16)LOWORD(lParam); cy = (T)(int16)HIWORD(lParam); }
#endif
};

template <class T>
String Size_<T>::ToString() const {
	String ss;
	return ss << '(' << cx << ", " << cy << ')';
}

template <class T>
struct Point_ : Moveable< Point_<T> > {
	T             x, y;

	typedef       Size_<T> Sz;

	void          Clear()                           { x = y = 0; }
	bool          IsZero() const                    { return x == 0 && y == 0; }

	void          SetNull()                         { x = y = Null; }
	bool          IsNullInstance() const            { return UPP::IsNull(x); }

	void          Offset(T dx, T dy)                { x += dx; y += dy; }

	Point_&       operator+=(Sz p)                  { x  += p.cx; y  += p.cy; return *this; }
	Point_&       operator+=(Point_ p)              { x  += p.x;  y  += p.y; return *this; }
	Point_&       operator+=(T t)                   { x  += t;    y  += t;    return *this; }
	Point_&       operator-=(Sz p)                  { x  -= p.cx; y  -= p.cy; return *this; }
	Point_&       operator-=(Point_ p)              { x  -= p.x;  y  -= p.y; return *this; }
	Point_&       operator-=(T t)                   { x  -= t;    y  -= t;    return *this; }
	Point_&       operator*=(Sz p)                  { x  *= p.cx; y  *= p.cy; return *this; }
	Point_&       operator*=(Point_ p)              { x  *= p.x;  y  *= p.y; return *this; }
	Point_&       operator*=(T t)                   { x  *= t;    y  *= t;    return *this; }
	Point_&       operator/=(Sz p)                  { x  /= p.cx; y  /= p.cy; return *this; }
	Point_&       operator/=(Point_ p)              { x  /= p.x;  y  /= p.y; return *this; }
	Point_&       operator/=(T t)                   { x  /= t;    y  /= t;    return *this; }
	Point_&       operator<<=(int sh)               { x <<= sh;   y <<= sh;   return *this; }
	Point_&       operator>>=(int sh)               { x >>= sh;   y >>= sh;   return *this; }

	Point_&       operator++()                      { ++x; ++y; return *this; }
	Point_&       operator--()                      { --x; --y; return *this; }

	friend Point_ operator+(Point_ p)               { return p; }
	friend Point_ operator-(Point_ p)               { return Point_(-p.x, -p.y); }

	friend Point_ operator+(Point_ a, Sz b)         { return a += b; }
	friend Point_ operator+(Point_ a, Point_ b)     { return a += b; }
	friend Point_ operator+(Point_ a, T t)          { return a += t; }
	friend Point_ operator+(T t, Point_ b)          { return b += t; }
	friend Sz     operator+(Sz a, Point_ b)         { return Point_(a) + b; }
	friend Sz&    operator+=(Sz& a, Point_ b)       { return a = a + b; }
	friend Point_ operator-(Point_ a, Sz b)         { return a -= b; }
	friend Point_ operator-(Point_ a, T t)          { return a -= t; }
	friend Sz     operator-(Point_ a, Point_ b)     { return a -= b; }
	friend Sz     operator-(Sz a, Point_ b)         { return Point_(a) - b; }
	friend Sz&    operator-=(Sz& a, Point_ b)       { return a = a - b; }
	friend Point_ operator*(Point_ a, Point_ b)     { return a *= b; }
	friend Point_ operator*(Point_ a, T b)          { return a *= b; }
	friend Point_ operator*(T a, Point_ b)          { return b *= a; }
	friend Point_ operator/(Point_ a, Sz b)         { return a /= b; }
	friend Point_ operator/(Point_ a, T b)          { return a /= b; }
	friend Point_ operator<<(Point_ a, int sh)      { return a <<= sh; }
	friend Point_ operator>>(Point_ a, int sh)      { return a >>= sh; }


	friend bool   operator==(Point_ a, Point_ b)    { return a.x == b.x && a.y == b.y; }
	friend bool   operator!=(Point_ a, Point_ b)    { return a.x != b.x || a.y != b.y; }

	friend Point_ min(Point_ a, Point_ b)           { return Point_(min(a.x, b.x), min(a.y, b.y)); }
	friend Point_ max(Point_ a, Point_ b)           { return Point_(max(a.x, b.x), max(a.y, b.y)); }

	friend Point_ Nvl(Point_ a, Point_ b)           { return IsNull(a) ? b : a; }

	unsigned      GetHashValue() const              { return CombineHash(x, y); }

	String        ToString() const;

	Point_() {}
	Point_(T x, T y) : x(x), y(y) {}

	Point_(const Point_<int>& pt) : x((T)pt.x), y((T)pt.y) {}
	Point_(const Point_<short>& pt) : x((T)pt.x), y((T)pt.y) {}
	Point_(const Point_<double>& pt) : x((T)pt.x), y((T)pt.y) {}
	Point_(const Point_<int64>& pt) : x((T)pt.x), y((T)pt.y) {}

	Point_(const Size_<T>& sz)  : x(sz.cx), y(sz.cy) {}

	Point_(const Nuller&)                           { x = y = Null; }

	operator Value() const                          { return FitsSvoValue<Point_>() ? SvoToValue(*this) : RichToValue(*this); }
	Point_(const Value& src)                        { *this = src.Get<Point_>(); }

	operator Ref()                                  { return AsRef(*this); }

	void Serialize(Stream& s)                       { s % x % y; }
	void Jsonize(JsonIO& jio)                       { jio("x", x)("y", y); }
	void Xmlize(XmlIO& xio)                         { xio.Attr("x", x).Attr("y", y); }

	int  Compare(const Point_&) const               { NEVER(); return 0; }
	int  PolyCompare(const Value&) const            { NEVER(); return 0; }

#ifdef PLATFORM_WIN32
	operator POINT*()                               { ASSERT(sizeof(*this) == sizeof(POINT)); return (POINT*)this; }
	operator const POINT*() const                   { ASSERT(sizeof(*this) == sizeof(POINT)); return (POINT*)this; }
	operator POINT() const                          { POINT p; p.x = x; p.y = y; return p; }
	LONG     GetLONG() const                        { return MAKELONG(x, y); }

	Point_(POINT pt) : x((T)pt.x), y((T)pt.y)       {}
	Point_(SIZE sz) : x((T)sz.cx), y((T)sz.cy)      {}
	explicit Point_(LONG lParam)                    { x = (T)(int16)LOWORD(lParam); y = (T)(int16)HIWORD(lParam); }
#endif
};

template <class T>
String Point_<T>::ToString() const {
	String ss;
	return ss << '[' << x << ", " << y << ']';
}

template <class T>
T GHalf_(T t) { return t >> 1; }

template <>
inline double GHalf_(double d) { return d / 2; }

template <class T>
struct Rect_ : Moveable< Rect_<T> > {
	T      left, top, right, bottom;

	typedef Point_<T>  Pt;
	typedef Size_<T>   Sz;

	void   Clear()                          { left = top = right = bottom = 0; }

	bool   IsEmpty() const                  { return right <= left || bottom <= top; }
	void   SetNull();
	bool   IsNullInstance() const;

	Pt     TopLeft() const                  { return Pt(left, top); }
	Pt     TopCenter() const                { return Pt(GHalf_(left + right), top); }
	Pt     TopRight() const                 { return Pt(right, top); }
	Pt     CenterLeft() const               { return Pt(left, GHalf_(top + bottom)); }
	Pt     CenterPoint() const              { return Pt(GHalf_(left + right), GHalf_(top + bottom)); }
	Pt     CenterRight() const              { return Pt(right, GHalf_(top + bottom)); }
	Pt     BottomLeft() const               { return Pt(left, bottom); }
	Pt     BottomCenter() const             { return Pt(GHalf_(left + right), bottom); }
	Pt     BottomRight() const              { return Pt(right, bottom); }
	T      Width() const                    { return right - left; }
	T      Height() const                   { return bottom - top; }
	Sz     Size() const                     { return Sz(right - left, bottom - top); }

	T      GetWidth() const                 { return right - left; }
	T      GetHeight() const                { return bottom - top; }
	Sz     GetSize() const                  { return Sz(right - left, bottom - top); }

	Pt     CenterPos(T cx, T cy) const;
	Pt     CenterPos(Sz sz) const           { return CenterPos(sz.cx, sz.cy); }
	Rect_  CenterRect(Sz sz) const          { return Rect_(CenterPos(sz), sz); }
	Rect_  CenterRect(T cx, T cy) const     { return CenterRect(Sz(cx, cy)); }

	void   Set(T l, T t, T r, T b)          { left = l; top = t; right = r; bottom = b; }
	void   Set(Pt a, Pt b)                  { left = a.x; top = a.y; right = b.x; bottom = b.y; }
	void   Set(Pt a, Sz sz)                 { Set(a, a + sz); }
	void   Set(const Rect_& r)              { Set(r.left, r.top, r.right, r.bottom); }

	void   SetSize(int cx, int cy)              { right = left + cx; bottom = top + cy; }
	void   SetSize(Sz sz)                       { SetSize(sz.cx, sz.cy); }

	void   InflateHorz(T dx)                    { left -= dx; right += dx; }
	void   InflateVert(T dy)                    { top -= dy; bottom += dy; }
	void   Inflate(T dx, T dy)                  { InflateHorz(dx); InflateVert(dy); }
	void   Inflate(Sz sz)                       { Inflate(sz.cx, sz.cy); }
	void   Inflate(T dxy)                       { Inflate(dxy, dxy); }
	void   Inflate(T l, T t, T r, T b)          { left -= l; top -= t; right += r; bottom += b; }
	void   Inflate(const Rect_& r)              { Inflate(r.left, r.top, r.right, r.bottom); }

	void   DeflateHorz(T dx)                    { InflateHorz(-dx); }
	void   DeflateVert(T dy)                    { InflateVert(-dy); }
	void   Deflate(T dx, T dy)                  { Inflate(-dx, -dy); }
	void   Deflate(Sz sz)                       { Inflate(-sz); }
	void   Deflate(T dxy)                       { Inflate(-dxy); }
	void   Deflate(T l, T t, T r, T b)          { Inflate(-l, -t, -r, -b); }
	void   Deflate(const Rect_& r)              { Deflate(r.left, r.top, r.right, r.bottom); }

	Rect_  InflatedHorz(T dx) const             { Rect_ r = *this; r.InflateHorz(dx); return r; }
	Rect_  InflatedVert(T dy) const             { Rect_ r = *this; r.InflateVert(dy); return r; }
	Rect_  Inflated(T dx, T dy) const           { Rect_ r = *this; r.Inflate(dx, dy); return r; }
	Rect_  Inflated(Sz sz) const                { Rect_ r = *this; r.Inflate(sz); return r; }
	Rect_  Inflated(T dxy) const                { Rect_ r = *this; r.Inflate(dxy); return r; }
	Rect_  Inflated(T l, T t, T r, T b) const   { Rect_ m = *this; m.Inflate(l, t, r, b); return m; }
	Rect_  Inflated(const Rect_& q) const       { Rect_ r = *this; r.Inflate(q); return r; }

	Rect_  DeflatedHorz(T dx) const             { Rect_ r = *this; r.DeflateHorz(dx); return r; }
	Rect_  DeflatedVert(T dy) const             { Rect_ r = *this; r.DeflateVert(dy); return r; }
	Rect_  Deflated(T dx, T dy) const           { Rect_ r = *this; r.Deflate(dx, dy); return r; }
	Rect_  Deflated(Sz sz) const                { Rect_ r = *this; r.Deflate(sz); return r; }
	Rect_  Deflated(T dxy) const                { Rect_ r = *this; r.Deflate(dxy); return r; }
	Rect_  Deflated(T l, T t, T r, T b) const   { Rect_ m = *this; m.Deflate(l, t, r, b); return m; }
	Rect_  Deflated(const Rect_& q) const       { Rect_ r = *this; r.Deflate(q); return r; }

	void   OffsetHorz(T dx)                     { left += dx; right += dx; }
	void   OffsetVert(T dy)                     { top += dy; bottom += dy; }
	void   Offset(T dx, T dy)                   { OffsetHorz(dx); OffsetVert(dy); }
	void   Offset(Sz sz)                        { Offset(sz.cx, sz.cy); }
	void   Offset(Pt p)                         { Offset(p.x, p.y); }

	Rect_  OffsetedHorz(T dx) const             { Rect_ r = *this; r.OffsetHorz(dx); return r; }
	Rect_  OffsetedVert(T dy) const             { Rect_ r = *this; r.OffsetVert(dy); return r; }
	Rect_  Offseted(T dx, T dy) const           { Rect_ r = *this; r.Offset(dx, dy); return r; }
	Rect_  Offseted(Sz sz) const                { Rect_ r = *this; r.Offset(sz); return r; }
	Rect_  Offseted(Pt p) const                 { Rect_ r = *this; r.Offset(p); return r; }

	void   Normalize();
	Rect_  Normalized() const                   { Rect_ r = *this; r.Normalize(); return r; }

	void   Union(Pt p);
	void   Union(const Rect_& rc);
	void   Intersect(const Rect_& rc);

	bool   Contains(T x, T y) const;
	bool   Contains(Pt p) const                 { return Contains(p.x, p.y); }
	bool   Contains(const Rect_& rc) const;
	bool   Intersects(const Rect_& rc) const;

	Pt     Bind(Pt pt) const;

	Rect_& operator+=(Sz sz)                                { Offset(sz); return *this; }
	Rect_& operator+=(Pt p)                                 { Offset(p); return *this; }
	Rect_& operator+=(const Rect_& b);
	Rect_& operator-=(Sz sz)                                { Offset(-sz); return *this; }
	Rect_& operator-=(Pt p)                                 { Offset(-p); return *this; }
	Rect_& operator-=(const Rect_& b);

	Rect_& operator|=(Pt p)                                 { Union(p); return *this; }
	Rect_& operator|=(const Rect_& rc)                      { Union(rc); return *this; }
	Rect_& operator&=(const Rect_& rc)                      { Intersect(rc); return *this; }

	bool   operator==(const Rect_& b) const;
	bool   operator!=(const Rect_& b) const                 { return !operator==(b); }

	friend Rect_ operator+(Rect_ a, Sz b)                   { return a += b; }
	friend Rect_ operator+(Sz a, Rect_ b)                   { return b += a; }
	friend Rect_ operator+(Rect_ a, Pt b)                   { return a += b; }
	friend Rect_ operator+(Pt a, Rect_ b)                   { return b += a; }
	friend Rect_ operator+(Rect_ a, const Rect_& b)         { return a += b; }
	friend Rect_ operator-(Rect_ a, Sz b)                   { return a -= b; }
	friend Rect_ operator-(Rect_ a, Pt b)                   { return a -= b; }
	friend Rect_ operator-(Rect_ a, const Rect_& b)         { return a -= b; }
	friend Rect_ operator|(Rect_ a, Rect_ b)                { a.Union(b); return a; }
	friend Rect_ operator&(Rect_ a, Rect_ b)                { a.Intersect(b); return a; }
	friend bool  operator&&(const Rect_& a, const Rect_& b) { return a.Intersects(b); }
	friend bool  operator>=(const Rect_& a, Pt b)           { return a.Contains(b); }
	friend bool  operator<=(Pt a, const Rect_& b)           { return b.Contains(a); }
	friend bool  operator<=(const Rect_& a, const Rect_& b) { return b.Contains(a); }
	friend bool  operator>=(const Rect_& b, const Rect_& a) { return a.Contains(b); }

	friend const Rect_& Nvl(const Rect_& a, const Rect_& b) { return IsNull(a) ? b : a; }

	unsigned     GetHashValue() const                       { return CombineHash(left, top, right, bottom); }

	String ToString() const;

	Rect_()                                     {}
	Rect_(T l, T t, T r, T b)                   { Set(l, t, r, b); }
	Rect_(Pt a, Pt b)                           { Set(a, b); }
	Rect_(Pt a, Sz sz)                          { Set(a, sz); }
	Rect_(Sz sz)                                { Set(0, 0, sz.cx, sz.cy); }

	Rect_(const Rect_<int>& r) { Set((T)r.left, (T)r.top, (T)r.right, (T)r.bottom); }
	Rect_(const Rect_<short>& r) { Set((T)r.left, (T)r.top, (T)r.right, (T)r.bottom); }
	Rect_(const Rect_<double>& r) { Set((T)r.left, (T)r.top, (T)r.right, (T)r.bottom); }
	Rect_(const Rect_<int64>& r) { Set((T)r.left, (T)r.top, (T)r.right, (T)r.bottom); }

	Rect_(const Nuller&)             { SetNull(); }

	operator Value() const           { return RichValue<Rect_>(*this); }
	/*explicit */Rect_(const Value& src) { *this = RichValue<Rect_>::Extract(src); }

	operator Ref()                   { return AsRef(*this); }

	void     Serialize(Stream& s) { s % left % top % right % bottom; }
	void     Jsonize(JsonIO& jio) { jio("left", left)("top", top)("right", right)("bottom", bottom); }
	void     Xmlize(XmlIO& xio)   { xio.Attr("left", left).Attr("top", top).Attr("right", right).Attr("bottom", bottom); }

	int      Compare(const Rect_&) const           { NEVER(); return 0; }
	int      PolyCompare(const Value&) const       { NEVER(); return 0; }

#ifdef PLATFORM_WIN32
	operator const RECT*() const { ASSERT(sizeof(*this) == sizeof(RECT)); return (RECT*)this; }
	operator RECT*()             { ASSERT(sizeof(*this) == sizeof(RECT)); return (RECT*)this; }
	operator RECT&()             { ASSERT(sizeof(*this) == sizeof(RECT)); return *(RECT*)this; }
	operator RECT() const        { RECT r; r.top = top; r.bottom = bottom;
									       r.left = left; r.right = right; return r; }
	Rect_(const RECT& rc)        { Set((T)rc.left, (T)rc.top, (T)rc.right, (T)rc.bottom); }
#endif
};

template <class T>
inline void Rect_<T>::SetNull()
{
	left = right = top = bottom = Null;
}

template <>
inline void Rect_<double>::SetNull()
{
	left = top = 0;
	right = bottom = -1;
}

template <class T>
bool Rect_<T>::IsNullInstance() const {
	return IsNull(left);
}

template <>
inline bool Rect_<double>::IsNullInstance() const {
	return left > right || top > bottom;
}

template <class T>
Point_<T> Rect_<T>::CenterPos(T cx, T cy) const {
	return Point_<T>(left + GHalf_(Width() - cx), top + GHalf_(Height() - cy));
}

template <class T>
void Rect_<T>::Normalize() {
	if(left > right) Swap(left, right);
	if(top > bottom) Swap(top, bottom);
}

template <class T>
void Rect_<T>::Union(Pt p) {
	if(IsNull(p)) return;
	if(IsNullInstance()) {
		right = 1 + (left = p.x);
		bottom = 1 + (top = p.y);
	}
	else
	{
		if(p.x >= right) right = p.x + 1;
		else if(p.x <  left) left = p.x;
		if(p.y >= bottom) bottom = p.y + 1;
		else if(p.y < top) top = p.y;
	}
}

template <>
inline void Rect_<double>::Union(Point_<double> p) {
	if(IsNull(p)) return;
	if(IsNullInstance())
	{
		left = right = p.x;
		top = bottom = p.y;
	}
	else
	{
		if(p.x < left) left = p.x;
		else if(p.x > right) right = p.x;
		if(p.y < top) top = p.y;
		else if(p.y > bottom) bottom = p.y;
	}
}

template <class T>
void Rect_<T>::Union(const Rect_<T>& r) {
	if(IsNull(r)) return;
	if(IsNullInstance()) {
		Set(r);
		return;
	}
	if(r.left < left) left = r.left;
	if(r.top < top) top = r.top;
	if(r.right > right) right = r.right;
	if(r.bottom > bottom) bottom = r.bottom;
}

void Rect_double_Union(Rect_<double>& self, const Rect_<double>& r);

template <>
inline void Rect_<double>::Union(const Rect_<double>& r) {
	Rect_double_Union(*this, r);
}

template <class T>
void Rect_<T>::Intersect(const Rect_<T>& r) {
	if(r.left > left) left = r.left;
	if(r.top > top) top = r.top;
	if(right < left) right = left;
	if(r.right < right) right = r.right;
	if(r.bottom < bottom) bottom = r.bottom;
	if(bottom < top) bottom = top;
}

template <class T>
bool Rect_<T>::Contains(T x, T y) const {
	return x >= left && x < right && y >= top && y < bottom;
}

template <>
inline bool Rect_<double>::Contains(double x, double y) const {
	return x >= left && x <= right && y >= top && y <= bottom;
}

template <class T>
bool Rect_<T>::Contains(const Rect_<T>& r) const {
	return r.left >= left && r.top >= top && r.right <= right && r.bottom <= bottom;
}

template <class T>
bool Rect_<T>::Intersects(const Rect_<T>& r) const {
	if(IsEmpty() || r.IsEmpty()) return false;
	return r.right > left && r.bottom > top && r.left < right && r.top < bottom;
}

bool Rect_double_Intersects(const Rect_<double>& self, const Rect_<double>& r);

template <>
inline bool Rect_<double>::Intersects(const Rect_<double>& r) const {
	return Rect_double_Intersects(*this, r);
}

template <class T>
Point_<T> Rect_<T>::Bind(Point_<T> pt) const {
	return Point_<T>(pt.x < left ? left : pt.x >= right ? right - 1 : pt.x,
		             pt.y < top ? top : pt.y >= bottom ? bottom - 1 : pt.y);
}

Point_<double> Rect_double_Bind(const Rect_<double>& self, Point_<double> pt);

template <>
inline Point_<double> Rect_<double>::Bind(Point_<double> pt) const {
	return Rect_double_Bind(*this, pt);
}

template <class T>
Rect_<T>& Rect_<T>::operator-=(const Rect_<T>& b) {
	left -= b.left;
	top -= b.top;
	right -= b.right;
	bottom -= b.bottom;
	return *this;
}

template <class T>
Rect_<T>& Rect_<T>::operator+=(const Rect_<T>& b) {
	left += b.left;
	top += b.top;
	right += b.right;
	bottom += b.bottom;
	return *this;
}

template <class T>
bool Rect_<T>::operator==(const Rect_& b) const {
	return top == b.top && bottom == b.bottom && left == b.left && right == b.right;
}

template <class T>
String Rect_<T>::ToString() const
{
	String str;
	return str << AsString(TopLeft()) << " - " << AsString(BottomRight())
	           << " : " << AsString(Size());
}

typedef Size_<int>     Size;
typedef Point_<int>    Point;
typedef Rect_<int>     Rect;

typedef Size_<int16>   Size16;
typedef Point_<int16>  Point16;
typedef Rect_<int16>   Rect16;

typedef Size_<double>  Sizef;
typedef Point_<double> Pointf;
typedef Rect_<double>  Rectf;

typedef Size_<int64>   Size64;
typedef Point_<int64>  Point64;
typedef Rect_<int64>   Rect64;

const int SIZE_V   = 70;
const int SIZE16_V = 71;
const int SIZEF_V  = 72;
const int SIZE64_V = 79;

template<> inline dword ValueTypeNo(const Size*)   { return SIZE_V; }
template<> inline dword ValueTypeNo(const Size16*) { return SIZE16_V; }
template<> inline dword ValueTypeNo(const Size64*) { return SIZE64_V; }
template<> inline dword ValueTypeNo(const Sizef*)  { return SIZEF_V; }

const int POINT_V   = 73;
const int POINT16_V = 74;
const int POINTF_V  = 75;
const int POINT64_V = 80;

template<> inline dword ValueTypeNo(const Point*)   { return POINT_V; }
template<> inline dword ValueTypeNo(const Point16*) { return POINT16_V; }
template<> inline dword ValueTypeNo(const Point64*) { return POINT64_V; }
template<> inline dword ValueTypeNo(const Pointf*)  { return POINTF_V; }

const int RECT_V   = 76;
const int RECT16_V = 77;
const int RECTF_V  = 78;
const int RECT64_V = 81;

template<> inline dword ValueTypeNo(const Rect*)   { return RECT_V; }
template<> inline dword ValueTypeNo(const Rect16*) { return RECT16_V; }
template<> inline dword ValueTypeNo(const Rect64*) { return RECT64_V; }
template<> inline dword ValueTypeNo(const Rectf*)  { return RECTF_V; }

Rect    RectC(int x, int y, int cx, int cy);
Rect16  Rect16C(int16 x, int16 y, int16 cx, int16 cy);
Rectf   RectfC(double x, double y, double cx, double cy);

inline Size& operator*=(Size& sz, double a)
{
	sz.cx = int(sz.cx * a);
	sz.cy = int(sz.cy * a);
	return sz;
}

inline Size& operator/=(Size& sz, double a)
{
	sz.cx = int(sz.cx / a);
	sz.cy = int(sz.cy / a);
	return sz;
}

inline Size& operator*=(Size& sz, Sizef a)
{
	sz.cx = int(sz.cx * a.cx);
	sz.cy = int(sz.cy * a.cy);
	return sz;
}

inline Size& operator/=(Size& sz, Sizef a)
{
	sz.cx = int(sz.cx / a.cx);
	sz.cy = int(sz.cy / a.cy);
	return sz;
}

inline Sizef operator*(Size sz, double a)   { return Sizef(sz.cx * a, sz.cy * a); }
inline Sizef operator*(double a, Size sz)   { return Sizef(sz.cx * a, sz.cy * a); }
inline Sizef operator/(Size sz, double a)   { return Sizef(sz.cx / a, sz.cy / a); }
inline Sizef operator*(Size sz, Sizef a)    { return Sizef(sz.cx * a.cx, sz.cy * a.cy); }
inline Sizef operator*(Sizef a, Size sz)    { return Sizef(sz.cx * a.cx, sz.cy * a.cy); }
inline Sizef operator/(Size sz, Sizef a)    { return Sizef(sz.cx / a.cx, sz.cy / a.cy); }

inline Size16& operator*=(Size16& sz, double a)
{
	sz.cx = int16(sz.cx * a);
	sz.cy = int16(sz.cy * a);
	return sz;
}

inline Size16& operator/=(Size16& sz, double a)
{
	sz.cx = int16(sz.cx / a);
	sz.cy = int16(sz.cy / a);
	return sz;
}

inline Size16& operator*=(Size16& sz, Sizef a)
{
	sz.cx = int16(sz.cx * a.cx);
	sz.cy = int16(sz.cy * a.cy);
	return sz;
}

inline Size16& operator/=(Size16& sz, Sizef a)
{
	sz.cx = int16(sz.cx / a.cx);
	sz.cy = int16(sz.cy / a.cy);
	return sz;
}

inline Sizef operator*(Size16 sz, double a)   { return Sizef(sz.cx * a, sz.cy * a); }
inline Sizef operator*(double a, Size16 sz)   { return Sizef(sz.cx * a, sz.cy * a); }
inline Sizef operator/(Size16 sz, double a)   { return Sizef(sz.cx / a, sz.cy / a); }
inline Sizef operator*(Size16 sz, Sizef a)  { return Sizef(sz.cx * a.cx, sz.cy * a.cy); }
inline Sizef operator*(Sizef a, Size16 sz)  { return Sizef(sz.cx * a.cx, sz.cy * a.cy); }
inline Sizef operator/(Size16 sz, Sizef a)  { return Sizef(sz.cx / a.cx, sz.cy / a.cy); }

inline Rect RectC(int x, int y, int cx, int cy) {
	return Rect(x, y, x + cx, y + cy);
}

inline Rect16  Rect16C(int16 x, int16 y, int16 cx, int16 cy) {
	return Rect16(x, y, x + cx, y + cy);
}

inline Rectf   RectfC(double x, double y, double cx, double cy) {
	return Rectf(x, y, x + cx, y + cy);
}

inline Rect  RectSort(Point a, Point b)    { return Rect(min(a, b), max(a, b) + 1); }
inline Rectf RectfSort(Pointf a, Pointf b) { return Rectf(min(a, b), max(a, b)); }

Stream& Pack16(Stream& s, Point& p);
Stream& Pack16(Stream& s, Size& sz);
Stream& Pack16(Stream& s, Rect& r);

Size iscale(Size a,  int b, int c);
Size iscalefloor(Size a,  int b, int c);
Size iscaleceil(Size a,  int b, int c);
Size idivfloor(Size a,  int b);
Size idivceil(Size a,  int b);
Size iscale(Size a,  Size b, Size c);
Size iscalefloor(Size a,  Size b, Size c);
Size iscaleceil(Size a,  Size b, Size c);
Size idivfloor(Size a,  Size b);
Size idivceil(Size a,  Size b);

enum Alignment {
	ALIGN_NULL,
	ALIGN_LEFT,
	ALIGN_TOP = ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_BOTTOM = ALIGN_RIGHT,
	ALIGN_CENTER,
	ALIGN_JUSTIFY,
};

Size        GetRatioSize(Size stdsize, int cx, int cy);
Size        GetFitSize(Size objsize, int cx, int cy);
inline Size GetFitSize(Size objsize, Size intosize) { return GetFitSize(objsize, intosize.cx, intosize.cy); }

Sizef GetFitSize(Sizef sz, double cx, double cy);
inline Sizef GetFitSize(Sizef objsize, Sizef intosize) { return GetFitSize(objsize, intosize.cx, intosize.cy); }

Pointf Mid(const Pointf& a, const Pointf& b);
Pointf Orthogonal(const Pointf& p);
double Squared(const Pointf& p);
double Length(const Pointf& p);
double Bearing(const Pointf& p);
double Distance(const Pointf& p1, const Pointf& p2);
double SquaredDistance(const Pointf& p1, const Pointf& p2);
Pointf Polar(double a);
Pointf Polar(const Pointf& p, double r, double a);
