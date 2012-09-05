#ifndef _DXF_Point_h_
#define _DXF_Point_h_

class DXFPoint : public DXFEntity
{
	friend class DXFEntities;
	private:

		// the point coordinates
		Pointf p;
		
	protected:
	
		virtual bool Write(Stream &s);
		
		// private constructor -- may be created only by DXFEntities
		DXFPoint(DXFEntities *e);
		
	public:
	
		// get/set points
		Pointf const &GetP(void) const { return p; }
		DXFPoint &SetPoint(Pointf const &p);

		// gets bounding box of element
		virtual Rectf GetBoundingBox(void) const;
};

#endif
