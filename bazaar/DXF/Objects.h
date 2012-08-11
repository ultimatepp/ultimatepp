#ifndef _DXF_Objects_h_
#define _DXF_Objects_h_

#include "DXF.h"

class DXFObjects
{
	friend class DXF;
	private:
	
		// back link to container DXF
		Ptr<DXF> dxf;
		
		// private constructor -- may be created only by DXF class
		DXFObjects(DXF *d);
		
		// write tables section to stream
		bool Write(Stream &s);
	
	protected:
	
	public:
	
};
#endif
