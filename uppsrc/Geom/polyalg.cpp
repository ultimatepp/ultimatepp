#include "Geom.h"

namespace Upp {

#define POLY_TIMING(n) // RTIMING(n)
#define POLY_LOGGING   0 // 1 = on, 0 = off

/*
static bool ContainsPoints(const Point *poly, int count, Point P)
{
	if(count <= 2 || IsNull(P))
		return false;
	bool in = false;
	Point A = poly[count - 1];
	Size ap(cmp(A.x, P.x), cmp(A.y, P.y));
	for(Point B; --count >= 0; A = B)
	{
		B = *poly++;
		Size bp(cmp(B.x, P.x), cmp(B.y, P.y));
		if(ap.cx < 0 && bp.cx < 0 || ap.cy > 0 && bp.cy > 0 || ap.cy < 0 && bp.cy < 0)
			continue;
		if((ap.cx | ap.cy) == 0 || (bp.cx | bp.cy) == 0)
			return true;
		else if(ap.cy == 0 && bp.cy == 0)
		{
			if(ap.cx <= 0 || bp.cx <= 0)
				return true;
		}
		else if(ap.cy == 0)
		{
			if(ap.cx > 0 && bp.cy > 0)
				in = !in;
		}
		else if(bp.cy == 0)
		{
			if(bp.cx > 0 && ap.cy > 0)
				in = !in;
		}
		else if(ap.cx > 0 && bp.cx > 0)
		{
			in = !in;
		}
		else
		{
			int s = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
			if(ap.cy > 0)
				s = -s;
			if(s == 0)
				return true;
			if(s > 0)
				in = !in;
		}
	}
	return in;
}
*/
static inline int PolyCompare(Pointf P, Pointf A, Pointf B, Size ap, Size bp)
{
	if(ap.cx < 0 && bp.cx < 0 || ap.cy > 0 && bp.cy > 0 || ap.cy < 0 && bp.cy < 0)
		return -1;
	if((ap.cx | ap.cy) == 0 || (bp.cx | bp.cy) == 0)
		return 0;
	else if(ap.cy == 0 && bp.cy == 0)
		return (ap.cx <= 0 || bp.cx <= 0 ? 0 : -1);
	else if(ap.cy == 0)
		return (ap.cx > 0 && bp.cy > 0 ? 1 : -1);
	else if(bp.cy == 0)
		return (bp.cx > 0 && ap.cy > 0 ? 1 : -1);
	else if(ap.cx > 0 && bp.cx > 0)
		return 1;
	else
	{
		double s = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
		if(ap.cy > 0)
			s = -s;
		return cmp(s, 0.0);
	}
}

struct PolyPart : public Rectf
{
	PolyPart(Rectf rc, int first, int begin, int end)
		: Rectf(rc), first(first), begin(begin), end(end) {}
	int first, begin, end;
};

static Rectf SplitPoly(const Array<Pointf>& polygon, const Vector<int>& polyend, bool closed,
	Array<PolyPart>& parts)
{
	if(polyend.IsEmpty())
		return Null;
	unsigned c = polygon.GetCount();
	int n = 1;
	if(c >= 1 << 16) { c >>= 16; n <<= 8; }
	if(c >= 1 <<  8) { c >>=  8; n <<= 4; }
	if(c >= 1 <<  4) { c >>=  4; n <<= 2; }
	if(c >= 2) n <<= 1;
	Rectf sum = Null;
	int i = 0;
	for(int p = 0; p < polyend.GetCount(); p++)
	{
		int e = polyend[p], f = (closed ? e - 1 : i++);
		while(i < e)
		{
//			RTIMING("SplitPoly / part");
			int ib = i;
			int ie = min(i + n, e);
			const Pointf *A = &polygon[f];
			Rectf rc;
			rc.left = rc.right = A -> x;
			rc.top = rc.bottom = A -> y;
			while(i < ie)
			{
//				RTIMING("SplitPoly / vertex");
				A = &polygon[i++];
				double Ax = A -> x, Ay = A -> y;
				if     (Ax < rc.left)   rc.left   = Ax;
				else if(Ax > rc.right)  rc.right  = Ax;
				if     (Ay < rc.top)    rc.top    = Ay;
				else if(Ay > rc.bottom) rc.bottom = Ay;
			}
//			RTIMING("SplitPoly / new");
			parts.Add(new PolyPart(rc, f, ib, ie));
			sum |= rc;
			f = ie - 1;
		}
	}
	return sum;
}

int ContainsPoints(const Array<Pointf>& polygon, const Vector<int>& polyend, const Array<Pointf>& points)
{
	if(points.IsEmpty() || polygon.GetCount() <= 1)
		return CMP_OUT;
	Array<PolyPart> parts;
	Rectf prect = SplitPoly(polygon, polyend, true, parts);
	bool is_in = false, is_out = false;
	for(int p = 0; p < points.GetCount(); p++)
	{
		const Pointf& P = points[p];
		if(!prect.Contains(P))
		{
			if(is_in)
				return CMP_SECT;
			is_out = true;
			continue;
		}
		bool in = false;
		for(int pp = 0; pp < parts.GetCount(); pp++)
		{
			const PolyPart& part = parts[pp];
			if(part.right < P.x || part.top > P.y || part.bottom < P.y)
				continue;
			const Pointf *A = &polygon[part.first];
			Size ap(cmp(A -> x, P.x), cmp(A -> y, P.y));
			for(int j = part.begin; j < part.end; j++)
			{
				const Pointf *B = &polygon[j];
				Size bp(cmp(B -> x, P.x), cmp(B -> y, P.y));
				switch(PolyCompare(P, *A, *B, ap, bp))
				{
				case 0: return CMP_SECT;
				case 1: in = !in; break;
				}
				A = B;
				ap = bp;
			}
		}
		(in ? is_in : is_out) = true;
		if(is_in && is_out)
			return CMP_SECT;
	}
	return is_in ? CMP_IN : CMP_OUT;
}

int ContainsPoints(const Array<Pointf>& polygon, const Array<Pointf>& points)
{
	if(points.IsEmpty() || polygon.GetCount() <= 1)
		return CMP_OUT;
	Vector<int> polyend;
	polyend.SetCount(1);
	polyend[0] = polygon.GetCount();
	return ContainsPoints(polygon, polyend, points);
}

int ContainsPoint(const Array<Pointf>& polygon, const Vector<int>& polyend, Pointf pt)
{
	if(IsNull(pt) || polygon.GetCount() < 3 || polyend.IsEmpty())
		return CMP_OUT;
	Array<Pointf> plist;
	plist.SetCount(1, pt);
	return ContainsPoints(polygon, polyend, plist);
}

int ContainsPoint(const Array<Pointf>& polygon, Pointf pt)
{
	if(IsNull(pt) || polygon.GetCount() <= 1)
		return CMP_OUT;
	Array<Pointf> plist;
	plist.SetCount(1, pt);
	return ContainsPoints(polygon, plist);
}

int ContainsPoly(const Array<Pointf>& chkpoly,
	const Array<Pointf>& polygon, const Vector<int>& polyend, bool closed)
{
//	RTIMING("Contains(Poly / Poly)");
	if(chkpoly.IsEmpty() || polyend.IsEmpty())
		return CMP_OUT;
	if(polyend.GetCount() == polygon.GetCount())
		return ContainsPoints(chkpoly, polygon);
//	static TimingInspector prep("Contains(Poly / Poly) -> preprocess");
//	prep.Start();
	Array<PolyPart> pp, cp;
	Rectf pfull = SplitPoly(polygon, polyend, closed, pp);
	Vector<int> chkend;
	chkend.SetCount(1);
	chkend[0] = chkpoly.GetCount();
	Rectf cfull = SplitPoly(chkpoly, chkend, true, cp);
	int i, j;
	for(i = pp.GetCount(); --i >= 0;)
		if(!(pp[i] && cfull))
			pp.Remove(i);
//	prep.End();
	if(pp.IsEmpty())
		return CMP_OUT;
	for(i = 0; i < pp.GetCount(); i++)
	{
		const PolyPart& api = pp[i];
		if(!(api && cfull))
			continue;
		const Pointf *A0 = &polygon[api.first];
		for(j = 0; j < cp.GetCount(); j++)
		{
			const PolyPart& bpi = cp[j];
			if(api && bpi)
			{
				const Pointf *A = A0;
				const Pointf *C0 = &chkpoly[bpi.first];
				for(int ia = api.begin; ia < api.end; ia++)
				{
					const Pointf *B = &polygon[ia];
					Pointf AB = *B - *A;
					const Pointf *C = C0;
					for(int ib = bpi.begin; ib < bpi.end; ib++)
					{
//						RTIMING("Contains(Poly / Poly) -> cross");
						const Pointf *D = &chkpoly[ib];
						Pointf CD = *D - *C, AC = *C - *A, BD = *D - *B;
						int acd = sgn(AC % CD) + 1, bcd = sgn(BD % CD) + 1;
						int cab = sgn(AC % AB) + 1, dab = sgn(BD % AB) + 1;
						static const int clss[3] = { 0376104000, 0104104104, 0000104376 };
						if(clss[acd] & (1 << (9 * bcd + 3 * cab + dab)))
							return CMP_SECT;
						C = D;
					}
					A = B;
				}
			}
		}
	}
	if(!cfull.Contains(pfull))
		return CMP_OUT;
//	RTIMING("Contains(Poly / Poly) -> in / out");
	Pointf P = polygon[0];
	bool in = false;
	for(i = 0; i < cp.GetCount(); i++)
	{
		const PolyPart& api = cp[i];
		if(api.right < P.x || api.top > P.y || api.bottom < P.y)
			continue;
		const Pointf *A = &chkpoly[api.first];
		Size cap(cmp(A -> x, P.x), cmp(A -> y, P.y));
		for(j = api.begin; j < api.end; j++)
		{
			const Pointf *B = &chkpoly[j];
			Size cbp(cmp(B -> x, P.x), cmp(B -> y, P.y));
			switch(PolyCompare(P, *A, *B, cap, cbp))
			{
			case 0: return CMP_SECT;
			case 1: in = !in; break;
			}
			A = B;
			cap = cbp;
		}
	}
	return (in ? CMP_IN : CMP_OUT);
}

int ContainsPoly(const Array<Pointf>& chkpoly, const Array<Pointf>& polygon, bool closed)
{
	if(chkpoly.GetCount() <= 2 || polygon.IsEmpty())
		return CMP_OUT;
	if(polygon.GetCount() == 1)
		return ContainsPoint(chkpoly, polygon[0]);
	Vector<int> polyend;
	polyend.SetCount(1);
	polyend[0] = polygon.GetCount();
	return ContainsPoly(chkpoly, polygon, polyend, closed);
}

template <class T>
struct PIUtils
{
	typedef int             PointList;
	typedef int             Iterator;
	typedef int             ConstIterator;
};

template <>
struct PIUtils<int>
{
	typedef Vector<Point>   PointList;
	typedef Point          *Iterator;
	typedef const Point    *ConstIterator;

