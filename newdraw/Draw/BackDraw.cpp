#include "Draw.h"

NAMESPACE_UPP

SystemDraw& ScreenInfo();

bool BackDraw::IsPaintingOp(const Rect& r) const
{
	Rect rr = r + GetOffset();
	if(!rr.Intersects(size))
		return false;
	return painting ? painting->IsPainting(rr + painting_offset) : true;
}

BackDraw::BackDraw()
{
	painting = NULL;
	painting_offset = Point(0, 0);
}

BackDraw::~BackDraw()
{
	Destroy();
}

END_UPP_NAMESPACE
