#ifndef _DXF_Header_h_
#define _DXF_Header_h_

#include <Core/Core.h>
using namespace Upp;

class DXFHeader
{
	friend class DXF;
	
	private:
		// write header to stream
		bool Write(Stream &s);

	protected:
	
	public:
};

#endif