	static int              tscale(int x, int y, int z)  { return iscale(x, y, z); }
	static Size             tscale(Size x, int y, int z) { return iscale(x, y, z); }
	static Rect             EmptyRect(Point pt)          { return Rect(pt.x, pt.y, pt.x + 1, pt.y + 1); }
	static void             Copy(Iterator dst, ConstIterator src, ConstIterator lim)
	{
		while(src != lim)
			*dst = *src, ++dst, ++src;
	}
	static int              MAXVAL()                     { return INT_MAX; }
	static int              MulVector(Size a, Size b)    { return a.cx * b.cy - a.cy * b.cx; }
};

template <>
struct PIUtils<double>
{
	typedef Array<Pointf>   PointList;
	typedef Array<Pointf>::Iterator Iterator;
	typedef Array<Pointf>::ConstIterator ConstIterator;

	static double           tscale(double x, double y, double z) { return x * y / z; }
	static Sizef            tscale(Sizef x, double y, double z)  { return x * (y / z); }
	static Rectf            EmptyRect(Pointf pt)                 { return Rectf(pt.x, pt.y, pt.x, pt.y); }
	static void             Copy(Iterator dst, ConstIterator src, ConstIterator lim)
	{
		while(src != lim)
			*dst = *src, ++dst, ++src;
	}
	static double           MAXVAL()                    { return DBL_MAX; }
	static double           MulVector(Sizef a, Sizef b) { return a.cx * b.cy - a.cy * b.cx; }
};

template <class T>
struct PolygonIterator
{
	PolygonIterator(Rect_<T> clip, int max_trace_points);

