#include "Geom.h"

namespace Upp {

double Distance(Pointf X, Pointf A, Pointf B, double *arg)
{ // distance from line segment (not line!)
	Pointf AB = B - A;
	Pointf AX = X - A;
	double ab2 = Squared(AB);
	double abx = AB ^ AX;
	if(abx <= 0 || ab2 == 0)
	{
		if(arg)
			*arg = 0;
		return X | A;
	}
	if(abx >= ab2)
	{
		if(arg)
			*arg = 1;
		return X | B;
	}
	if(arg)
		*arg = abx / ab2;
	return sqrt(sqr(AX % AB) / ab2);
}

double Distance(Pointf X, Pointf A, Pointf B, double bulge, double* arg)
{
	if(bulge == 0)
		return Distance(X, A, B, arg);
	return VecArcInfo(A, B, bulge).Distance(X, arg);
}

double Distance(Pointf X, Pointf C, double radius, double *arg)
{
	if(arg)
		*arg = Bearing(X, C) / TWOPI;
	return fabs((X | C) - radius);
}

enum { FI_B = 0x01, FI_I = 0x02, FI_A = 0x04 };

static inline char CheckAgainstInterval(double x, double l, double h)
{
	return x < l ? FI_B : x > h ? FI_A : FI_I;
}

bool Crosses(const Rectf& R, Pointf A, Pointf B)
{
	if(R.Contains(A) || R.Contains(B))
		return true;
	char ax = CheckAgainstInterval(A.x, R.left, R.right);
	char ay = CheckAgainstInterval(A.y, R.top,  R.bottom);
	char bx = CheckAgainstInterval(B.x, R.left, R.right);
	char by = CheckAgainstInterval(B.y, R.top,  R.bottom);
	if(((ax & bx) & (FI_B | FI_A)) || ((ay & by) & (FI_B | FI_A)))
		return false; // all in a by-plane
	if((ax & bx) & FI_I || (ay & by) & FI_I)
		return true; // crossing through
	if(!((ax | ay | bx | by) & FI_I))
	{ // diagonal cross
		double t, u;
		Pointf AB = B - A;
		double n = A % B;
		if(ax == ay)
		{
			t = R.TopRight()   % AB;
			u = R.BottomLeft() % AB;
		}
		else
		{
			t = R.TopLeft()     % AB;
			u = R.BottomRight() % AB;
		}
		return (t >= n && u <= n) || (t <= n && u >= n);
	}

	static const char clue1[4][4] =
	{
		{ 9, 0, 9, 1 },
		{ 2, 9, 9, 3 },
		{ 9, 9, 9, 9 },
		{ 4, 5, 9, 9 },
	};

	ax = clue1[bx - 1][ax - 1];
	ay = clue1[by - 1][ay - 1];

	ASSERT(ax != 9 && ay != 9);

	static const char clue2[6][6] =
	{
		{ 9, 7, 4, 7, 4, 9 },
		{ 0, 9, 4, 7, 9, 3 },
		{ 5, 5, 9, 9, 6, 6 },
		{ 0, 0, 9, 9, 3, 3 },
		{ 5, 9, 1, 2, 9, 6 },
		{ 9, 2, 1, 2, 1, 9 },
	};

	ax = clue2[(int)ay][(int)ax];

	double temp;
	switch(ax >> 1)
	{
	default: ASSERT(0); // invalid choice
	case 0: temp = (B - A) % (R.BottomLeft() - A); break;
	case 1: temp = (B - A) % (R.BottomRight() - A); break;
	case 2: temp = (B - A) % (R.TopLeft() - A); break;
	case 3: temp = (B - A) % (R.TopRight() - A); break;
	}

	return (ax & 1) ? temp >= 0 : temp <= 0;
}

bool Crosses(const Rectf& R, Pointf A, Pointf B, double bulge)
{
	if(R.Contains(A) || R.Contains(B))
		return true;

	return VecArcInfo(A, B, bulge).Crosses(R);
}

bool Crosses(const Rectf& R, Pointf C, double radius)
{
	Rectf box = PointfRectf(C);
	box.Inflate(radius);
	if(R.Contains(box))
		return true;
	if(!box.Intersects(R))
		return false;

	box = R - C;
	radius *= radius;

	int temp = 0;
	if(Squared(box.TopLeft())     > radius) temp |= 1;
	if(Squared(box.TopRight())    > radius) temp |= 2;
	if(Squared(box.BottomLeft())  > radius) temp |= 4;
	if(Squared(box.BottomRight()) > radius) temp |= 8;
	if(temp != 15)
		return temp != 0;
	return (box.left * box.right < 0 || box.top * box.bottom < 0);
}

bool ClipLine(Pointf& A, Pointf& B, const Rectf& R)
{
	if(IsNull(R) || R.IsEmpty()) // null rectangle
		return false;

	if(R.Contains(A) && R.Contains(B)) // both are in
		return true;

	bool rev = false;
	if(A.x > B.x)
	{
		rev ^= true;
		Swap(A, B);
	}

	// now clip x coordinate; A.x <= B.x
	if(B.x <= R.left || A.x >= R.right)
		return false;

	if(A.x < R.left)
	{
		A.y += (B.y - A.y) * (A.x - R.left) / (A.x - B.x);
		A.x  = R.left;
	}
	if(B.x > R.right)
	{
		B.y += (A.y - B.y) * (B.x - R.right) / (B.x - A.x);
		B.x  = R.right;
	}

	if(A.y > B.y)
	{
		rev ^= true;
		Swap(A, B);
	}

	// now clip y coordinate. A.y <= B.y
	if(B.y <= R.top || A.y >= R.bottom)
		return false;

	if(A.y < R.top)
	{
		A.x += (B.x - A.x) * (A.y - R.top) / (A.y - B.y);
		A.y  = R.top;
	}
	if(B.y > R.bottom)
	{
		B.x += (A.x - B.x) * (B.y - R.bottom) / (B.y - A.y);
		B.y  = R.bottom;
	}

	if(rev)
		Swap(A, B);

	return true;
}

bool ClipLine(Pointf& A, Pointf& B, const Rect& R)
{
	if(R.IsEmpty()) // null rectangle
		return false;

	if(A.x >= R.left && A.x <= R.right && A.y >= R.top && A.y <= R.bottom
	&& B.x >= R.left && B.x <= R.right && B.y >= R.top && B.y <= R.bottom)
		return true; // both are in

	bool rev = false;
	if(A.x > B.x)
	{
		rev ^= true;
		Swap(A, B);
	}

	// now clip x coordinate; A.x <= B.x
	if(B.x <= R.left || A.x >= R.right)
		return false;

	if(A.x < R.left)
	{
		A.y += (B.y - A.y) * (A.x - R.left) / (A.x - B.x);
		A.x  = R.left;
	}
	if(B.x > R.right)
	{
		B.y += (A.y - B.y) * (B.x - R.right) / (B.x - A.x);
		B.x  = R.right;
	}

	if(A.y > B.y)
	{
		rev ^= true;
		Swap(A, B);
	}

	// now clip y coordinate. A.y <= B.y
	if(B.y <= R.top || A.y >= R.bottom)
		return false;

	if(A.y < R.top)
	{
		A.x += (B.x - A.x) * (A.y - R.top) / (A.y - B.y);
		A.y  = R.top;
	}
	if(B.y > R.bottom)
	{
		B.x += (A.x - B.x) * (B.y - R.bottom) / (B.y - A.y);
		B.y  = R.bottom;
	}

	if(rev)
		Swap(A, B);

	return true;
}

bool ClipLine(Point& A, Point& B, const Rect& R)
{
	if(R.IsEmpty()) // null rectangle
		return false;

	if(A.x >= R.left && A.x <= R.right && A.y >= R.top && A.y <= R.bottom
	&& B.x >= R.left && B.x <= R.right && B.y >= R.top && B.y <= R.bottom)
		return true; // both are in

	bool rev = false;
	if(A.x > B.x)
	{
		rev ^= true;
		Swap(A, B);
	}

	// now clip x coordinate; A.x <= B.x
	if(B.x <= R.left || A.x >= R.right)
		return false;

	if(A.x < R.left)
	{
		A.y += (B.y - A.y) * (A.x - R.left) / (A.x - B.x);
		A.x  = R.left;
	}
	if(B.x > R.right)
	{
		B.y += (A.y - B.y) * (B.x - R.right) / (B.x - A.x);
		B.x  = R.right;
	}

	if(A.y > B.y)
	{
		rev ^= true;
		Swap(A, B);
	}

	// now clip y coordinate. A.y <= B.y
	if(B.y <= R.top || A.y >= R.bottom)
		return false;

	if(A.y < R.top)
	{
		A.x += (B.x - A.x) * (A.y - R.top) / (A.y - B.y);
		A.y  = R.top;
	}
	if(B.y > R.bottom)
	{
		B.x += (A.x - B.x) * (B.y - R.bottom) / (B.y - A.y);
		B.y  = R.bottom;
	}

	if(rev)
		Swap(A, B);

	return true;
}

Rectf GetBoundingBox(const Array<Pointf>& vertices)
{
	if(vertices.IsEmpty())
		return Null;
	Rectf rc = PointfRectf(vertices[0]);
	for(int i = 1; i < vertices.GetCount(); i++)
		SetUnion(rc, vertices[i]);
	return rc;
}

Rect GetBoundingBox(const Point *vertices, int vertex_count)
{
	if(vertex_count == 0)
		return Null;
	Rect rc(*vertices, Size(1, 1));
	const Point *e = vertices + vertex_count;
	while(++vertices < e)
	{
		if(vertices -> x < rc.left)    rc.left   = vertices -> x;
		if(vertices -> x >= rc.right)  rc.right  = vertices -> x + 1;
		if(vertices -> y < rc.top)     rc.top    = vertices -> y;
		if(vertices -> y >= rc.bottom) rc.bottom = vertices -> y + 1;
	}
	return rc;
}

Rect GetBoundingBox(const Vector<Point>& vertices)
{
	return GetBoundingBox(vertices.Begin(), vertices.GetCount());
}

template <class C, class VP>
class ConvexHullGenerator {
public:
	typedef typename C::ValueType P;
	ConvexHullGenerator(const C& points) : points(points) {}
	
