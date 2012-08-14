#include "DXF.h"

//////////////////////////////////////////////////////////////////////////
// private constructor -- may be created only by DXF class
DXFHeader::DXFHeader(DXF *d)
{
	dxf = d;
	
}

// write header to stream
bool DXFHeader::Write(Stream &s)
{
	String hdr = (const char *)dxf_hdr;
	s << "0\nSECTION\n2\nHEADER\n";
	s << hdr;
	s << "9\n$CLAYER\n8\n" << ToCharset(CHARSET_WIN1252, dxf->layer) << "\n";
	s << "9\n$CELTYPE\n6\n" << ToCharset(CHARSET_WIN1252, dxf->lineType) << "\n";
	s << "9\n$CECOLOR\n62\n" << dxf->color << "\n";
	s << "9\n$CELTSCALE\n40\n1.0\n";
	s << "0\nENDSEC\n";
	return true;
}