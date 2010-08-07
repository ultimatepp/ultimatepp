#include "PixRaster.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// converts pix to monochrome 1bpp
Pix Pix::ConvertTo1(int threshold)
{
	if(IsEmpty())
		return Pix();
	
	PIX *dPix = pixConvertTo1(pix, threshold);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
}

END_UPP_NAMESPACE
