#include "DXF.h"

// private constructor -- may be created only by DXFEntities
DXFCircle::DXFCircle(DXFEntities *e) : DXFEntity(e, "CIRCLE")
{
	center.x = center.y = 0;
	radius = 1;
}
	
// write to stream
bool DXFCircle::Write(Stream &s)
{
	// call base class write
	if(!DXFEntity::Write(s))
		return false;
	
	// output line points
	s << "100\nAcDbCircle\n";
	s << "10\n" << center.x << "\n20\n" << center.y << "\n30\n" << 0.0 << "\n";
	s << "40\n" << radius << "\n";
	return true;
}
		
// setters
DXFCircle &DXFCircle::Set(Pointf const &c)
{
	center = T(c);
	return *this;
}

DXFCircle &DXFCircle::Set(double d)
{
	radius = S(d);
	return *this;
}

DXFCircle &DXFCircle::Set(Pointf c, double d)
{
	center = T(c);
	radius = S(d);
	return *this;
}

// gets bounding box of element
Rectf DXFCircle::GetBoundingBox(void) const
{
	return Rectf(center.x - radius, center.y + radius, center.x + radius, center.y - radius);
}
