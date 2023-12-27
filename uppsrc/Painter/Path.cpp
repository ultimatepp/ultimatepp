#include "Painter.h"

#define LLOG(x) // DLOG(x)

namespace Upp {

void BufferPainter::ClearPath()
{
	current = move = Null;
	ccontrol = qcontrol = Pointf(0, 0);
	path_info->ischar = false;
	path_info->path_min = Pointf(1e200, 1e200);
	path_info->path_max = -Pointf(1e200, 1e200);
	path_info->path.SetCount(1);

	Vector<byte>& p = path_info->path.Top();
	if(path_info->path.Top().GetCount() > 2048) {
		p.Clear();
		p.Reserve(128);
	}
	else
		p.SetCount(0);
}

void BufferPainter::DoPath0()
{
	path_index = (path_index + co) & (BATCH_SIZE - 1);
	path_info = &paths[path_index];
	ClearPath();
	pathattr = attr;
	current = Pointf(0, 0);
}

Pointf BufferPainter::PathPoint(const Pointf& p, bool rel)
{
	DoPath();
	Pointf r;
	r.x = IsNull(p.x) ? current.x : rel ? p.x + current.x : p.x;
	r.y = IsNull(p.y) ? current.y : rel ? p.y + current.y : p.y;
	if(dopreclip) {
		path_info->path_min = min(r, path_info->path_min);
		path_info->path_max = max(r, path_info->path_max);
	}
	return r;
}

Pointf BufferPainter::EndPoint(const Pointf& p, bool rel)
{
	return current = PathPoint(p, rel);
}

template <class T> T& BufferPainter::PathAdd(int type)
{
	Vector<byte>& p = path_info->path.Top();
	int l = p.GetCount();
	p.SetCount(l + sizeof(T));
	T *e = (T *)(p.begin() + l);
	e->type = type;
	return *e;
}

void BufferPainter::ClearOp(const RGBA& color)
{
	Finish();
	if(co && mode != MODE_NOAA) { // schedule for late clear during rendering
		if(!co_clear)
			co_clear.Alloc(ip->GetHeight());
		memset(~co_clear, 1, ip->GetHeight());
		co_clear_color = color;
	}
	else
		UPP::Fill(~*ip, color, ip->GetLength());
	if(color.a == 255)
		ip->SetKind(IMAGE_OPAQUE);
}

void BufferPainter::MoveOp(const Pointf& p, bool rel)
{
	LLOG("@ MoveOp " << p << ", " << rel);
	move = ccontrol = qcontrol = EndPoint(p, rel);
	PathAdd<LinearData>(MOVE).p = move;
}

force_inline
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
	LLOG("@ QuadraticOp " << p1 << ", " << p << ", " << rel);
	DoMove0();
	auto& m = PathAdd<QuadraticData>(QUADRATIC);
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
	auto& m = PathAdd<CubicData>(CUBIC);
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
	if(!IsNull(current) && !IsNull(move) && current != move) {
		Line(move);
		move = Null;
	}
}

void BufferPainter::DivOp()
{
	LLOG("@ DivOp");
	CloseOp();
	path_info->path.Add();
}

void BufferPainter::CharacterOp(const Pointf& p, int ch, Font fnt)
{
	LLOG("@ CharacterOp " << p << ", " << ch << ", " << fnt);
#if 0
	DoMove0();
	PaintCharacter(*this, p, ch, fnt);
#else
	move = current = EndPoint(p, false);
	auto& m = PathAdd<CharData>(CHAR);
	m.p = EndPoint(p, false);
	m.ch = ch;
	m.fnt = fnt;
	path_info->ischar = true;
	EvenOdd();
#endif
}

}
