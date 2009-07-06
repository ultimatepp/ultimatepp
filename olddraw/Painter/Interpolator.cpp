#include "Painter.h"

NAMESPACE_UPP

void LinearInterpolator::Dda2::Set(int p1, int p2, int len)
{
	count = len <= 0 ? 1 : len;
	lift = (p2 - p1) / count;
	rem = (p2 - p1) % count;
	mod = rem;
	p = p1;
	if(mod <= 0) {
		mod += count;
		rem += count;
		lift--;
    }
	mod -= count;
}

int LinearInterpolator::Dda2::Get()
{
	int pp = p;
	mod += rem;
	p += lift;
	if(mod > 0) {
		mod -= count;
		p++;
	}
	return pp;
}

void LinearInterpolator::Begin(int x, int y, int len)
{
	Pointf p1 = xform.Transform(Pointf(x, y)/* + 0.5*/);
	Pointf p2 = xform.Transform(Pointf(x + len, y)/* + 0.5*/);
	ddax.Set(Q8(p1.x), Q8(p2.x), len);
	dday.Set(Q8(p1.y), Q8(p2.y), len);
}

Point LinearInterpolator::Get()
{
	return Point(ddax.Get(), dday.Get());
}

END_UPP_NAMESPACE
