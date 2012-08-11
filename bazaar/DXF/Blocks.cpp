#include "DXF.h"

// private constructor -- may be created only by DXF class
DXFBlocks::DXFBlocks(DXF *d)
{
	dxf = d;
}

// write tables section to stream
bool DXFBlocks::Write(Stream &s)
{
	// objects table with placeholder
	s << "0\nSECTION\n2\nBLOCKS\n";
	s << (const char *)dxf_blk;
	for(int i = 0; i < blocks.GetCount(); i++)
		blocks[i].Write(blocks.GetKey(i), s);
	s << "0\nENDSEC\n";
	
	return true;
}
