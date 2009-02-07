#include "ScanLine.h"

double SquareDist(const Pointf& p1, const Pointf& p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

Pointf Mid(const Pointf& a, const Pointf& b)
{
	return 0.5 * (a + b);
}

Pointf Ortogonal(const Pointf& p)
{
	return Pointf(-p.y, p.x);
}

double SquareLength(const Pointf& p)
{
	return p.x * p.x + p.y * p.y;
}

double Length(const Pointf& p)
{
	return sqrt(SquareLength(p));
}

double Bearing(const Pointf& p)
{
	return atan2(p.y, p.x);
}

double Distance(const Pointf& p1, const Pointf& p2)
{
	return Length(p1 - p2);
}

double SquareDistance(const Pointf& p1, const Pointf& p2)
{
	return SquareLength(p1 - p2);
}

Pointf PolarPointf(double a)
{
	return Pointf(cos(a), sin(a));
}

Pointf Polar(const Pointf& p, double r, double a)
{
	return p + r * PolarPointf(a);
}
