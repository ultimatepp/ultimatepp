#include "Painter.h"

NAMESPACE_UPP

void BufferPainter::ClearPath()
{
	path.type.Clear();
	path.data.Clear();
	current = move = Null;
	ccontrol = qcontrol = Pointf(0, 0);
}

Pointf BufferPainter::PathPoint(const Pointf& p, bool rel)
{
	Pointf r;
	r.x = IsNull(p.x) ? current.x : rel ? p.x + current.x : r.x;
	r.y = IsNull(p.y) ? current.y : rel ? p.y + current.y : r.y;
	if(IsNull(current)) {
		ClearPath();
		pathrect.left = pathrect.right = r.x;
		pathrect.top = pathrect.bottom = r.y;
		pathattr = attr;
	}
	else {
		pathrect.left = min(pathrect.left, r.x);
		pathrect.top = min(pathrect.top, r.y);
		pathrect.right = max(pathrect.right, r.x);
		pathrect.bottom = max(pathrect.bottom, r.y);
	}
	return p;
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
	PathAdd<LinearData>(MOVE).p = move = ccontrol = qcontrol = EndPoint(p, rel);
}

void BufferPainter::DoMove0()
{
	if(IsNull(move))
		MoveOp(Pointf(0, 0), false);
}

void BufferPainter::LineOp(const Pointf& p, bool rel)
{
	DoMove0();
	PathAdd<LinearData>(LINE).p = ccontrol = qcontrol = EndPoint(p, rel);
}

void BufferPainter::QuadraticOp(const Pointf& p1, const Pointf& p, bool rel)
{
	DoMove0();
	QuadraticData& m = PathAdd<QuadraticData>(QUADRATIC);
	qcontrol = m.p1 = PathPoint(p1, rel);
	m.p = EndPoint(p, rel);
}

void BufferPainter::QuadraticOp(const Pointf& p, bool rel)
{
	QuadraticOp(2 * current - qcontrol, p, rel);
}

void BufferPainter::CubicOp(const Pointf& p1, const Pointf& p2, const Pointf& p, bool rel)
{
	DoMove0();
	CubicData& m = PathAdd<CubicData>(CUBIC);
	m.p1 = PathPoint(p1, rel);
	ccontrol = m.p2 = PathPoint(p2, rel);
	m.p = EndPoint(p, rel);
}

void BufferPainter::CubicOp(const Pointf& p2, const Pointf& p, bool rel)
{
	CubicOp(2 * current - ccontrol, p2, p, rel);
}

void BufferPainter::CloseOp()
{
	if(!IsNull(move) && current != move) {
		Line(move);
		move = Null;
	}
}

END_UPP_NAMESPACE
