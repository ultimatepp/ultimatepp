#include "DXF.h"

// private constructor -- may be created only by DXFEntities
DXFBlockRef::DXFBlockRef(DXFEntities *e) : DXFEntity(e, "INSERT")
{
	insertPoint.x = insertPoint.y = 0;
	angle = 0;
	scale = 1;
	name = "unnamed";
}

// write to stream
bool DXFBlockRef::Write(Stream &s)
{
	// call base class write
	if(!DXFEntity::Write(s))
		return false;
	
	// output line points
	s << "100\nAcDbBlockReference\n";
	s << "2\n" << name << "\n";
	s << "10\n" << insertPoint.x << "\n20\n" << insertPoint.y << "\n30\n" << 0.0 << "\n";
	s << "41\n" << scale << "\n42\n" << scale << "\n43\n" << scale << "\n";
	s << "50\n" << angle << "\n";
	return true;
}

// scale distance depending on scale option in DXF
double DXFBlockRef::S(double d) const
{
	if(GetDXF().GetScaleInsertions())
		return DXFEntity::S(d);
	return d;
}

// setters
DXFBlockRef &DXFBlockRef::SetName(String const &n)
{
	name = n;
	return *this;
}

DXFBlockRef &DXFBlockRef::SetInsertPoint(Pointf const &p)
{
	insertPoint = T(p);
	return *this;
}

DXFBlockRef &DXFBlockRef::SetScale(double s)
{
	scale = S(s);
	return *this;
}

DXFBlockRef &DXFBlockRef::SetAngle(double a)
{
	angle = R(a);
	return *this;
}

DXFBlockRef &DXFBlockRef::Set(Pointf const &p, double s, double a)
{
	insertPoint = T(p);
	scale = S(s);
	angle = R(a);
	return *this;
}

DXFBlockRef &DXFBlockRef::Set(String const &n, Pointf const &p, double s, double a)
{
	name = n;
	insertPoint = T(p);
	scale = S(s);
	angle = R(a);
	return *this;
}
