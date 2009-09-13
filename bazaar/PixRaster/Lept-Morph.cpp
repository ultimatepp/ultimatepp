#include "PixRaster.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::ErodeGray(int hsize, int vsize)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixErodeGray(pix, hsize, vsize);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::ErodeGray()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::DilateGray(int hsize, int vsize)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixDilateGray(pix, hsize, vsize);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::DilateGray()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::OpenGray(int hsize, int vsize)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixOpenGray(pix, hsize, vsize);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::OpenGray()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::CloseGray(int hsize, int vsize)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixCloseGray(pix, hsize, vsize);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::CloseGray()

END_UPP_NAMESPACE
