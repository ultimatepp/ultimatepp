#ifndef _DXF_Header_h_
#define _DXF_Header_h_

#include <Core/Core.h>
using namespace Upp;

class DXFHeader
{
	friend class DXF;
	
	private:
		// back link to container DXF
		Ptr<DXF> dxf;
		
		// write header to stream
		bool Write(Stream &s);

		// private constructor -- may be created only by DXF class
		DXFHeader(DXF *d);
		
	protected:
	
	public:
};

#endif
