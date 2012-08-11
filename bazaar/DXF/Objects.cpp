#include "DXF.h"

// private constructor -- may be created only by DXF class
DXFObjects::DXFObjects(DXF *d)
{
	dxf = d;
}

// write tables section to stream
bool DXFObjects::Write(Stream &s)
{
	// objects table with placeholder
	s << "0\nSECTION\n2\nOBJECTS\n";
	s << (const char *)dxf_obj;
	s << "0\nENDSEC\n";
	
	return true;
}
