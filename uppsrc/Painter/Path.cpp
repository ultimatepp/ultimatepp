#include "Painter.h"

#define LLOG(x) // DLOG(x)

NAMESPACE_UPP

void BufferPainter::ClearPath()
{
	path.type.Clear();
	path.data.Clear();
	current = move = Null;
	ccontrol = qcontrol = Pointf(0, 0);
	ischar = false;
	path_min = Pointf(DBL_MAX, DBL_MAX);
	path_max = -path_min;
}

Pointf BufferPainter::PathPoint(const Pointf& p, bool rel)
{
	Pointf r;
	r.x = IsNull(p.x) ? current.x : rel ? p.x + current.x : p.x;
	r.y = IsNull(p.y) ? current.y : rel ? p.y + current.y : p.y;
	if(IsNull(current)) {
		ClearPath();
		pathattr = attr;
	}
	if(dopreclip) {
		path_min = min(r, path_min);
		path_max = max(r, path_max);
	}
	return r;
}

Pointf BufferPainter::EndPoint(const Pointf& p, bool rel)
{
	return current = PathPoint(p, rel);
}

void *BufferPainter::PathAddRaw(int type, int size)
{
	int q = path.data.GetCount();
	path.type.Add(type);
	path.data.SetCount(q + size);
	return &path.data[q];
}

void BufferPainter::MoveOp(const Pointf& p, bool rel)
{
	LLOG("@ MoveOp " << p << ", " << rel);
	move = ccontrol = qcontrol = EndPoint(p, rel);
	PathAdd<LinearData>(MOVE).p = move;
}

void BufferPainter::DoMove0()
{
	if(IsNull(move))
		MoveOp(Pointf(0, 0), false);
}

void BufferPainter::LineOp(const Pointf& p, bool rel)
{
	LLOG("@ LineOp " << p << ", " << rel);
	DoMove0();
	PathAdd<LinearData>(LINE).p = ccontrol = qcontrol = EndPoint(p, rel);
}

void BufferPainter::QuadraticOp(const Pointf& p1, const Pointf& p, bool rel)
{
	LLOG("@ QuadraticOp " << p1 << ", " << p << ", " << rel);
	DoMove0();
	QuadraticData& m = PathAdd<QuadraticData>(QUADRATIC);
	qcontrol = m.p1 = PathPoint(p1, rel);
	m.p = EndPoint(p, rel);
}

void BufferPainter::QuadraticOp(const Pointf& p, bool rel)
{
	QuadraticOp(2.0 * current - qcontrol, p, rel);
}

void BufferPainter::CubicOp(const Pointf& p1, const Pointf& p2, const Pointf& p, bool rel)
{
	LLOG("@ CubicOp " << p1 << ", " << p1 << ", " << p << ", " << rel);
	DoMove0();
	CubicData& m = PathAdd<CubicData>(CUBIC);
	m.p1 = PathPoint(p1, rel);
	ccontrol = m.p2 = PathPoint(p2, rel);
	m.p = EndPoint(p, rel);
}

void BufferPainter::CubicOp(const Pointf& p2, const Pointf& p, bool rel)
{
	CubicOp(2.0 * current - ccontrol, p2, p, rel);
}

void BufferPainter::ArcOp(const Pointf& c, const Pointf& r, double angle, double sweep, bool rel)
{
	LLOG("@ ArcOp " << c << ", " << r << ", " << angle << ", " << sweep << ", " << rel);
	DoMove0();
	DoArc(PathPoint(c, rel), r, angle, sweep, 0);
}

void BufferPainter::SvgArcOp(const Pointf& r, double xangle, bool large, bool sweep,
                             const Pointf& p, bool rel)
{
	LLOG("@ SvgArcOp " << r << ", " << xangle << ", " << large << ", " << sweep << ", " << p << ", " << rel);
	DoMove0();
	Pointf c = current;
	DoSvgArc(r, xangle, large, sweep, EndPoint(p, rel), c);
}

void BufferPainter::CloseOp()
{
	LLOG("@ CloseOp");
	if(!IsNull(move) && !IsNull(current) && current != move) {
		Line(move);
		move = Null;
	}
}

void BufferPainter::DivOp()
{
	LLOG("@ DivOp");
	CloseOp();
	path.type.Add(DIV);
}

void BufferPainter::CharacterOp(const Pointf& p, int ch, Font fnt)
{
	LLOG("@ CharacterOp " << p << ", " << ch << ", " << fnt);
#if 0
	DoMove0();
	PaintCharacter(*this, p, ch, fnt);
#else
	move = current = EndPoint(p, false);
	CharData& m = PathAdd<CharData>(CHAR);
	m.p = EndPoint(p, false);
	m.ch = ch;
	m.fnt = fnt;
	ischar = true;
	EvenOdd();
#endif
}

END_UPP_NAMESPACE
