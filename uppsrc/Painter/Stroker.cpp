#include "Painter.h"

#define LLOG(x)   // DLOG(x)

namespace Upp {

void Stroker::Init(double width, double miterlimit, double tolerance, int _linecap, int _linejoin, const Rectf& preclip_)
{
	LLOG("Init");
	linecap = _linecap;
	linejoin = _linejoin;
	preclip = preclip_;
	w2 = width / 2;
	qmiter = miterlimit * w2;
	if(!IsNull(preclip))
		tw = 4 * max(qmiter, width); // preclipping width
	qmiter *= qmiter;
	fid = acos(1 - tolerance / w2);
	p0 = p1 = p2 = Null;
	lines = 0;
}

void Stroker::Move(const Pointf& p)
{
	LLOG("Stroker::Move " << p);
	Finish();
	p1 = p;
	p0 = p2 = Null;
}

void Stroker::Round(const Pointf& p, const Pointf& v1, const Pointf& v2, double r)
{
	double a1 = Bearing(v1);
	double a2 = Bearing(v2);
	if(a1 < a2)
		a1 += 2 * M_PI;
	while(a1 > a2) {
		PutLine(Polar(p, r, a1));
		a1 -= fid;
	}
}

inline bool Stroker::PreClipped(Pointf p2, Pointf p3)
{
	return p2.x + tw < preclip.left && p3.x + tw < preclip.left ||
	       p2.x - tw > preclip.right && p3.x - tw > preclip.right ||
           p2.y + tw < preclip.top && p3.y + tw < preclip.top ||
	       p2.y - tw > preclip.bottom && p3.y - tw > preclip.bottom;
}

void Stroker::Line(const Pointf& p3)
{
	LLOG("Stroker::Line " << p3);
	lines++;
	if(IsNull(p1)) {
		Move(p3);
		return;
	}
	if(IsNull(p2)) {
		Pointf v = p3 - p1;
		double l = Length(v);
		if(l < 1e-8) // lower precision to accomodate Arc precision issue
			return;
		p2 = p3;
		v1 = v;
		o1 = Orthogonal(v1) * w2 / l;
		a1 = p1 + o1;
		b1 = p1 - o1;
		if(IsNull(p0)) {
			p0 = p1;
			v0 = v1;
			o0 = o1;
			a0 = a1;
			b0 = b1;
		}
		return;
	}

	Pointf v2 = p3 - p2;
	double l = Length(v2);
	if(l < 1e-15)
		return;
	Pointf o2 = Orthogonal(v2) * w2 / l;
	Pointf a2 = p2 + o2;
	Pointf b2 = p2 - o2;

	double d = v1.y * v2.x - v2.y * v1.x;
	if(d > 1e-15) {
		Pointf ts = a1 + v1 * (v2.y * (a1.x - a2.x) - v2.x * (a1.y - a2.y)) / d;
		PutMove(a1);
		if(linejoin != LINEJOIN_MITER || SquaredDistance(ts, p2) > qmiter) {
			PutLine(a1 + v1);
			if(linejoin == LINEJOIN_ROUND)
				Round(p2, o1, o2, w2);
		}
		else
			PutLine(ts);
		PutLine(a2);
		PutMove(b2);
		PutLine(p2);
		PutLine(b1 + v1);
		PutLine(b1);
	}
	else
	if(d < -1e-15) {
		Pointf ts = b2 + v2 * (v1.y * (a2.x - a1.x) - v1.x * (a2.y - a1.y)) / d;
		PutMove(b2);
		if(linejoin != LINEJOIN_MITER || SquaredDistance(ts, p2) > qmiter) {
			if(linejoin == LINEJOIN_ROUND)
				Round(p2, -o2, -o1, w2);
			PutLine(b1 + v1);
		}
		else
			PutLine(ts);
		PutLine(b1);
		PutMove(a1);
		PutLine(a1 + v1);
		PutLine(p2);
		PutLine(a2);
	}
	else {
		PutMove(a1);
		PutLine(a1 + v1);
		if(linejoin == LINEJOIN_ROUND)
			Round(p2, o1, o2, w2);
		PutLine(a2);
		PutMove(b2);
		PutLine(b1 + v1);
		PutLine(b1);
	}

	p1 = p2;
	v1 = v2;
	o1 = o2;
	a1 = a2;
	b1 = b2;
	p2 = p3;
}

void Stroker::Cap(const Pointf& p, const Pointf& v, const Pointf& o,
                  const Pointf& a, const Pointf& b)
{
	PutMove(a);
	if(linecap == LINECAP_SQUARE) {
		Pointf nv = Orthogonal(o);
		PutLine(a + nv);
		PutLine(b + nv);
	}
	if(linecap == LINECAP_ROUND)
		Round(p, -o, o, w2);
	PutLine(b);
}

void Stroker::Finish()
{
	if(IsNull(p1) || IsNull(p2) || IsNull(p0))
		return;
	LLOG("-- Finish " << p1 << " " << p2 << ", lines " << lines);
	if(lines == 1 && !IsNull(preclip) && PreClipped(p1, p2)) { // this is mostly intended to preclip dasher segments
		LLOG("FINISH PRECLIPPED " << p1 << " - " << p2);
		lines = 0;
		return;
	}
	if(p2 == p0)
		Line(p0 + v0);
	else {
		PutMove(a1);
		PutLine(a1 + v1);
		PutMove(b1 + v1);
		PutLine(b1);
		Cap(p0, v0, o0, b0, a0);
		Cap(p2, -v1, -o1, a1 + v1, b1 + v1);
	}
	p0 = p1 = p2 = Null;
	lines = 0;
	LLOG("* done");
}

void Stroker::End()
{
	Finish();
	PutEnd();
}

}