	Vector<int> Generate();

	bool        operator () (int i, int j) const;
	
private:
	void        Recurse(int ib, int ie, int& ue, int& le);
	int         Stitch(int lb, int le, int rb, int re, bool is_lower);
	
	static VP   VectorProduct(const P& p, const P& a, const P& b)
	{ return VP(a.x - p.x) * VP(b.y - p.y) - VP(b.x - p.x) * VP(a.y - p.y); }

	const C& points;
	Vector<int> lower;
	Vector<int> upper;
};

template <class C, class VP>
Vector<int> ConvexHullGenerator<C, VP>::Generate()
{
	for(int i = 0; i < points.GetCount(); i++)
		if(!IsNull(points[i]))
			upper.Add(i);
	Vector<int> out;
	if(upper.IsEmpty())
		return out;
	if(upper.GetCount() == 1) {
		out.SetCount(1, upper[0]);
		return out;
	}
	Sort(upper, *this);
	int nnpt = upper.GetCount();
	lower <<= upper;
	int ue = nnpt;
	int le = nnpt;
	Recurse(0, nnpt, ue, le);
	int lb = 0;
	if(lower[lb] == upper[0])
		lb++;
	if(le > lb && lower[le - 1] == upper[ue - 1])
		le--;
	out.SetCount(ue + le - lb);
	Copy(out.Begin(), upper.Begin(), upper.GetIter(ue));
	const int *b = lower.GetIter(lb);
	const int *e = lower.GetIter(le);
	for(int *o = out.GetIter(ue); e > b; *o++ = *--e)
		;
	return out;
}

template <class C, class VP>
bool ConvexHullGenerator<C, VP>::operator () (int i, int j) const
{
	const P& a = points[i];
	const P& b = points[j];
	if(a.x != b.x) return a.x < b.x;
	return a.y < b.y;
}

template <class C, class VP>
void ConvexHullGenerator<C, VP>::Recurse(int ib, int ie, int &ue, int& le)
{
	if(ie - ib <= 2) {
		ue = le = ie;
		return;
	}
	int im = (ib + ie) >> 1;
	int aue, ale, bue, ble;
	Recurse(ib, im, aue, ale);
	Recurse(im, ie, bue, ble);
	ue = Stitch(ib, aue, im, bue, false);
	le = Stitch(ib, ale, im, ble, true);
}

template <class C, class VP>
int ConvexHullGenerator<C, VP>::Stitch(int lb, int le, int rb, int re, bool is_lower)
{
	Vector<int>& index = (is_lower ? lower : upper);
	for(;;) {
		const P& l1 = points[index[le - 1]];
		const P& r1 = points[index[rb + 0]];
		if(le - lb >= 2) {
			const P& l2 = points[index[le - 2]];
			VP prod = VectorProduct(l1, r1, l2);
			if(!is_lower)
				prod = -prod;
			if(prod >= 0) {
				le--;
				continue;
			}
		}
		if(re - rb >= 2) {
			const P& r2 = points[index[rb + 1]];
			VP prod = VectorProduct(r1, r2, l1);
			if(!is_lower)
				prod = -prod;
			if(prod >= 0) {
				rb++;
				continue;
			}
		}
		break;
	}
	Copy(index.GetIter(le), index.GetIter(rb), index.GetIter(re));
	return le + (re - rb);
}

Vector<int> ConvexHullOrder(const Vector<Point>& points)
{
	ConvexHullGenerator<Vector<Point>, int64> generator(points);
	return generator.Generate();
}

Vector<int> ConvexHullOrder(const Array<Pointf>& points)
{
	ConvexHullGenerator<Array<Pointf>, double> generator(points);
	return generator.Generate();
}

//////////////////////////////////////////////////////////////////////
// VecArcIterator::

VecArcIterator::VecArcIterator(Pointf start, Pointf end, double bulge, Callback1<Pointf> lineto)
	: arc(start, end, bulge), clip(Null), lineto(lineto)
{
	level = DEFAULT_LEVEL;
	precision = 1; // default precision
}

//////////////////////////////////////////////////////////////////////

VecArcIterator::VecArcIterator(const VecArc& arc, Callback1<Pointf> lineto)
	: arc(arc), clip(Null), lineto(lineto)
{
	level = DEFAULT_LEVEL;
	precision = 1; // default precision
}

//////////////////////////////////////////////////////////////////////

void VecArcIterator::Go()
{
	ASSERT(precision >= 0);
	last = arc.A;
	last_clip = (IsNull(clip) ? 0 : GetClip(arc.A));
	int end_clip = (IsNull(clip) ? 0 : GetClip(arc.B));
	arclen = 0;
	if(!IsNull(clip) && arcto)
		arclen = Diagonal(clip);
	double l = arc.VecLine::Length();
	if(l <= precision && fabs(arc.bulge) <= precision)
		return;
	if(2 * fabs(arc.bulge) <= l) // less than semicircle - generate all in one go
		Recurse(arc.B, l, arc.bulge, level, end_clip);
	else
	{ // divide in two halves
		double ll, hh;
		Pointf centre;
		VecArc::Bisect(arc.A, arc.B, l, arc.bulge, centre, ll, hh);
		int centre_clip = IsNull(clip) ? 0 : GetClip(centre);
		Recurse(centre, ll, hh, level - 1, centre_clip);
		Recurse(arc.B, ll, hh, level - 1, end_clip);
	}
}

//////////////////////////////////////////////////////////////////////

void VecArcIterator::Recurse(Pointf next, double l, double h, int depth, int next_clip)
{
	if(--depth < 0 || fabs(h) <= precision || fabs(l) <= precision)
		lineto(next); // degenerate to line
	else
	{ // bisect arc
		if(l < arclen)
    	{
			ASSERT(!IsNull(clip) && arcto);
			if(2 * Distance(clip, Mid(last, next)) <= l + precision)
				if(arcto(next, h))
				{
					last = next;
					last_clip = next_clip;
					return;
				}
		}
		Pointf normal = Right(next - last) * h / l;
		Pointf centre = Mid(last, next) + normal;
		double ll = centre | next;
		double hh = h * ll / (ll + ll + l);
		int centre_clip = 0;
		if(last_clip | next_clip)
		{
			if(last_clip & next_clip & GetClip(last + normal) & GetClip(next + normal))
			{ // clip this out
				moveto(next);
				last = next;
				last_clip = next_clip;
				return;
			}
			centre_clip = GetClip(centre);
		}
		Recurse(centre, ll, hh, depth, centre_clip);
		Recurse(next, ll, hh, depth, next_clip);
	}
	last = next;
	last_clip = next_clip;
}

//////////////////////////////////////////////////////////////////////

int VecArcIterator::GetClip(Pointf point) const
{
	return (point.x < clip.left ? CF_XL : point.x > clip.right  ? CF_XG : 0)
		|  (point.y < clip.top  ? CF_YL : point.y > clip.bottom ? CF_YG : 0);
}

//////////////////////////////////////////////////////////////////////

double Vec_tolerance       = 1e-6;
double Vec_ang_tolerance   = 1e-6;
double Vec_outer_tolerance = 1e30;

//////////////////////////////////////////////////////////////////////

bool VecTolEq(double x, double y)
{
	return fabs(x - y) <= Vec_tolerance;
}

//////////////////////////////////////////////////////////////////////

bool VecTolEq(Pointf a, Pointf b)
{
	return VecTolEq(a.x, b.x) && VecTolEq(a.y, b.y);
}

//////////////////////////////////////////////////////////////////////

VecLine::VecLine()
: A(Null), B(Null)
{
}

//////////////////////////////////////////////////////////////////////

VecLine& VecLine::SetNull()
{
	A = B = Null;
	return *this;
}

//////////////////////////////////////////////////////////////////////

double VecLine::Distance(Pointf point, double* arg) const
{
	if(IsNull(point) || IsNull(*this))
		return Null;
	return UPP::Distance(point, A, B, arg);
}

//////////////////////////////////////////////////////////////////////

Pointf VecLine::Mid() const
{
	return UPP::Mid(A, B);
}

//////////////////////////////////////////////////////////////////////

Pointf VecLine::Right() const
{
	return UPP::Right(Vector());
}

//////////////////////////////////////////////////////////////////////

Pointf VecLine::Left() const
{
	return UPP::Left(Vector());
}

//////////////////////////////////////////////////////////////////////

Pointf VecLine::Vector() const
{
	return B - A;
}

//////////////////////////////////////////////////////////////////////

VecLine VecLine::Reversed() const
{
	return VecLine(B, A);
}

//////////////////////////////////////////////////////////////////////

VecLine& VecLine::SetReversed()
{
	Swap(A, B);
	return *this;
}

//////////////////////////////////////////////////////////////////////

VecLine& VecLine::SetClip(const Rectf& R)
{
	if(IsNull(*this)) // null line
		return *this;

	if(IsNull(R) || R.IsEmpty()) // null rectangle
		return SetNull();

	if(R.Contains(A) && R.Contains(B)) // both are in
		return *this;

	bool rev = false;
	if(A.x > B.x)
	{
		rev ^= true;
		SetReversed();
	}

	// now clip x coordinate; A.x <= B.x
	if(B.x <= R.left || A.x >= R.right)
		return SetNull();

	if(A.x < R.left)
	{
		A.y += (B.y - A.y) * (A.x - R.left) / (A.x - B.x);
		A.x  = R.left;
	}
	if(B.x > R.right)
	{
		B.y += (A.y - B.y) * (B.x - R.right) / (B.x - A.x);
		B.x  = R.right;
	}

	if(A.y > B.y)
	{
		rev ^= true;
		SetReversed();
	}

	// now clip y coordinate. A.y <= B.y
	if(B.y <= R.top || A.y >= R.bottom)
		return SetNull();

	if(A.y < R.top)
	{
		A.x += (B.x - A.x) * (A.y - R.top) / (A.y - B.y);
		A.y  = R.top;
	}
	if(B.y > R.bottom)
	{
		B.x += (A.x - B.x) * (B.y - R.bottom) / (B.y - A.y);
		B.y  = R.bottom;
	}

	if(rev)
		SetReversed();

	return *this;
}

//////////////////////////////////////////////////////////////////////

double VecLine::GetArg(Pointf pt) const
{
	Pointf AB = Vector();
	return minmax<double>(((pt - A) ^ AB) / Squared(AB), 0, 1);
}

//////////////////////////////////////////////////////////////////////

Pointf VecLine::GetPointAt(Pointf A, Pointf B, double arg)
{
	return A + (B - A) * arg;
}

//////////////////////////////////////////////////////////////////////

Pointf VecLine::GetPointAt(double arg) const
{
	return GetPointAt(A, B, arg);
}

//////////////////////////////////////////////////////////////////////

String Dump(VecLine line)
{
	String out;
	if(IsNull(line))
		out = "Null(VecLine)";
	else
		out << "VecLine(" << line.A << " -> " << line.B << ")";
	return out;
}

//////////////////////////////////////////////////////////////////////
// VecArc::

VecArc::VecArc()
	: bulge(Null)
{
}

//////////////////////////////////////////////////////////////////////

VecArc VecArc::Reversed() const
{
	return VecArc(B, A, -bulge);
}

//////////////////////////////////////////////////////////////////////

VecArc& VecArc::SetReversed()
{
	VecLine::SetReversed();
	bulge = -bulge;
	return *this;
}

//////////////////////////////////////////////////////////////////////

Pointf VecArc::ArcMid() const
{
	return ArcMid(A, B, A | B, bulge);
}

//////////////////////////////////////////////////////////////////////

double VecArc::ArcLength() const
{
	return ArcLength(A, B, A | B, bulge);
}

//////////////////////////////////////////////////////////////////////

Rectf VecArc::GetBoundingBox() const
{
	Rectf result = SortRectf(A, B);
	double l = A | B;
	if(fabs(bulge) <= Vec_tolerance || tabs(l) <= Vec_tolerance)
		return result;
	if(2 * fabs(bulge) >= l)
	{ // more than semiarc - split in two
		Pointf centre;
		double ll, hh;
		Bisect(A, B, l, bulge, centre, ll, hh);
		return VecArc(A, centre, hh).GetBoundingBox()
		| VecArc(centre, B, hh).GetBoundingBox();
	}

	double r = bulge / 2 + l * l / (8 * bulge);
	Pointf C = UPP::Mid(A, B) + UPP::Right(B - A) * ((bulge - r) / l);
	int q1 = (A.x < C.x) ^ (A.y < C.y ? 3 : 0);
	int q2 = (B.x < C.x) ^ (B.y < C.y ? 3 : 0);
	if(r < 0) // swap quadrants
	{
		int temp = q2; q2 = q1; q1 = temp;
		r = -r;
	}
	if(q2 < q1 || (q2 == q1 && bulge >= r))
		q2 += 4;
	q1 = ((1 << q2) - 1) ^ ((1 << q1) - 1);
	if(q1 & 0x11)
		SetUnion(result, C + Pointf(0, +r));
	if(q1 & 0x22)
		SetUnion(result, C + Pointf(-r, 0));
	if(q1 & 0x44)
		SetUnion(result, C + Pointf(0, -r));
	if(q1 & 0x88)
		SetUnion(result, C + Pointf(+r, 0));

	return result;
}

//////////////////////////////////////////////////////////////////////

String Dump(const VecArc& arc)
{
	String out;
	if(IsNull(arc))
		out = "Null(VecArc)";
	else
		out << "VecArc(" << arc.A << " -> " << arc.B << ", bulge = " << arc.bulge << ")";
	return out;
}

//////////////////////////////////////////////////////////////////////

Pointf VecArc::ArcMid(Pointf P, Pointf Q, double l, double h)
{
	Pointf result = UPP::Mid(P, Q);
	if(l > Vec_tolerance)
		result += UPP::Right(Q - P) * (h / l);
	return result;
}

//////////////////////////////////////////////////////////////////////

double VecArc::ArcLength(Pointf P, Pointf Q, double l, double h)
{
	l *= 0.5;
	double ll = sqrt(l * l + h * h);
	if(l <= Vec_tolerance)
		return 0; // too short, sorry
	if((l /= ll) <= 1e-10) // arcsin(x) \approx x
		return 4 * ll;
	return 4 * ll * asin(l) / l;
}

//////////////////////////////////////////////////////////////////////

Pointf VecArc::GetPointAt(double t) const
{
	return GetPointAt(A, B, A | B, bulge, t);
}

//////////////////////////////////////////////////////////////////////

Pointf VecArc::GetPointAt(Pointf P, Pointf Q, double l, double h, double t)
{
	if(l <= Vec_tolerance || fabs(h) <= Vec_tolerance)
		return VecLine::GetPointAt(P, Q, t);
	double k = l * 0.5;
	double lambda = h / k;
	Pointf C = UPP::Mid(P, Q);
	double beta = 2 * atan(lambda);
	t = sin((2 * t - 1) * beta) / sin(beta);
	C += (Q - P) * (t / 2);
//	double k2 = k * k;
//	double k4 = k2 * k2;
	double l2 = lambda * lambda;
	double opl = 1 + l2;
	t *= t;
	double denom = sqrt(opl * opl - 4 * l2 * t) + 1 - l2;
	if(denom > Vec_tolerance)
		C += UPP::Right(Q - P) * (4 * l2 * (1 - t) / denom);
	return C;
}

//////////////////////////////////////////////////////////////////////

void VecArc::Bisect(Pointf P, Pointf Q, double l, double h, Pointf& centre, double& ll, double& hh)
{
	centre = UPP::Mid(P, Q) + UPP::Right(Q - P) * h / l;
	ll = sqrt(l * l / 4 + h * h);
	hh = h * ll / (2 * ll + l);
}

//////////////////////////////////////////////////////////////////////
// VecVertex::

/*
VecVertex::VecVertex()
	: Pointf(Null), bulge(0)
{
}

//////////////////////////////////////////////////////////////////////

String VecVertex::Dump() const
{
	return Pointf::Dump() + Format(" (%.10g)", bulge);
}

//////////////////////////////////////////////////////////////////////
// VecCurve::

VecCurve::VecCurve()
{
}

//////////////////////////////////////////////////////////////////////

VecCurve::VecCurve(const VecCurve& another, int)
	: Base(another, 0)
{
}

//////////////////////////////////////////////////////////////////////

void VecCurve::Add(const VecVertex& vertex)
{
	Base::Add(vertex);
}

//////////////////////////////////////////////////////////////////////

void VecCurve::Add(Pointf point)
{
	Base::Add(point);
}

//////////////////////////////////////////////////////////////////////

void VecCurve::Add(Pointf point, double bulge)
{
	Base::Add(VecVertex(point, bulge));
}

//////////////////////////////////////////////////////////////////////

void VecCurve::Add(double x, double y, double bulge)
{
	Base::Add(VecVertex(x, y, bulge));
}

//////////////////////////////////////////////////////////////////////

String VecCurve::Dump() const
{
	if(IsEmpty())
		return "Null(VecCurve)";
	String dump = '(' + (*this)[0].Dump();
	for(int i = 1; i < GetCount(); i++)
		dump << " -> " << (*this)[i].Dump();
	return dump + ')';
}

//////////////////////////////////////////////////////////////////////

Vector<Pointf> VecCurve::Check() const
{
	Vector<Pointf> errors;
	int count = GetCount();
	if(count < 2)
		return errors; // 1 or 0 vertices - no errors
	if(count == 2)
	{ // special case: segment bulges & vertices should differ
		const VecVertex& A = (*this)[0];
		const VecVertex& B = (*this)[1];
		if(VecTolEq(A, B))
			errors.Add(A);
		else if(VecTolEq(A.bulge, -B.bulge)) // same bulges - add centre of the arc
			errors.Add(VecArc(A, B, B.bulge).ArcMid());
		return errors;
	}
	return errors;
}

//////////////////////////////////////////////////////////////////////

VecArc VecCurve::Segment(int i) const
{
	int count = GetCount();
	ASSERT(i >= 0 && i < count);
	int j = i + 1;
	if(j >= count)
		j = 0;
	const VecVertex& A = (*this)[i];
	const VecVertex& B = (*this)[j];
	return VecArc(A, B, B.bulge);
}
*/

//////////////////////////////////////////////////////////////////////
///// VecArcInfo::

VecArcInfo::VecArcInfo()
	: C(Null)
	, bow(Null)
	, alpha(Null)
	, beta(Null)
	, radius(Null)
{
}

///// methods

//////////////////////////////////////////////////////////////////////
// Set: sets arc defined by centre, endpoints & orientation.

void VecArcInfo::Set(Pointf _C, Pointf _A, Pointf _B, bool anticlockwise)
{
	A = _A;
	B = _B;
	C = _C;
	reversed = !anticlockwise;
	bow = A | B;
	alpha = UPP::Bearing(A - C);
	beta = UPP::Bearing(B - C);
	radius = B | C;
	circle = (A == B);
	curved = circle || (bow > Vec_tolerance);
	if(reversed)
		Swap(alpha, beta);
	while(beta <= alpha)
		beta += 2 * M_PI;
	bulge = Null;
	if(circle)
		bulge = 2 * radius;
	else if(curved)
	{
		bulge = (reversed ? -radius : +radius) - ((B - A) % (C - A)) / bow;
		curved = (fabs(bulge) > Vec_tolerance);
	}
}

//////////////////////////////////////////////////////////////////////
// Set: sets up a line segment (degenerated arc).
//////////////////////////////////////////////////////////////////////

void VecArcInfo::Set(Pointf _P, Pointf _Q)
{
	A        = _P;
	B        = _Q;
	bow      = A | B;
	bulge    = 0;

	alpha    = Null;
	beta     = Null;
	radius   = Null;
	C        = Null;

	curved   = false;
	reversed = false;
	circle   = false;
}

//////////////////////////////////////////////////////////////////////
// Set: sets up arc defined by endpoints and a point on arc.
//////////////////////////////////////////////////////////////////////

void VecArcInfo::Set(Pointf _P, Pointf _Q, Pointf X)
{
	circle = false;

	bow = _P | _Q;

	if(bow <= Vec_tolerance)
	{ // singular bow: A = B
		Set(_P, _Q);
		return;
	}

	double x2  = UPP::Squared(X);
	Pointf     AX  = X - _P;
	Pointf     BX  = X - _Q;
	double m1x = (x2 - UPP::Squared(_P)) / 2;
	double m1y = (x2 - UPP::Squared(_Q)) / 2;
	double det = AX % BX;
	Pointf     dc(m1x * BX.y - m1y * AX.y, m1y * AX.x - m1x * BX.x);

	if(fabs(det) * Vec_outer_tolerance <= max(fabs(dc.x), fabs(dc.y)))
		Set(_P, _Q);
	else
	{
		A = _P;
		B = _Q;
		curved    = true;
		reversed  = false;

		C         = dc / det;
		radius    = X | C;
		alpha     = UPP::Bearing(A - C);
		beta      = UPP::Bearing(B - C);

		if(det >= 0)
		{ // reverse arc direction
			Swap(alpha, beta);
			reversed = true;
		}
		if(beta < alpha)
			beta += TWOPI;

		bulge = (det >= 0 ? -radius : +radius) - ((B - A) % (C - A)) / bow;
	}
}

//////////////////////////////////////////////////////////////////////
// Set: sets up arc defined by chord and bulge.
//////////////////////////////////////////////////////////////////////

void VecArcInfo::Set(Pointf _P, Pointf _Q, double _bulge)
{
	circle = false;

	bow = _P | _Q;

	if(bow <= Vec_tolerance || fabs(_bulge) <= Vec_tolerance)
	{ // singular bow
		Set(_P, _Q);
		return;
	}

	A        = _P;
	B        = _Q;
	bulge    = _bulge;
	curved   = true;
	reversed = false;

	radius = _bulge / 2 + bow * bow / (8 * _bulge);
	C      = UPP::Mid(A, B) + UPP::Right(B - A) * ((_bulge - radius) / bow);
	alpha  = UPP::Bearing(A - C);
	beta   = UPP::Bearing(B - C);

	if(bulge < 0)
	{ // reverse arc direction
		double temp;
		temp = alpha; alpha = beta; beta = temp;
		reversed = true;
	}
	if(beta < alpha)
		beta += TWOPI;
	if(radius < 0)
		radius = -radius;
}

//////////////////////////////////////////////////////////////////////
// Set: sets up arc defined by centre, radius & angles.
//////////////////////////////////////////////////////////////////////

void VecArcInfo::Set(Pointf _C, double r, double a, double b)
{
	if((r = fabs(r)) <= Vec_tolerance)
	{ // singular arc: zero radius
		Set(_C, _C);
		return;
	}

	C      = _C;
	radius = r;
	alpha  = modulo(a, TWOPI);

	double delta = modulo(b - a, TWOPI);
	beta   = alpha + delta;

	A        = C + PolarPointf(r, a);
	B        = C + PolarPointf(r, b);
	bulge    = radius * (1 - cos(delta / 2));
	bow      = A | B;
	circle   = false;
	curved   = true;
	reversed = false;
}

//////////////////////////////////////////////////////////////////////
// Set: sets the arc to a full circle.
//////////////////////////////////////////////////////////////////////

void VecArcInfo::Set(Pointf _C, double r)
{
	circle   = true;
	curved   = true;
	reversed = false;
	C        = _C;
	radius   = fabs(r);
	alpha    = 0;
	beta     = 2 * M_PI;
	A        = C;
	A.x     += radius;
	B        = A;
	bow      = 0;
	bulge    = 2 * radius;
}

//////////////////////////////////////////////////////////////////////

Pointf VecArcInfo::CentreOfMass() const
{
	if(IsCircle())
		return C;
	Pointf M = UPP::Mid(A, B);
	if(!IsCurved())
		return M;
	double extrusion = bow / (beta - alpha) - radius + fabs(bulge);
	return M + UnitV(IsReversed() ? UPP::Left(M) : UPP::Right(M)) * extrusion;
}

//////////////////////////////////////////////////////////////////////

void VecArcInfo::Move(Pointf offset)
{
	if(!IsNull(A)) A += offset;
	if(!IsNull(B)) B += offset;
	if(!IsNull(C)) C += offset;
}

//////////////////////////////////////////////////////////////////////

double VecArcInfo::Length() const
{
	if(IsCircle())
		return TWOPI * radius;
	if(IsCurved())
		return (beta - alpha) * radius;
	return bow;
}

//////////////////////////////////////////////////////////////////////
// ContainsBearing: checks whether arc contains given bearing.
//////////////////////////////////////////////////////////////////////

bool VecArcInfo::ContainsBearing(double bearing) const
{
	if(IsCircle())
		return true;

	if(radius <= Vec_tolerance)
		return false;

	double tol = Vec_tolerance / radius;

	if(bearing >= alpha - tol && bearing <= beta + tol
	|| modulo(bearing - alpha + tol, TWOPI) <= beta - alpha + 2 * tol)
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////
// GetPointAt: returns point on parametric curve.
//////////////////////////////////////////////////////////////////////

Pointf VecArcInfo::GetPointAt(double t) const
{
	if(IsCircle())
		return PolarPointf(C, radius, t * (reversed ? -TWOPI : TWOPI) + alpha);
	else if(IsCurved())
		return PolarPointf(C, radius, alpha
		+ (reversed ? 1 - t : t) * (beta - alpha));
	else
		return A + (B - A) * t;
}

//////////////////////////////////////////////////////////////////////

VecArcInfo& VecArcInfo::SetReversed()
{
	if(IsCircle())
		reversed ^= true;
	else if(IsCurved())
	{
		Swap(A, B);
		bulge = -bulge;
		reversed ^= true;
	}
	else
		Swap(A, B);
	return *this;
}

//////////////////////////////////////////////////////////////////////
// Dump: displays arc information on given stream.
//////////////////////////////////////////////////////////////////////

String Dump(const VecArcInfo& arc)
{
	String output;
	if(arc.IsCircle())
	{
		output
			<< "VecArcInfo (circle) { centre: " << arc.C
			<< ", radius: " << arc.radius;
	}
	else
	{
		output << "VecArcInfo" << (arc.IsCurved() ? " (arc)" : " (line)")
			<< " { bow " << arc.A << " -> " << arc.B << ", length: " << arc.bow;
		if(arc.IsCurved())
			output
				<< ", bulge: "  << arc.bulge
				<< ", centre: " << arc.C
				<< ", radius: " << arc.radius
				<< ", alpha: "  << arc.alpha / DEGRAD
				<< ", beta: "   << arc.beta / DEGRAD;
	}
	output << " }";
	return output;
}

//////////////////////////////////////////////////////////////////////
// Distance: distance between arc and point.

double VecArcInfo::Distance(Pointf X, double* arg) const
{
	if(IsCircle())
	{
		if(arg)
			*arg = GetArg(X);
		return fabs((X | C) - radius);
	}
	if(!IsCurved())
		return UPP::Distance(X, A, B, arg);

	bool a = (A - C) % (X - C) > 0;
	bool b = (X - C) % (B - C) > 0;
	if(IsReversed())
	{
		a ^= true;
		b ^= true;
	}

	if(2 * fabs(bulge) < bow)
	{
		if(a && b)
		{
			if(arg)
				*arg = GetArg(X);
			return fabs((X | C) - radius);
		}
	}
	else
	{
		if(a || b)
		{
			if(arg)
				*arg = GetArg(X);
			return fabs((X | C) - radius);
		}
	}
	double da = X | A, db = X | B;
	if(da < db)
	{
		if(arg)
			*arg = 0;
		return da;
	}
	else
	{
		if(arg)
			*arg = 1;
		return db;
	}
}

//////////////////////////////////////////////////////////////////////

double VecArcInfo::GetMaxDistance(Pointf point, Pointf *farthest) const
{
	if(IsCircle() || IsCurved() && ContainsBearing(UPP::Bearing(C, point)))
	{
		double distance = (point | C) + radius;
		if(farthest)
			*farthest = point + UPP::Length(C - point, radius);
		return distance;
	}

	double l1 = point | A, l2 = point | B;
	if(farthest)
		*farthest = (l1 >= l2 ? A : B);
	return max(l1, l2);
}

//////////////////////////////////////////////////////////////////////

bool VecArcInfo::Crosses(const Rectf& R) const
{
	if(IsNull(*this))
		return false;

	if(!IsCurved())
		return UPP::Crosses(R, A, B);

	if(R.Contains(A) || R.Contains(B))
		return true;

	if(!UPP::Crosses(R, C, radius))
		return false;

	if(IsCircle())
		return true;

	VecIntersection dummy;
	Pointf K = R.TopLeft();
	Pointf L = R.TopRight();
	Pointf M = R.BottomRight();
	Pointf N = R.BottomLeft();

	return dummy.LA(K, L, *this) || dummy.LA(L, M, *this)
		|| dummy.LA(M, N, *this) || dummy.LA(N, K, *this);
}

//////////////////////////////////////////////////////////////////////

double VecArcInfo::GetArg(double bearing) const
{
	bearing = modulo(bearing - alpha, 2 * M_PI);
	double included = beta - alpha;
	if(included)
		bearing /= included;
	return minmax<double>(IsReversed() ? 1 - bearing : bearing, 0, 1);
}

//////////////////////////////////////////////////////////////////////

double VecArcInfo::GetArg(Pointf point) const
{
	if(!IsCurved())
		return VecLine::GetArg(point);
	return GetArg(UPP::Bearing(point, C));
}

//////////////////////////////////////////////////////////////////////

Rectf VecArcInfo::GetBoundingBox() const
{
	if(IsCircle())
		return Rectf(C.x - radius, C.y - radius, C.x + radius, C.y + radius);
	else if(IsCurved())
		return VecArc::GetBoundingBox();
	return SortRectf(A, B);
}

//////////////////////////////////////////////////////////////////////
// GetAngle: orientovany stredovy uhel oblouku. Kladny, pokud jde
// oblouk proti smeru h.r.

double VecArcInfo::GetAngle() const
{
	if(IsCircle())
		return IsReversed() ? -TWOPI : +TWOPI;
	if(!IsCurved())
		return 0;
	double angle = beta - alpha;
	if(angle <= 0)
		angle += 2 * M_PI;
	return (IsReversed() ? -angle : +angle);
}

//////////////////////////////////////////////////////////////////////
// GetStartDir: returns tangent direction at entity beginning.

Pointf VecArcInfo::GetStartDir() const
{
	if(IsCurved())
	{
		Pointf CA = UPP::Left(A - C);
		if(IsReversed())
			CA = -CA;
		return CA;
	}
	return B - A;
}

//////////////////////////////////////////////////////////////////////
// GetEndDir: returns tangent direction at entity end.

Pointf VecArcInfo::GetEndDir() const
{
	if(IsCurved())
	{
		Pointf CB = UPP::Left(B - C);
		if(IsReversed())
			CB = -CB;
		return CB;
	}
	return B - A;
}

//////////////////////////////////////////////////////////////////////
// GetStartTangent: returns tangent angle at entita beginning.

double VecArcInfo::GetStartTangent() const
{
	return UPP::Bearing(GetStartDir());
}

//////////////////////////////////////////////////////////////////////
// GetEndTangent: returns tangent angle at entita end.

double VecArcInfo::GetEndTangent() const
{
	return UPP::Bearing(GetEndDir());
}

//////////////////////////////////////////////////////////////////////

VecArcInfo VecArcInfo::Offset(double dist) const
{
	if(dist == 0)
		return *this;
	Pointf last = A + UPP::UnitV(UPP::Right(GetStartDir())) * dist;
	Pointf next = B + UPP::UnitV(UPP::Right(GetEndDir())) * dist;
	if(IsCurved())
		return VecArcInfo(C, last, next, !IsReversed());
	else
		return VecArcInfo(last, next);
}

//////////////////////////////////////////////////////////////////////

VecArcInfo VecArcInfo::Subset(double start, double end) const
{
	if(IsCircle())
	{
		VecArcInfo info;
		if(IsReversed())
		{
			info.Set(C, radius, alpha - end * TWOPI, alpha - start * TWOPI);
			info.SetReversed();
		}
		else
			info.Set(C, radius, alpha + start * TWOPI, alpha + end * TWOPI);
		return info;
	}
	else if(IsCurved())
		return VecArcInfo(GetPointAt(start), GetPointAt(end),
			(IsReversed() ? -radius : +radius) * (1 - cos(0.5 * (end - start) * GetAngle())));
	else
		return VecArcInfo(GetPointAt(start), GetPointAt(end));
}

//////////////////////////////////////////////////////////////////////
// VecIntersection::

//////////////////////////////////////////////////////////////////////
// mirror: swaps <t> and <u> - fields in intersection descriptor.

void VecIntersection::Mirror()
{
	Swap(t[0], u[0]);
	Swap(t[1], u[1]);
}

//////////////////////////////////////////////////////////////////////
// remove: removes n-th intersection.

void VecIntersection::Remove(int index)
{
	ASSERT(index >= 0 && index < count);
	if(--count > index && index == 0)
	{
		t[0] = t[1];
		u[0] = u[1];
	}
}

///// intersections

//////////////////////////////////////////////////////////////////////
// CheckBearing: checks whether circle intersection is an arc
// intersection as well.

void VecIntersection::CheckBearing(double *hints, const VecArcInfo &a)
{
	double temp;
	hints += count - 1;
	for(int k = count; --k >= 0; hints--)
		if(a.ContainsBearing(temp = *hints * TWOPI))
		{
			double gap = (TWOPI - (a.beta - a.alpha)) / 2;
			*hints =
				minmax<double>((modulo(temp - a.alpha + gap, TWOPI) - gap)
				/ (a.beta - a.alpha), 0, 1);
			if(a.reversed)
				*hints = 1 - *hints;
		}
		else
			Remove(k);
}

//////////////////////////////////////////////////////////////////////
// LL: line vs line.

bool VecIntersection::LL(const Pointf& P1, const Pointf& Q1, const Pointf& P2, const Pointf& Q2)
{
	if(min(P2.x, Q2.x) - max(P1.x, Q1.x) > Vec_tolerance
	|| min(P2.y, Q2.y) - max(P1.y, Q1.y) > Vec_tolerance
	|| min(P1.x, Q1.x) - max(P2.x, Q2.x) > Vec_tolerance
	|| min(P1.y, Q1.y) - max(P2.y, Q2.y) > Vec_tolerance)
		return Nothing();

	double l1  = P1 | Q1;
	double l2  = P2 | Q2;

	if(l1 <= Vec_tolerance || l2 <= Vec_tolerance)
		return Nothing();

	Pointf PQ1     = Q1 - P1;
	Pointf PQ2     = P2 - Q2;
	Pointf PP      = P2 - P1;
	double d   = PQ1 % PQ2;
	double dt1 = PP % PQ2;
	double dt2 = PQ1 % PP;

	if(d < 0)
	{
		d = -d;
		dt1 = -dt1;
		dt2 = -dt2;
	}

	l1 = Vec_tolerance * d / l1;
	l2 = Vec_tolerance * d / l2;

	if(d == 0 || dt1 < -l1 || dt1 > d + l1 || dt2 < -l2 || dt2 > d + l2)
		return Nothing();

	count = 1;
	t[0] = minmax<double>(dt1 / d, 0, 1);
	u[0] = minmax<double>(dt2 / d, 0, 1);
	return true;
}

//////////////////////////////////////////////////////////////////////
// LC: line vs circle.

bool VecIntersection::LC(const Pointf& P1, const Pointf& Q1, const Pointf& C2, double r2)
{
	Pointf PQ    = Q1 - P1;
	Pointf CP    = P1 - C2;
	double a = Squared(PQ);

	count = 0;
	if(fabs(r2) <= Vec_tolerance || fabs(PQ.x) <= Vec_tolerance && fabs(PQ.y) <= Vec_tolerance)
		return false;

	double mb = -(PQ ^ CP);
	double c = Squared(CP) - r2 * r2;
	double d = mb * mb - a * c;

	double tol2 = Vec_tolerance * Vec_tolerance;

	if(d > tol2)
	{ // two possible solutions
		d = sqrt(d);
		double r;
		r = mb + d;
		if(r >= -Vec_tolerance && r <= a + Vec_tolerance)
		{ // 1st solution is valid
			t[0] = r = minmax<double>(r / a, 0, 1);
			u[0] = Bearing(CP + PQ * r) / TWOPI;
//			Pointf temp = Pointf::Polar(C2, r2, u[0] * TWOPI);
//			LOG("P = " << P1 << ", Q = " << Q1 << ", C = " << C2 << ", r = " << r2
//				<< ": temp = " << temp);
			count = 1;
		}
		r = mb - d;
		if(r >= -Vec_tolerance && r <= a + Vec_tolerance)
		{ // 2nd solution is valid
			t[count] = r = minmax<double>(r / a, 0, 1);
			u[count] = Bearing(CP + PQ * r) / TWOPI;
			count++;
		}
	}
	else if(d >= -tol2)
	{ // one solution
		if(mb >= -Vec_tolerance && mb <= a + Vec_tolerance)
		{
			double r;
			t[0] = r = minmax<double>(mb / a, 0, 1);
			u[0] = Bearing(CP + PQ * r) / TWOPI;
			count = 1;
		}
	}
	return count != 0;
}

//////////////////////////////////////////////////////////////////////
// LA: line vs arc.

bool VecIntersection::LA(const Pointf& P1, const Pointf& Q1, const VecArcInfo &a2)
{
	if(!a2.IsCurved())
		return LL(P1, Q1, a2.A, a2.B);

	if(LC(P1, Q1, a2.C, a2.radius))
		CheckBearing(u, a2);
	return count != 0;
}

//////////////////////////////////////////////////////////////////////
// CC: circle vs circle.

bool VecIntersection::CC(const Pointf& C1, double r1, const Pointf& C2, double r2)
{
	if(fabs(r1) <= Vec_tolerance || fabs(r2) <= Vec_tolerance
		|| (C1 | C2) >= r1 + r2 + Vec_tolerance)
		return Nothing();
	Pointf CC  = C2 - C1;
	Pointf P   = C1 + CC * (0.5 + (r1 * r1 - r2 * r2) / (2 * Squared(CC)));
	Pointf CP1 = P - C1;
	Pointf CP2 = P - C2;
	double r;
	if(r1 >= r2)
		r = r1 * r1 - Squared(CP1);
	else
		r = r2 * r2 - Squared(CP2);
	double tol2 = Vec_tolerance * Vec_tolerance;

	if(r > tol2)
	{ // two solutions
		Pointf D = UnitV(Right(CC)) * sqrt(r);
		count = 2;
		t[0] = Bearing(CP1 + D) / TWOPI;
		u[0] = Bearing(CP2 + D) / TWOPI;
		t[1] = Bearing(CP1 - D) / TWOPI;
		u[1] = Bearing(CP2 - D) / TWOPI;
		return true;
	}
	else if(r >= -tol2)
	{ // one solution
		count = 1;
		t[0] = Bearing(CP1) / TWOPI;
		u[0] = Bearing(CP2) / TWOPI;
		return true;
	}
	else
		return Nothing();
}

//////////////////////////////////////////////////////////////////////
// CA: circle vs arc.

bool VecIntersection::CA(const Pointf& C1, double r1, const VecArcInfo &a2)
{
	if(!a2.IsCurved())
	{
		LC(a2.A, a2.B, C1, r1);
		Mirror();
		return count != 0;
	}

	if(CC(C1, r1, a2.C, a2.radius))
		CheckBearing(u, a2);
	return count != 0;
}

//////////////////////////////////////////////////////////////////////
// AA: arc vs arc.

bool VecIntersection::AA(const VecArcInfo &a1, const VecArcInfo &a2)
{
	if(!a1.IsCurved())
		if(!a2.IsCurved())
			return LL(a1.A, a1.B, a2.A, a2.B);
		else
			return LA(a1.A, a1.B, a2);
	else if(!a2.IsCurved())
	{
		LA(a2.A, a2.B, a1);
		Mirror();
		return count != 0;
	}

	if(CC(a1.C, a1.radius, a2.C, a2.radius))
	{
		CheckBearing(t, a1);
		CheckBearing(u, a2);
	}
	return count != 0;
}

//////////////////////////////////////////////////////////////////////

}
