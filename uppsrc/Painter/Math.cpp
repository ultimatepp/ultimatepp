#include "Painter.h"

NAMESPACE_UPP

double SquareDist(const Pointf& p1, const Pointf& p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

Pointf Mid(const Pointf& a, const Pointf& b)
{
	return (a + b) / 2;
}

Pointf Orthogonal(const Pointf& p)
{
	return Pointf(-p.y, p.x);
}

double Squared(const Pointf& p)
{
	return p.x * p.x + p.y * p.y;
}

double Length(const Pointf& p)
{
	return sqrt(Squared(p));
}

double Bearing(const Pointf& p)
{
	return atan2(p.y, p.x);
}

double Distance(const Pointf& p1, const Pointf& p2)
{
	return Length(p1 - p2);
}

double SquaredDistance(const Pointf& p1, const Pointf& p2)
{
	return Squared(p1 - p2);
}

Pointf Polar(double a)
{
	return Pointf(cos(a), sin(a));
}

Pointf Polar(const Pointf& p, double r, double a)
{
	return p + r * Polar(a);
}

END_UPP_NAMESPACE
