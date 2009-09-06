#include "PixRaster.h"

NAMESPACE_UPP

bool PixRaster::Invert(int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, PIXRASTER_CLONE);
	PIX *dPix = pixInvert(NULL, sPix);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::AddConstantGray(int val, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	Dup(page);
	PIX *sPix = GetPIX(PIXRASTER_LASTPAGE, PIXRASTER_CLONE);
	int res = pixAddConstantGray(sPix, val);
	pixDestroy(&sPix);
	if(res)
	{
		Drop();
		return false;
	}
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::MultConstantGray(int val, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	Dup(page);
	PIX *sPix = GetPIX(PIXRASTER_LASTPAGE, PIXRASTER_CLONE);
	int res = pixMultConstantGray(sPix, val);
	pixDestroy(&sPix);
	if(res)
	{
		Drop();
		return false;
	}
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::AddGray(int page1, int page2)
{
	if(IsEmpty())
		return false;
	page1 = getTruePage(page1);
	page2 = getTruePage(page2);
	
	PIX *sPix1 = pixaGetPix(pixa, page1, L_CLONE);
	PIX *sPix2 = pixaGetPix(pixa, page2, L_CLONE);
	Dup(page1);
	PIX *dPix = GetPIX(PIXRASTER_LASTPAGE, PIXRASTER_CLONE); 
	PIX *res = pixAddGray(dPix, sPix1, sPix2);
	pixDestroy(&sPix1);
	pixDestroy(&sPix2);
	pixDestroy(&dPix);
	if(!res)
	{
		Drop();
		return false;
	}
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::SubtractGray(int page1, int page2)
{
	if(IsEmpty())
		return false;
	page1 = getTruePage(page1);
	page2 = getTruePage(page2);
	
	PIX *sPix1 = pixaGetPix(pixa, page1, L_CLONE);
	PIX *sPix2 = pixaGetPix(pixa, page2, L_CLONE);
	PIX *dPix = pixSubtractGray(NULL, sPix1, sPix2);
	pixDestroy(&sPix1);
	pixDestroy(&sPix2);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

END_UPP_NAMESPACE
