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



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::ErodeBrick(int hsize, int vsize)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixErodeBrick(NULL, pix, hsize, vsize);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::ErodeBrick()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::DilateBrick(int hsize, int vsize)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixDilateBrick(NULL, pix, hsize, vsize);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::DilateBrick()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::OpenBrick(int hsize, int vsize)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixOpenBrick(NULL, pix, hsize, vsize);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::OpenBrick()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::CloseBrick(int hsize, int vsize)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixCloseBrick(NULL, pix, hsize, vsize);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::CloseBrick()

END_UPP_NAMESPACE
