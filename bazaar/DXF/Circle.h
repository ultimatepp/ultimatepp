#ifndef _DXF_Circle_h_
#define _DXF_Circle_h_

#include "Entity.h"

class DXFCircle : public DXFEntity
{
	friend class DXFEntities;
	private:
	
		// center point and radius
		Pointf center;
		double radius;

	protected:
	
		// write to stream
		virtual bool Write(Stream &s);
		
		// private constructor -- may be created only by DXFEntities
		DXFCircle(DXFEntities *e);
	
	public:

		// setters
		DXFCircle &Set(Pointf const &c);
		DXFCircle &Set(double d);
		DXFCircle &Set(Pointf c, double d);

		// gets bounding box of element
		virtual Rectf GetBoundingBox(void) const;
};

#endif
