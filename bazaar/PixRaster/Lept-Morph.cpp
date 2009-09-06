#include "PixRaster.h"

NAMESPACE_UPP

bool PixRaster::ErodeGray(int hsize, int vsize, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixErodeGray(sPix, hsize, vsize);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

bool PixRaster::DilateGray(int hsize, int vsize, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixDilateGray(sPix, hsize, vsize);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

bool PixRaster::OpenGray(int hsize, int vsize, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixOpenGray(sPix, hsize, vsize);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

bool PixRaster::CloseGray(int hsize, int vsize, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixCloseGray(sPix, hsize, vsize);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

END_UPP_NAMESPACE
