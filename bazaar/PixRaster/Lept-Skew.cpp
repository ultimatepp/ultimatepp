#include "PixRaster.h"

NAMESPACE_UPP

// Simple top-level deskew interfaces
bool PixRaster::Deskew(int ReductionFactor, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixDeskew(sPix, ReductionFactor);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

bool PixRaster::FindSkewAndDeskew(int ReductionFactor, double *skewAngle, double *confidenceFactor, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	l_float32 dSkew, dConf;
	PIX *dPix = pixFindSkewAndDeskew(sPix, ReductionFactor, &dSkew, &dConf);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	if(skewAngle)
		*skewAngle = dSkew;
	if(confidenceFactor)
		*confidenceFactor = dConf;
	return true;
	
}

// Simple top-level skew angle finding interface
bool PixRaster::FindSkew(double *pangle, double *pconf, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	l_float32 dSkew, dConf;
	int res = pixFindSkew(sPix, &dSkew, &dConf);
	pixDestroy(&sPix);
	if(res)
		return false;
	if(pangle)
		*pangle = dSkew;
	if(pconf)
		*pconf = dConf;
	return true;
	
}

// Basic angle-finding functions with all parameters
bool PixRaster::FindSkewSweep(double *pangle, int reduction, double sweeprange, double sweepdelta, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	l_float32 dSkew;
	int res = pixFindSkewSweep(sPix, &dSkew, reduction, sweeprange, sweepdelta);
	pixDestroy(&sPix);
	if(res)
		return false;
	if(pangle)
		*pangle = dSkew;
	return true;
	
}

// Angle-finding functions with all parameters
bool PixRaster::FindSkewSweepAndSearch(
	double *pangle, double *pconf,
	int redsweep, int redsearch,
	double sweeprange, double sweepdelta,
	double minbsdelta,
	int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	l_float32 dSkew, dConf;
	int res = pixFindSkewSweepAndSearch(sPix, &dSkew, &dConf, redsweep, redsearch, sweeprange, sweepdelta, minbsdelta);
	pixDestroy(&sPix);
	if(res)
		return false;
	if(pangle)
		*pangle = dSkew;
	if(pconf)
		*pconf = dConf;
	return true;
	
}

bool PixRaster::DeskewLocal(int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixDeskewLocal(sPix, 0, 0, 0, 0.0, 0.0, 0.0);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	SeekPage(PIXRASTER_LASTPAGE);
	pixDestroy(&dPix);
	return true;
}


END_UPP_NAMESPACE
