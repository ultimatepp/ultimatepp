#include "glyph.h"

void FDraw::Init(const Rect& r)
{
	Cloff& c = cloff.Add();
	c.clip.Add(r);
	c.offset = Point(0, 0);
}

void FDraw::BeginOp()
{
	Cloff& c = cloff.Top();
	cloff.Add() <<= c;
}

bool FDraw::ClipOp(const Rect& r)
{
	Cloff& c = cloff.Top();
	Cloff& c1 = cloff.Add();
	c1.clip = Intersection(c.clip, r + c.offset);
	c1.offset = c.offset;
	return c1.clip.GetCount();
}

bool FDraw::ClipoffOp(const Rect& r)
{
	Cloff& c = cloff.Top();
	Cloff& c1 = cloff.Add();
	c1.clip = Intersection(c.clip, r + c.offset);
	c1.offset = c.offset + r.TopLeft();
	return c1.clip.GetCount();
}

bool FDraw::IntersectClipOp(const Rect& r)
{
	Cloff& c = cloff.Top();
	c.clip = Intersection(c.clip, r + c.offset);
	return c.clip.GetCount();
}

bool FDraw::ExcludeClipOp(const Rect& r)
{
	Cloff& c = cloff.Top();
	bool dummy;
	c.clip = Subtract(c.clip, r + c.offset, dummy);
	return c.clip.GetCount();
}

bool FDraw::IsPaintingOp(const Rect& r) const
{
	return true;
}

void FDraw::OffsetOp(Point p)
{
	Cloff& c = cloff.Top();
	Cloff& c1 = cloff.Add();
	c1.clip <<= c.clip;
	c1.offset = c.offset + p;
}

void FDraw::EndOp()
{
	cloff.Drop();
}
