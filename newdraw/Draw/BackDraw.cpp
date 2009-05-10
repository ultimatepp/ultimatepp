#include "Draw.h"

NAMESPACE_UPP

SystemDraw& ScreenInfo();

void BackDraw::Create(int cx, int cy)
{
	DrawLock __;
	Create(ScreenInfo(), cx, cy);
	style = GUI|BACK;
}

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

bool ScreenInPaletteMode()
{
	return ScreenInfo().PaletteMode();
}

Size GetScreenSize()
{
	return ScreenInfo().GetPagePixels();
}

END_UPP_NAMESPACE
