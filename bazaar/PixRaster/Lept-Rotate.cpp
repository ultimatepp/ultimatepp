#include "PixRaster.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rotates image (2-4-8 or 32 bpp) about center.
Pix Pix::RotateAM(double angle, BringInModes incolor)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixRotateAM(pix, angle, incolor);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::RotateAM()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::RotateAMColor(double angle, int incolor)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixRotateAMColor(pix, angle, incolor);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::RotateAMColor()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::RotateAMGray(double angle, int incolor)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixRotateAMGray(pix, angle, incolor);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::RotateAMGray()

END_UPP_NAMESPACE
