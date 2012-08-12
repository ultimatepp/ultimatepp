#ifndef _DXF_Entity_h_
#define _DXF_Entity_h_

#include <Core/Core.h>
using namespace Upp;

class DXFEntities;
class DXF;
class DXFEntity
{
	friend class DXFEntities;
	private:
	
		// container entities object
		Ptr<DXFEntities> entities;
	
		// color
		int color;
		
		// linetype
		String lineType;
		
		// layer
		String layer;
		
		// handle
		uint64 handle;
		
		// gets next available handle
		uint64 GetNextHandle(void);
		
		// kind of entity -- initialized by constructor
		String kind;
	
	protected:
	
		// gets DXF object
		DXF &GetDXF(void);
		DXF const &GetDXF(void) const;
	
		// write to stream
		// must be redefined by derived classes
		// calling the inherited one
		virtual bool Write(Stream &s);
		
		// may be constructed only by entities container class
		DXFEntity(DXFEntities *e, String const &k);
		
		// apply transformation to point
		Pointf T(Pointf const &p) const;
		
		// scale a distance by transtormation matrix
		double S(double d) const;
		
		// rotate an angle by transformation matrix
		double R(double a) const;
	
	public:
	
		// needs virtual destructor !
		virtual ~DXFEntity() {}
	
		// gets/sets color
		DXFEntity &SetColor(int c);
		int GetColor(void) const { return color; }
		
		// gets/sets layer
		DXFEntity &SetLayer(String const &l);
		String const &GetLayer(void) const { return layer; }
		
		// gets/sets linetype
		DXFEntity &SetLineType(String const &l);
		String const &GetLineType(void) const { return lineType; }
		
		// gets bounding box of element
		virtual Rectf GetBoundingBox(void) const { return Rectf(1e99, -1e99, -1e99, 1e99); }
};

#endif
