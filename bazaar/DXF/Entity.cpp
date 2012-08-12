#include "DXF.h"

// gets DXF object
DXF &DXFEntity::GetDXF(void)
{
	return entities->GetDXF();
}
	
DXF const &DXFEntity::GetDXF(void) const
{
	return entities->GetDXF();
}
	
// write to stream
// must be redefined by derived classes
// calling the inherited one
bool DXFEntity::Write(Stream &s)
{
	s << "0\n" << kind << "\n";
	s << "8\n" << layer << "\n";
	s << "100\nAcDbEntity\n";
	if(color != DXF::ByLayer)
		s << "62\n" << color << "\n";
	if(lineType != "BYLAYER")
		s << "6\n" << lineType << "\n";
	s << "5\n" << Format64Hex(handle) << "\n";
	return true;
}
	
// may be constructed only by derived classes
DXFEntity::DXFEntity(DXFEntities *e, String const &k)
{
	entities = e;
	kind = k;
	
	color = e->GetCurrentColor();
	lineType = e->GetCurrentLineType();
	layer = e->GetCurrentLayer();
	handle = GetNextHandle();
}

// gets next available handle
uint64 DXFEntity::GetNextHandle(void)
{
	return entities->GetNextHandle();
}
		
// apply transformation to point
Pointf DXFEntity::T(Pointf const &p) const
{
	return entities->GetTransformationMatrix()(p);
}
		
// scale a distance by transtormation matrix
double DXFEntity::S(double d) const
{
	return entities->GetTransformationMatrix().S(d);
}

// rotate an angle by transformation matrix
double DXFEntity::R(double a) const
{
	return entities->GetTransformationMatrix().R(a);
}

// sets color
DXFEntity &DXFEntity::SetColor(int c)
{
	color = c;
	return *this;
}

// sets layer
DXFEntity &DXFEntity::SetLayer(String const &l)
{
	layer = l;
	return *this;
}

// sets linetype
DXFEntity &DXFEntity::SetLineType(String const &l)
{
	// linetype MUST be there, so add if not
	if(!GetDXF().tables.HasLineType(l))
		GetDXF().tables.AddLineType(l);
	
	lineType = l;
	return *this;
}
		
