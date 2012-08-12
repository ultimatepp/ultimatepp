#ifndef _DXF_Line_h_
#define _DXF_Line_h_

#include "Entity.h"

class DXFLine : public DXFEntity
{
	friend class DXFEntities;
	private:

		// the line endpoints
		Pointf p1, p2;
		
	protected:
	
		virtual bool Write(Stream &s);
		
		// private constructor -- may be created only by DXFEntities
		DXFLine(DXFEntities *e);
		
	public:
	
		// get/set points
		Pointf const &GetP1(void) const { return p1; }
		Pointf const &GetP2(void) const { return p2; }
		Vector<Pointf> GetPoints(void) const;
		DXFLine &SetPoints(Pointf const &p1, Pointf const &p2);

		// gets bounding box of element
		virtual Rectf GetBoundingBox(void) const;
};

#endif
