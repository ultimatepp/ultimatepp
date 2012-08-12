#ifndef _DXF_BlockRef_h_
#define _DXF_BlockRef_h_

#include "Entity.h"

class DXFBlockRef : public DXFEntity 
{
	friend class DXFEntities;
	private:
	
		// block name
		String name;
	
		// insertion point
		Pointf insertPoint;
		
		// scale factor
		double scale;
		
		// insertion angle
		double angle;

	protected:
	
		// write to stream
		virtual bool Write(Stream &s);
		
		// private constructor -- may be created only by DXFEntities
		DXFBlockRef(DXFEntities *e);
		
		// scale distance depending on scale option in DXF
		double S(double d) const;
	
	public:

		// setters
		DXFBlockRef &SetName(String const &n);
		DXFBlockRef &SetInsertPoint(Pointf const &p);
		DXFBlockRef &SetScale(double s);
		DXFBlockRef &SetAngle(double a);
		DXFBlockRef &Set(Pointf const &p, double s, double a);
		DXFBlockRef &Set(String const &n, Pointf const &p, double s, double a);

		// gets bounding box of element
		virtual Rectf GetBoundingBox(void) const;
};

#endif
