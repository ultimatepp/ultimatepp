#include "PixRaster.h"

NAMESPACE_UPP

// top level page segmenting
bool PixRaster::GetRegionsBinary(int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *ht, *tl, *tb;
	PIX *pix = pixaGetPix(pixa, page, L_CLONE);
	int res = pixGetRegionsBinary(pix, &ht, &tl, &tb, 0);
	pixDestroy(&pix);
	if(res)
		return false;
	AddPIX(ht);
	AddPIX(tl);
	AddPIX(tb);
	pixDestroy(&ht);
	pixDestroy(&tl);
	pixDestroy(&tb);
	
	// seeks the ht page
	SeekPage(-2);
	
	return true;
	
}

END_UPP_NAMESPACE
