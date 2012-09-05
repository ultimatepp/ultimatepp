#include "DXF.h"


	
//////////////////////////////////////////////////////////////////////
// may be constructed only by DXF class
DXFEntities::DXFEntities(DXFBlock *b)
{
	block = b;
}

// gets next available handle
uint64 DXFEntities::GetNextHandle(void)
{
	return block->GetDXF().GetNextHandle();
}
		
// gets pointer to main class DXF
DXF &DXFEntities::GetDXF(void) const
{
	return block->GetDXF();
}
		
// write entities to stream without header
// used in blocks writing
bool DXFEntities::WriteNoHeader(Stream &s)
{
	for(int i = 0; i < entities.GetCount(); i++)
		entities[i].Write(s);
	return true;
}

// write entitie to stream
bool DXFEntities::Write(Stream &s)
{
	s << "0\nSECTION\n2\nENTITIES\n";
	bool res = WriteNoHeader(s);
	s << "0\nENDSEC\n";
	return res;
}
	
// gets default layer, color, linetype and scale
String const &DXFEntities::GetCurrentLayer(void) const
{
	return block->GetCurrentLayer();
}

String const &DXFEntities::GetCurrentLineType(void) const
{
	return block->GetCurrentLineType();
}

int DXFEntities::GetCurrentColor(void) const
{
	return block->GetCurrentColor();
}

TransMatrix const &DXFEntities::GetTransformationMatrix(void) const
{
	return block->GetTransformationMatrix();
}

// create entities

DXFPoint &DXFEntities::AddPoint(Pointf const &p)
{
	DXFPoint *pt = new DXFPoint(this);
	pt->SetPoint(p);
	entities.Add(pt);
	return *pt;
}

DXFLine &DXFEntities::AddLine(Pointf const &p1, Pointf const &p2)
{
	DXFLine *l = new DXFLine(this);
	l->SetPoints(p1, p2);
	entities.Add(l);
	return *l;
}

DXFLwPolyline &DXFEntities::AddLwPolyline(void)
{
	DXFLwPolyline *pl = new DXFLwPolyline(this);
	entities.Add(pl);
	return *pl;
}

DXFLwPolyline &DXFEntities::AddLwPolyline(Vector<Pointf> const &pts)
{
	return AddLwPolyline().AddPoints(pts);
}

DXFLwPolyline &DXFEntities::AddLwPolyline(Pointf const &p1, Pointf const &p2)
{
	return AddLwPolyline().AddPoints(p1, p2);
}

DXFLwPolyline &DXFEntities::AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3)
{
	return AddLwPolyline().AddPoints(p1, p2, p3);
}

DXFLwPolyline &DXFEntities::AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4)
{
	return AddLwPolyline().AddPoints(p1, p2, p3, p4);
}

DXFLwPolyline &DXFEntities::AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5)
{
	return AddLwPolyline().AddPoints(p1, p2, p3, p4, p5);
}

DXFLwPolyline &DXFEntities::AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6)
{
	return AddLwPolyline().AddPoints(p1, p2, p3, p4, p5, p6);
}

DXFLwPolyline &DXFEntities::AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6, Pointf const &p7)
{
	return AddLwPolyline().AddPoints(p1, p2, p3, p4, p5, p6, p7);
}

DXFLwPolyline &DXFEntities::AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6, Pointf const &p7, Pointf const &p8)
{
	return AddLwPolyline().AddPoints(p1, p2, p3, p4, p5, p6, p8);
}

// CIRCLE
DXFCircle &DXFEntities::AddCircle(Pointf const &center, double radius)
{
	DXFCircle *circle = new DXFCircle(this);
	circle->Set(center, radius);
	entities.Add(circle);
	return *circle;
}

// INSERT
DXFBlockRef &DXFEntities::InsertBlock(String const &name, Pointf const &insp, double scale, double angle)
{
	DXFBlockRef *blk = new DXFBlockRef(this);
	blk->Set(name, insp, scale, angle);
	entities.Add(blk);
	return *blk;
}

// get bounding box of entities
Rectf DXFEntities::GetBoundingBox(void) const
{
	Rectf bb(1e99, -1e99, -1e99, 1e99);
	for(int i = 0; i < entities.GetCount(); i++)
	{
		Rectf r = entities[i].GetBoundingBox();
		bb.left		= min(bb.left, r.left);
		bb.top		= max(bb.top, r.top);
		bb.right	= max(bb.right, r.right);
		bb.bottom	= min(bb.bottom, r.bottom);
	}
	return bb;
}
