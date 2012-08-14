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
	s << "2\n" << ToCharset(CHARSET_WIN1252, name) << "\n";
	s << "10\n" << insertPoint.x << "\n20\n" << insertPoint.y << "\n30\n" << 0.0 << "\n";
	s << "41\n" << scale << "\n42\n" << scale << "\n43\n" << scale << "\n";
	s << "50\n" << angle * 180.0 / M_PI << "\n";
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

// gets bounding box of element
Rectf DXFBlockRef::GetBoundingBox(void) const
{
	Rectf res;
	
	// get dxf object
	DXF const &dxf = GetDXF();

	// get block data by name; if none, returns
	if(!dxf.HasBlock(name))
		return Rectf(1e99, -1e99, -1e99, 1e99);
	DXFBlock const &blk = dxf.GetBlock(name);

	// get the bounding box of included entities
	Rectf bbe = blk.entities.GetBoundingBox();
	
	// now we need to apply transformation to bbox points
	TransMatrix M;
	M.Translate(insertPoint).Rotate(angle).Scale(scale);
	Pointf bl = M(bbe.BottomLeft());
	Pointf tl = M(bbe.TopLeft());
	Pointf br = M(bbe.BottomRight());
	Pointf tr = M(bbe.TopRight());
	
	// and now get the minimum and maximum coordinate values
	res.left	= min(min(bl.x, tl.x), min(br.x, tr.x));
	res.right	= max(max(bl.x, tl.x), max(br.x, tr.x));
	res.bottom	= min(min(bl.y, tl.y), min(br.y, tr.y));
	res.top		= max(max(bl.y, tl.y), max(br.y, tr.y));

	return res;
}
