#include "glyph.h"

void FDraw::BeginOp()
{
}

bool FDraw::ClipOp(const Rect& r)
{
	return false;
}

bool FDraw::ClipoffOp(const Rect& r)
{
	return false;
}

bool FDraw::IntersectClipOp(const Rect& r)
{
	return false;
}

bool FDraw::ExcludeClipOp(const Rect& r)
{
	return false;
}

bool FDraw::IsPaintingOp(const Rect& r) const
{
	return false;
}

void FDraw::OffsetOp(Point p)
{
}

void FDraw::EndOp()
{
}
