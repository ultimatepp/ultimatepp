#include <CtrlCore/CtrlCore.h>

#ifdef GUI_FB

NAMESPACE_UPP

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

SystemDraw::SystemDraw()
:	BufferPainter(Ctrl::framebuffer, Ctrl::renderingMode)
{
}

SystemDraw::~SystemDraw()
{
}


void SystemDraw::Push()
{
	Point p = GetOffset();
	offset.Add(p);
	BufferPainter::BeginOp();
}

void SystemDraw::Pop()
{
	if(offset.GetCount())
		offset.Drop();
	BufferPainter::EndOp();
}

Point SystemDraw::GetOffset() const
{
	return offset.GetCount() ? offset.Top() : Point(0, 0);
}

void SystemDraw::BeginOp()
{
	Push();
}

void SystemDraw::EndOp()
{
	Pop();
}

void SystemDraw::OffsetOp(Point p)
{
	Push();
	offset.Top() += p;
	Translate(p.x, p.y);
}

bool SystemDraw::ClipOp(const Rect& r)
{
	Push();
	RectPath(r);
	Painter::Clip();
	return true;
}

bool SystemDraw::ClipoffOp(const Rect& r)
{
	Push();
	offset.Top() += r.TopLeft();
	RectPath(r);
	Painter::Clip();
	Translate(r.left, r.top);
	return true;
}

bool SystemDraw::IsPaintingOp(const Rect& r) const
{
	Rect rr = r + GetOffset();
	for(int i = 0; i < Ctrl::invalid.GetCount(); i++)
		if(Ctrl::invalid[i].Intersects(rr))
			return true;
	return true;
}

/*Rect SystemDraw::GetVirtualScreenArea()
{
	GuiLock __;
}*/

/*
void BackDraw::Destroy()
{
	GuiLock __;
}

void BackDraw::Create(SystemDraw& w, int cx, int cy) {
	GuiLock __;
}

void BackDraw::Put(SystemDraw& w, int x, int y) {
	GuiLock __;
}
*/

END_UPP_NAMESPACE

#endif
