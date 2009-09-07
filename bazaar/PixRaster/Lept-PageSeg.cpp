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

/*
// text baseline finding routine
Array<Point> PixRaster::FindBaselines(int page)
{
	PTA *pta;
	NUMA *numa;
	int res;
	l_int32 x, y;
	Array<Point> ptArray;

	if(IsEmpty())
		return Array<Point>();
	page = getTruePage(page);
	
	PIX *pix = pixaGetPix(pixa, page, L_CLONE);
	numa = pixFindBaselines(pix, &pta, 0);
	pixDestroy(&pix);
	if(!numa || !pta)
		return Array<Point>();
	for(int iPoint = 0; iPoint < ptaGetCount(pta); iPoint++)
	{
		res = ptaGetIPt(pta, iPoint, &x, &y);
		if(res)
		{
			numaDestroy(&numa);
			ptaDestroy(&pta);
			return Array<Point>();
		}
		ptArray.Add(Point(x, y));
	}
	numaDestroy(&numa);
	ptaDestroy(&pta);
	return ptArray;
}
*/

// text baseline finding routine
Array<int> PixRaster::FindBaselines(int page)
{
	NUMA *numa;
	int res;
	l_int32 y;
	Array<int> intArray;

	if(IsEmpty())
		return Array<int>();
	page = getTruePage(page);
	
	PIX *pix = pixaGetPix(pixa, page, L_CLONE);
	numa = pixFindBaselines(pix, NULL, 0);
	pixDestroy(&pix);
	if(!numa)
		return Array<int>();
	for(int iLine = 0; iLine < numaGetCount(numa); iLine++)
	{
		res = numaGetIValue(numa, iLine, &y);
		if(res)
		{
			numaDestroy(&numa);
			return Array<int>();
		}
		intArray.Add(y);
	}
	numaDestroy(&numa);
	return intArray;
}

END_UPP_NAMESPACE
