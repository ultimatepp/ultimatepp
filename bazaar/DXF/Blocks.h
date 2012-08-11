#ifndef _DXF_Blocks_h_
#define _DXF_Blocks_h_

#include "DXF.h"

class DXFBlocks
{
	friend class DXF;
	friend class DXFTables;
	private:
	
		// back link to container DXF
		Ptr<DXF> dxf;
		
		// private constructor -- may be created only by DXF class
		DXFBlocks(DXF *d);
		
		// write tables section to stream
		bool Write(Stream &s);
		
		// the blocks, mapped by name
		ArrayMap<String, DXFBlock> blocks;
	
	protected:
	
	public:
	
};

#endif
