#include "PixRaster.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::Invert()
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixInvert(NULL, pix);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::Invert()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::AddConstantGray(int val)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixCopy(NULL, pix);
	int res = pixAddConstantGray(dPix, val);
	if(res)
	{
		pixDestroy(&dPix);
		return Pix();
	}
	return Pix(&dPix);

} // END Pix::AddConstantGray()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::MultConstantGray(int val)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixCopy(NULL, pix);
	int res = pixMultConstantGray(dPix, val);
	if(res)
	{
		pixDestroy(&dPix);
		return Pix();
	}
	return Pix(&dPix);

} // END Pix::MultConstantGray()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::AddGray(Pix &pix2)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixCopy(NULL, pix);
	PIX *res = pixAddGray(dPix, pix, pix2);
	if(!res)
	{
		pixDestroy(&dPix);
		return Pix();
	}
	return Pix(&dPix);

} // END Pix::AddGray()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::SubtractGray(Pix &pix2)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixCopy(NULL, pix);
	PIX *res = pixSubtractGray(dPix, pix, pix2);
	if(!res)
	{
		pixDestroy(&dPix);
		return Pix();
	}
	return Pix(&dPix);

} // END Pix::SubtractGray()

END_UPP_NAMESPACE
