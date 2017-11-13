#include "DXF.h"

// private constructor -- may be created only by DXFEntities
DXFLine::DXFLine(DXFEntities *e) : DXFEntity(e, "LINE")
{
	p1 = p2 = Pointf(0,0);
}

Vector<Pointf> DXFLine::GetPoints(void) const
{
	return pick(Vector<Pointf>() << p1 << p2);
}

DXFLine &DXFLine::SetPoints(Pointf const &_p1, Pointf const &_p2)
{
	p1 = T(_p1);
	p2 = T(_p2);
	return *this;
}

bool DXFLine::Write(Stream &s)
{
	// call base class write
	if(!DXFEntity::Write(s))
		return false;
	
	// output line points
	s << "100\nAcDbLine\n";
	s << "10\n" << p1.x << "\n20\n" << p1.y << "\n30\n" << 0.0 << "\n";
	s << "11\n" << p2.x << "\n21\n" << p2.y << "\n31\n" << 0.0 << "\n";
	return true;
}

// gets bounding box of element
Rectf DXFLine::GetBoundingBox(void) const
{
	return Rectf(min(p1.x, p2.x), max(p1.y, p2.y), max(p1.x, p2.x), min(p1.y, p2.y));
}
