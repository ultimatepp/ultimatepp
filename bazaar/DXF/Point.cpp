#include "DXF.h"

// private constructor -- may be created only by DXFEntities
DXFPoint::DXFPoint(DXFEntities *e) : DXFEntity(e, "POINT")
{
	p = Pointf(0,0);
}

DXFPoint &DXFPoint::SetPoint(Pointf const &_p)
{
	p = T(_p);
	return *this;
}

bool DXFPoint::Write(Stream &s)
{
	// call base class write
	if(!DXFEntity::Write(s))
		return false;
	
	// output line points
	s << "100\nAcDbPoint\n";
	s << "10\n" << p.x << "\n20\n" << p.y << "\n30\n" << 0.0 << "\n";
	return true;
}

// gets bounding box of element
Rectf DXFPoint::GetBoundingBox(void) const
{
	return Rectf(p.x, p.y, p.x, p.y);
}
