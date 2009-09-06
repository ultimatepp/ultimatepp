#include "PixRaster.h"

NAMESPACE_UPP

// Rotates image (2-4-8 or 32 bpp) about center.
bool PixRaster::RotateAM(double angle, BringInModes incolor, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixRotateAM(sPix, angle, incolor);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
}

bool PixRaster::RotateAMColor(double angle, int incolor, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixRotateAMColor(sPix, angle, incolor);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
}

bool PixRaster::RotateAMGray(double angle, int incolor, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixRotateAMGray(sPix, angle, incolor);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
}

END_UPP_NAMESPACE