	void                    Add(typename PIUtils<T>::ConstIterator vertices, int vertex_count, const int *counts, int count_count);
	void                    Run();

	static bool             SegmentLess(const typename PIUtils<T>::PointList& a, const typename PIUtils<T>::PointList& b)
	{
		Point_<T> a0 = a[0], b0 = b[0];
		if(a0.y != b0.y) return a0.y < b0.y;
		if(a0.x != b0.x) return a0.x < b0.x;
		return a[1].x < b[1].x; // this is guess but perhaps usually true
	}

	struct Trace
	{
		typename PIUtils<T>::PointList     done;
		typename PIUtils<T>::PointList     avail;
		typename PIUtils<T>::ConstIterator next;
		typename PIUtils<T>::ConstIterator stop;
		Point_<T>                 ypos;
		Point_<T>                 end;
		Size_<T>                  delta;

		T                         GetX(T y) const
		{ return delta.cy ? end.x - PIUtils<T>::tscale(end.y - y, delta.cx, delta.cy) : end.x; }

		void                      Add(Point_<T> p)
		{
			while(done.GetCount() >= 2
			&& PIUtils<T>::MulVector(p - done.Top(), p - done[done.GetCount() - 2]) == 0)
				done.Drop();
			done.Add(p);
		}
	};

	void                    Step(T ymax);
	void                    Flush(int index, T ypos);
	void                    CheckTraces();

