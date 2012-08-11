#include "DXF.h"

// write header to stream
bool DXFHeader::Write(Stream &s)
{
	String hdr = (const char *)dxf_hdr;
	s << hdr;
	return true;
}