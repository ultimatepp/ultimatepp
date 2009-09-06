#include "PixRaster.h"

NAMESPACE_UPP

bool PixRaster::CombineMasked(int destPage, int sourcePage, int maskPage)
{
	if(IsEmpty())
		return false;
	sourcePage = getTruePage(sourcePage);
	destPage = getTruePage(destPage);
	
	Dup(destPage);
	PIX *dPix = GetPIX(PIXRASTER_LASTPAGE, PIXRASTER_CLONE);
	PIX *sPix = pixaGetPix(pixa, sourcePage, L_CLONE);
	PIX *mPix = pixaGetPix(pixa, maskPage, L_CLONE);
	int res = pixCombineMasked(dPix, sPix, mPix);
	pixDestroy(&sPix);
	pixDestroy(&dPix);
	pixDestroy(&mPix);
	if(res)
	{
		Drop();
		return false;
	}
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

END_UPP_NAMESPACE