	void                    DumpTraces(T y) const;
	void                    DumpSegments() const;
	void                    DumpDone(int i) const;

	Rect_<T>                clip;
	T                       ymin, ymax;
	Rect_<T>                src_clip;
	int                     max_trace_points;
	Array<Trace>            traces;
	Vector< typename PIUtils<T>::PointList > segments;
	typename PIUtils<T>::PointList out_vertices;
	Vector<int>             out_counts;
};

void SplitPolygon(const Point *vertices, int vertex_count, const int *counts, int count_count,
	Vector<Point>& out_vertices, Vector<int>& out_counts, Rect clip, int max_trace_points)
{
	POLY_TIMING("SplitPolygon");
	PolygonIterator<int> pi(clip, max_trace_points);
	pi.Add(vertices, vertex_count, counts, count_count);
	pi.Run();
	out_vertices = pick(pi.out_vertices);
	out_counts = pick(pi.out_counts);
}

void SplitPolygon(const Vector<Point>& vertices, const Vector<int>& counts,
	Vector<Point>& out_vertices, Vector<int>& out_counts, Rect clip, int max_trace_points)
{
	SplitPolygon(vertices.Begin(), vertices.GetCount(), counts.Begin(), counts.GetCount(),
		out_vertices, out_counts, clip, max_trace_points);
}

void SplitPolygon(Array<Pointf>::ConstIterator vertices, int vertex_count, const int *counts, int count_count,
	Array<Pointf>& out_vertices, Vector<int>& out_counts, const Rectf& clip, int max_trace_points)
{
	POLY_TIMING("SplitPolygon");
	PolygonIterator<double> pi(clip, max_trace_points);
	pi.Add(vertices, vertex_count, counts, count_count);
	pi.Run();
	out_vertices = pick(pi.out_vertices);
	out_counts = pick(pi.out_counts);
}

void SplitPolygon(const Array<Pointf>& vertices, const Vector<int>& counts,
	Array<Pointf>& out_vertices, Vector<int>& out_counts, const Rectf& clip, int max_trace_points)
{
	SplitPolygon(vertices.Begin(), vertices.GetCount(), counts.Begin(), counts.GetCount(),
		out_vertices, out_counts, clip, max_trace_points);
}

template <class T>
PolygonIterator<T>::PolygonIterator(Rect_<T> clip, int max_trace_points)
: clip(clip), max_trace_points(max_trace_points)
{
	ymax = INT_MIN;
}

template <class T>
void PolygonIterator<T>::Add(typename PIUtils<T>::ConstIterator vertices, int vertex_count, const int *counts, int count_count)
{
	POLY_TIMING("PolygonIterator::Add");
	if(vertex_count <= 2)
		return;
	T cmin = IsNull(clip) ? -PIUtils<T>::MAXVAL() : clip.top;
	T cmax = IsNull(clip) ? +PIUtils<T>::MAXVAL() : clip.bottom;
	src_clip = PIUtils<T>::EmptyRect(vertices[0]);
	for(; --count_count >= 0; vertices += *counts++)
	{
		typename PIUtils<T>::ConstIterator p = vertices, e = vertices + *counts, l = e - 1;
		T ytop = p -> y;
		typename PIUtils<T>::ConstIterator q = p;
		for(; p != e; ++p)
		{
			if(p -> y > ytop)
				ytop = (q = p) -> y;
#if POLY_LOGGING
			src_clip |= PIUtils<T>::EmptyRect(*p);
#endif
		}
		ymax = max(ymax, ytop);
		p = q;
		do
		{
			typename PIUtils<T>::ConstIterator ub = p;
			ASSERT(ub < e);
			if(++p > q)
			{
				while(p != e && p[0].y <= p[-1].y)
					p++;
				if(p == e && q != vertices && vertices[0].y <= p[-1].y)
					p = vertices + 1;
			}
			while(p < q && p[0].y <= p[-1].y)
				p++;
			if(p == q && p[-1].y == ub -> y)
				break;
			typename PIUtils<T>::ConstIterator ue = --p;
			bool degen1 = (ub -> y == ue -> y || ub -> y <= cmin || ue -> y >= cmax);
			if(!degen1 && ub > ue)
			{
				while(ue > vertices && (ue[-1].y == ue[0].y || ue[-1].y <= cmin))
					ue--;
				while(ub < l && (ub[1].y == ub[0].y || ub[1].y >= cmax))
					ub++;
				if(ub == l && ue > vertices && (l -> y == vertices -> y || vertices -> y >= cmax))
					ub = vertices;
				else if(ue == vertices && ub < l && (l -> y == ue -> y || l -> y <= cmin))
					ue = l;
			}
			if(!degen1)
			{
				while(ub < ue && (ub[1].y == ub[0].y || ub[1].y >= cmax))
					ub++;
				while(ue > ub && (ue[0].y == ue[-1].y || ue[-1].y <= cmin))
					ue--;
			}
			typename PIUtils<T>::PointList& seg1 = segments.Add();
			seg1.SetCount(degen1 ? 2 : (int)(ue - ub) + 1 + (ue > ub ? 0 : *counts));
			ASSERT(seg1.GetCount() >= 2);
			ASSERT(ub != e);
			typename PIUtils<T>::Iterator sp = seg1.End();
			if(degen1)
			{
				sp -= 2;
				sp[0] = sp[1] = (ub -> y <= cmin ? Point_<T>(ub -> x, cmin)
				: ue -> y >= cmax ? Point_<T>(ue -> x, cmax) : *ub);
			}
			else
			{
				if(ub -> y > cmax)
				{
					Size_<T> delta = (ub == l ? vertices[0] : ub[1]) - *ub;
					*--sp = Point_<T>(ub -> x + PIUtils<T>::tscale(cmax - ub -> y, delta.cx, delta.cy), cmax);
					if(++ub == e)
						ub = vertices;
				}
				else
					*--sp = *ub++;
				if(ub > ue)
				{
					for(; ub < e; *--sp = *ub++)
						;
					ub = vertices;
				}
				while(ub < ue)
					*--sp = *ub++;
				if(ue -> y < cmin)
				{
					Size delta = (ue == vertices ? *l : ue[-1]) - *ue;
					*--sp = Point_<T>(ue -> x + PIUtils<T>::tscale(cmin - ue -> y, delta.cx, delta.cy), cmin);
				}
				else
					*--sp = *ub++;
			}
			ASSERT(sp == seg1.Begin());
			ub = p;
			if(p >= q)
			{
				while(p < l && p[1].y >= p[0].y)
					p++;
				if(p == l && vertices -> y >= p -> y)
					p = vertices;
			}
			while(p < q && p[1].y >= p[0].y)
				p++;
			ue = p;
			bool degen2 = (ub -> y == ue -> y || ub -> y >= cmax || ue -> y <= cmin);
			if(!degen2 && ub > ue)
			{
				while(ue > vertices && (ue[-1].y == ue[0].y || ue[-1].y >= cmax))
					ue--;
				while(ub < l && (ub[1].y == ub[0].y || ub[1].y <= cmin))
					ub++;
				if(ub == l && ue > vertices && (l -> y == vertices -> y || vertices -> y <= cmin))
					ub = vertices;
				else if(ue == vertices && ub < l && (l -> y == ue -> y || l -> y >= cmax))
					ue = l;
			}
			if(!degen2)
			{
				while(ub < ue && (ub[1].y == ub[0].y || ub[1].y <= cmin))
					ub++;
				while(ue > ub && (ue[0].y == ue[-1].y || ue[-1].y >= cmax))
					ue--;
			}
			typename PIUtils<T>::PointList& seg2 = segments.Add();
			seg2.SetCount(degen2 ? 2 : (int)(ue - ub) + 1 + (ue > ub ? 0 : *counts));
			ASSERT(seg2.GetCount() >= 2);
			sp = seg2.Begin();
			if(degen2)
				sp[0] = sp[1] = (ub -> y >= cmax ? Point_<T>(ub -> x, cmax) : ue -> y <= cmin ? Point_<T>(ue -> x, cmin) : *ub);
			else
			{
				if(ub -> y < cmin)
				{
					Size_<T> delta = (ub == l ? vertices[0] : ub[1]) - *ub;
					*sp++ = Point_<T>(ub -> x + PIUtils<T>::tscale(cmin - ub -> y, delta.cx, delta.cy), cmin);
				}
				else
					*sp++ = *ub;
				if(++ub == e)
					ub = vertices;

				if(ue -> y > cmax)
				{
					Size_<T> delta = (ue == vertices ? *l : ue[-1]) - *ue;
					seg2.Top() = Point_<T>(ue -> x + PIUtils<T>::tscale(cmax - ue -> y, delta.cx, delta.cy), cmax);
				}
				else
					seg2.Top() = *ue;

				if(ub > ue)
				{
					PIUtils<T>::Copy(sp, ub, e);
					sp += e - ub;
					PIUtils<T>::Copy(sp, vertices, ue);
					sp += ue - vertices;
				}
				else
				{
					PIUtils<T>::Copy(sp, ub, ue);
					sp += ue - ub;
				}
				ASSERT(sp == seg2.End() - 1);
			}
			if(degen1 && degen2)
				segments.Remove(segments.GetCount() - 2, 2);
		}
		while(p != q);
		ASSERT(!(segments.GetCount() & 1));
	}
	ymax = min(ymax, cmax);

#if POLY_LOGGING
	{ // check segment pair integrity
		VectorMap<T, int> counts;
		int i;
		for(i = 0; i < segments.GetCount(); i++)
			counts.GetAdd(segments[i].Top().y, 0)++;
		for(i = 0; i < counts.GetCount(); i++)
			if(counts[i] & 1)
				RLOG("odd bottom position " << counts.GetKey(i));
	}
#endif//POLY_LOGGING
}

template <class T>
void PolygonIterator<T>::DumpSegments() const
{
	for(int i = 0; i < segments.GetCount(); i++)
	{
		String seglog;
		seglog << "[" << i << "]: " << segments[i][0];
		for(int j = 1; j < segments[i].GetCount(); j++)
			seglog << " -> " << segments[i][j];
		RLOG(seglog);
	}
}

template <class T>
void PolygonIterator<T>::Run()
{
	POLY_TIMING("PolygonIterator::Run");
	if(segments.GetCount() < 2)
		return;
	Sort(segments, &PolygonIterator::SegmentLess);
#if POLY_LOGGING
	RLOG("PolygonIterator::Run");
	DumpSegments();
#endif//POLY_LOGGING

	ymin = segments[0][0].y;
	for(int s = 0; s < segments.GetCount();)
	{
		T y = segments[s][0].y;
		Step(y);
		int t = 0;
		for(; s < segments.GetCount() && segments[s][0].y == y; s += 2)
		{
			ASSERT(segments[s + 1][0].y == y);
			T x1 = segments[s + 0][0].x;
			while(t < traces.GetCount() && traces[t].ypos.x < x1)
				t++;
			int i1 = t;
			T x2 = segments[s + 1][0].x;
			while(t < traces.GetCount() && traces[t].ypos.x < x2)
				t++;
			int i2 = t;
#if POLY_LOGGING
			RLOG("y = " << y << ", insert[" << i1 << " - " << i2 << "]: segment #" << s);
#endif//POLY_LOGGING
			for(int flush = (i1 + 1) | 1; flush <= i2; flush += 2)
				Flush(flush, y);
			Trace& n = traces.Insert(i1);
			i2++;
			n.avail = pick(segments[s]);
			n.next = n.avail.Begin();
			n.stop = n.avail.End() - 1;
			n.end = *++n.next;
			n.delta = n.end - n.next[-1];
			if(i1 & 1)
			{ // hole opening
				Trace& r = traces[i1 + 1];
				n.done = pick(r.done);
#if POLY_LOGGING
				RLOG("insert: r.ypos = " << r.ypos);
				if(r.ypos.y < n.done.Top().y)
					RLOG("join-insert (1) y reversal: " << n.done.Top() << " -> " << r.ypos);
#endif//POLY_LOGGING
				n.Add(r.ypos);
				if(r.ypos.x != x1)
				{
#if POLY_LOGGING
					if(r.ypos.y < n.done.Top().y)
						RLOG("join-insert (2) y reversal: " << n.done.Top() << " -> " << r.ypos);
#endif//POLY_LOGGING
					n.Add(n.next[-1]);
				}
				r.done.Clear();
				r.Add(r.ypos);
#if POLY_LOGGING
				DumpDone(i1 + 1);
#endif//POLY_LOGGING
			}
			else // simply insert segment
				n.Add(n.next[-1]);
			Trace& o = traces.Insert(i2);
			o.avail = pick(segments[s + 1]);
			o.next = o.avail.Begin();
			o.stop = o.avail.End() - 1;
			o.Add(*o.next);
			o.end = *++o.next;
			o.delta = o.end - o.next[-1];
#if POLY_LOGGING
			RLOG("insert: o.avail = " << o.avail.GetCount() << ", end = " << o.end);
			DumpDone(i2);
#endif//POLY_LOGGING
			t += 2;
		}
	}
#if POLY_LOGGING
	CheckTraces();
#endif//POLY_LOGGING
	Step(ymax + 1);
#if POLY_LOGGING
	LOG("//PolygonIterator::Run");
#endif//POLY_LOGING
}

template <class T>
void PolygonIterator<T>::Step(T ymax)
{
	POLY_TIMING("PolygonIterator::Step");
	while(ymin < ymax && !traces.IsEmpty())
	{
		Trace *p, *q;
		for(;;)
		{ // check for trace swaps
			p = &traces[0];
			p -> ypos.y = min(p -> end.y, ymax);
			p -> ypos.x = p -> GetX(p -> ypos.y);
			int swap_index = -1;
			Point_<T> swap;
			int t;
			for(t = 1; t < traces.GetCount(); t++)
			{
				q = &traces[t];
				if(q -> end.y >= p -> ypos.y)
					q -> ypos.x = q -> GetX(q -> ypos.y = p -> ypos.y);
				else
				{
					q -> ypos = q -> end;
					p -> ypos.x = p -> GetX(p -> ypos.y = q -> end.y);
					swap_index = -1;
				}
				if(q -> ypos.x <= p -> ypos.x)
				{
					T det = PIUtils<T>::MulVector(p -> delta, q -> delta);
					if(det > 0)
					{
						T num = PIUtils<T>::MulVector(p -> end - q -> end, q -> delta);
						Point_<T> new_swap = p -> end - PIUtils<T>::tscale(p -> delta, minmax<T>(num, 0, det), det);
						if(new_swap.y <= q -> ypos.y)
						{
							swap_index = t;
							swap = new_swap;
							q -> ypos = new_swap;
						}
					}
				}
				p = q;
			}
			if(swap_index < 0)
				break;
			p = &traces[swap_index - 1];
			q = &traces[swap_index - 0];
#if POLY_LOGGING
			RLOG("ymin = " << ymin << ", swap[" << swap_index << "]: " << swap << ": l = "
				<< (p -> end - p -> delta) << " -> " << p -> end << " (" << p -> delta << "), "
				"r = " << (q -> end - q -> delta) << " -> " << q -> end << " (" << q -> delta << ")");
			DumpDone(swap_index - 1);
			if(swap.y < p -> done.Top().y)
				RLOG("swap y reversal in p: " << p -> done.Top() << " -> " << swap);
			DumpDone(swap_index - 0);
			if(swap.y < q -> done.Top().y)
				RLOG("swap y reversal in q: " << q -> done.Top() << " -> " << swap);
#endif//POLY_LOGGING
			p -> Add(swap);
			q -> Add(swap);
			Swap(p -> done, q -> done);
			traces.Swap(swap_index, swap_index - 1);
		}

		ymin = p -> ypos.y;

#if POLY_LOGGING
		RLOG("after swap: ymin = " << ymin);
		DumpTraces(ymin);
#endif//POLY_LOGGING

		if(p -> ypos.y >= ymax)
			return;

		ASSERT(!(traces.GetCount() & 1));
		Buffer<bool> remove(traces.GetCount());
		Fill<bool>(remove, remove + traces.GetCount(), false);
		int nremove = 0;
		bool hseg = false;
		for(int t = 0; t < traces.GetCount(); t++)
		{
			Trace& p = traces[t];

			while(p.end.y <= ymin)
			{
#if POLY_LOGGING
				if(p.end.y < p.done.Top().y)
					RLOG("remove y reversal[" << t << "]: " << p.done.Top() << " -> " << p.end);
#endif//POLY_LOGGING
				p.Add(p.end);
				if(p.next < p.stop)
				{
					p.delta = (p.end = p.next[1]) - p.next[0];
					if(p.delta.cy == 0)
						hseg = true;
					p.next++;
				}
				else
				{
					nremove++;
					remove[t] = true;
					break;
				}
			}
		}
		if(nremove)
		{
			ASSERT(!(nremove & 1));
			int t = traces.GetCount();
			if(t == nremove)
			{
				for(int i = 1; i < traces.GetCount(); i += 2)
					Flush(i, ymin);
				traces.Clear();
				return;
			}
			while(--t >= 0)
			{
				while(t >= 0 && !remove[t])
					t--;
				if(t < 0)
					break;
				int i2 = t;
				ASSERT(i2 >= 1);
				while(!remove[--t])
					;
				int i1 = t;
				ASSERT(i1 >= 0);
#if POLY_LOGGING
				LOG("ymin = " << ymin << ", remove[" << i1 << " - " << i2 << "] & " << ymin << ": l = "
				<< (traces[i1].end - traces[i1].delta) << " -> " << traces[i1].end << " (" << traces[i1].delta << "), "
				"r = " << (traces[i2].end - traces[i2].delta) << " -> " << traces[i2].end << " (" << traces[i2].delta << ")");
#endif//POLY_LOGGING
				for(int flush = i2 | 1; flush >= i1; flush -= 2)
					Flush(flush, ymin);
				if(i2 == i1 + 1)
					traces.Remove(i1, 2);
				else
				{
					traces.Remove(i2);
					traces.Remove(i1);
				}
			}
		}
#if POLY_LOGGING
		RLOG("After removals:");
		DumpTraces(ymin);
#endif//POLY_LOGGING
		if(hseg)
		{ // horizontal segments present, check for horizontal swaps
			ASSERT(!traces.IsEmpty());
			Trace *p = &traces[0];
			p -> ypos.x = p -> GetX(p -> ypos.y = ymin);
			for(int t = 1; t < traces.GetCount(); t++)
			{
				Trace *q = &traces[t];
				if((q -> ypos.x = q -> GetX(q -> ypos.y = ymin)) >= p -> ypos.x)
					p = q;
				else
				{
					int u = t - 1;
					do
					{
						Trace *r = &traces[u];
						q -> Add(q -> ypos);
						q -> Add(r -> ypos);
						r -> Add(r -> ypos);
						r -> Add(q -> ypos);
						Swap(q -> done, r -> done);
						traces.Swap(u, u + 1);
					}
					while(--u >= 0 && q -> ypos.x < traces[u].ypos.x);
				}
			}
#if POLY_LOGGING
			RLOG("After horizontal swaps:");
			DumpTraces(ymin);
#endif//POLY_LOGGING
		}
	}
}

template <class T>
void PolygonIterator<T>::Flush(int index, T ypos)
{
	POLY_TIMING("PolygonIterator::Flush");
	ASSERT(index & 1);
	Trace& p = traces[index - 1];
	Trace& q = traces[index - 0];
	p.ypos.x = p.GetX(p.ypos.y = ypos);
	q.ypos.x = q.GetX(q.ypos.y = ypos);
	int count = out_vertices.GetCount();
#if POLY_LOGGING
	RLOG("flush[" << index << "] at pos " << ypos << ": left done = " << p.done.GetCount()
		<< ", right done = " << q.done.GetCount());
	String plist, qlist;
	int c;
	for(c = 1; c < p.done.GetCount(); c++)
	{
		plist << ' ' << p.done[c];
		if(p.done[c].y < p.done[c - 1].y)
		{
			RLOG("y reversal in p & " << c << ": " << p.done[c - 1] << " -> " << p.done[c]);
		}
	}
	RLOG("plist = " << p.done[0] << plist);
	for(c = 1; c < q.done.GetCount(); c++)
	{
		qlist << ' ' << q.done[c];
		if(q.done[c].y < q.done[c - 1].y)
		{
			RLOG("y reversal in q & " << c << ": " << q.done[c - 1] << " -> " << q.done[c]);
		}
	}
	RLOG("qlist = " << q.done[0] << qlist);
#endif
	out_vertices.Append(p.done);
	if(out_vertices.Top() != p.ypos)
		out_vertices.Add(p.ypos);
	if(q.ypos != p.ypos)
		out_vertices.Add(q.ypos);
	typename PIUtils<T>::ConstIterator qe = q.done.End();
	if(*--qe != q.ypos)
		out_vertices.Add(*qe);
	typename PIUtils<T>::ConstIterator qb = q.done.Begin();
	if(*qb == p.done[0] && qb < qe)
		qb++;
	out_vertices.SetCountR(out_vertices.GetCount() + (int)(qe - qb));
	typename PIUtils<T>::Iterator out = out_vertices.End();
	while(qb < qe)
		*--out = *qb++;
	if(out_vertices.GetCount() <= count + 2)
		out_vertices.SetCountR(count); // trash degenerate segment
	else
	{
#if POLY_LOGGING
		String seg;
		Rect_<T> extent = PIUtils<T>::EmptyRect(out_vertices[count]);
		for(int i = count; i < out_vertices.GetCount(); i++)
		{
			seg << ' ' << out_vertices[i];
			extent |= PIUtils<T>::EmptyRect(out_vertices[i]);
		}
		RLOG("extent = " << extent << ":" << seg);
		if(!src_clip.Contains(extent))
			RLOG("-> out of source clip " << src_clip);
#endif
		out_counts.Add(out_vertices.GetCount() - count);
	}
	p.done.Clear();
	p.done.Add(p.ypos);
	q.done.Clear();
	q.done.Add(q.ypos);
#if POLY_LOGGING
	DumpDone(index - 1);
	DumpDone(index - 0);
#endif//POLY_LOGGING
}

template <class T>
void PolygonIterator<T>::CheckTraces()
{
	VectorMap<T, int> counts;
	int i;
	for(i = 0; i < traces.GetCount(); i++)
		counts.GetAdd(traces[i].stop -> y, 0)++;
	for(i = 0; i < counts.GetCount(); i++)
		if(counts[i] & 1)
		{
			RLOG("unpaired end of trace #" << i << ": " << traces[i].stop -> y);
			NEVER();
		}
}

template <class T>
void PolygonIterator<T>::DumpTraces(T y) const
{
	String out;
	out << traces.GetCount() << " traces & " << y << ":";
	for(int t = 0; t < traces.GetCount(); t++)
		out << ' ' << traces[t].GetX(y);
	RLOG(out);
}

template <class T>
void PolygonIterator<T>::DumpDone(int i) const
{
	String out;
	out << "done[" << i << "]:";
	for(int t = 0; t < traces[i].done.GetCount(); t++)
		out << ' ' << traces[i].done[t];
	RLOG(out);
}

}
