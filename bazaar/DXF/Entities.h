#ifndef _DXF_Entities_h_
#define _DXF_Entities_h_

#include "Entity.h"
#include "Point.h"
#include "Line.h"
#include "Circle.h"
#include "LwPolyline.h"
#include "BlockRef.h"

class DXF;
class DXFBlock;
class DXFEntities : public Pte<DXFEntities>
{
	friend class DXFBlock;
	friend class DXF;
	private:
	
		// the container DXFBlock class
		Ptr<DXFBlock> block;
	
		// the entities
		Array<DXFEntity> entities;
		
		// may be constructed only by DXF class
		DXFEntities(DXFBlock *blk);
		
		// write entities to stream without header
		// used in blocks writing
		bool WriteNoHeader(Stream &s);

		// write entitie to stream
		bool Write(Stream &s);
	
	protected:
	
	public:
	
		// gets next available handle
		uint64 GetNextHandle(void);
		
		// gets pointer to main class DXF
		DXF &GetDXF(void) const;
		
		// gets default layer, color, linetype and scale
		String const &GetCurrentLayer(void) const;
		String const &GetCurrentLineType(void) const;
		int GetCurrentColor(void) const;
		TransMatrix const &GetTransformationMatrix(void) const;
	
		// apply transformation to point
		Pointf T(Pointf const &p) const { return GetTransformationMatrix()(p); }
	
		// create entities
		
		// POINT
		DXFPoint &AddPoint(Pointf const &p);
		
		// LINE
		DXFLine &AddLine(Pointf const &p1, Pointf const &p2);
		
		// LWPOLYLINE, with some handy shortcuts
		DXFLwPolyline &AddLwPolyline(void);
		DXFLwPolyline &AddLwPolyline(Vector<Pointf> const &pts);
		DXFLwPolyline &AddLwPolyline(Pointf const &p1, Pointf const &p2);
		DXFLwPolyline &AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3);
		DXFLwPolyline &AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4);
		DXFLwPolyline &AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5);
		DXFLwPolyline &AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6);
		DXFLwPolyline &AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6, Pointf const &p7);
		DXFLwPolyline &AddLwPolyline(Pointf const &p1, Pointf const &p2, Pointf const &p3, Pointf const &p4, Pointf const &p5, Pointf const &p6, Pointf const &p7, Pointf const &p8);
		
		// CIRCLE
		DXFCircle &AddCircle(Pointf const &center, double radius);
		
		// INSERT
		DXFBlockRef &InsertBlock(String const &name, Pointf const &insp, double scale, double angle);
		DXFBlockRef &InsertBlock(String const &name, Pointf const &insp, double scale) { return InsertBlock(name, insp, scale, 0.0); }
		DXFBlockRef &InsertBlock(String const &name, Pointf const &insp) { return InsertBlock(name, insp, 1.0, 0.0); }
		
		// get bounding box of entities
		Rectf GetBoundingBox(void) const;
};

#endif
