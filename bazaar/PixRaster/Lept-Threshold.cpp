#include "PixRaster.h"

NAMESPACE_UPP

bool PixRaster::DitherToBinary(int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixDitherToBinary(sPix);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

bool PixRaster::DitherToBinarySpec(int lowerclip, int upperclip, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixDitherToBinarySpec(sPix, lowerclip, upperclip);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

// threshold the image -- warning, operates ONLY on grayscale pixmaps
// original image is unchanged - a new modified image at raster's end
bool PixRaster::ThresholdToBinary(int threshold, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixThresholdToBinary(sPix, threshold);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
}

bool PixRaster::VarThresholdToBinary(int thresholdPage, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	thresholdPage = getTruePage(thresholdPage);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *tPix = pixaGetPix(pixa, thresholdPage, L_CLONE);
	PIX *dPix = pixVarThresholdToBinary(sPix, tPix);
	pixDestroy(&sPix);
	pixDestroy(&tPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

bool PixRaster::DitherToBinaryLUT(int lowerclip, int upperclip, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixDitherToBinaryLUT(sPix, lowerclip, upperclip);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

bool PixRaster::GenerateMaskByValue(int val, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixGenerateMaskByValue(sPix, val);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::GenerateMaskByBand(int lower, int upper, int inband, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixGenerateMaskByBand(sPix, lower, upper, inband);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

bool PixRaster::DitherTo2bpp(int cmapflag, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixDitherTo2bpp(sPix, cmapflag);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::DitherTo2bppSpec(int lowerclip, int upperclip, int cmapflag, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixDitherTo2bppSpec(sPix, lowerclip, upperclip, cmapflag);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::ThresholdTo2bpp(int nlevels, int cmapflag, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixThresholdTo2bpp(sPix, nlevels, cmapflag);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::ThresholdTo4bpp(int nlevels, int cmapflag, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixThresholdTo4bpp(sPix, nlevels, cmapflag);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

bool PixRaster::ThresholdOn8bpp(int nlevels, int cmapflag, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixThresholdOn8bpp(sPix, nlevels, cmapflag);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::ThresholdGrayArb(const char  *edgevals, int outdepth, int use_average, int setblack, int setwhite, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixThresholdGrayArb(sPix, edgevals, outdepth, use_average, setblack, setwhite);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;
	
}

Buffer<int> PixRaster::MakeGrayQuantIndexTable(int nlevels)
{
	Buffer<int>buf;
	
	l_int32 *ip = makeGrayQuantIndexTable(nlevels);
	if(ip)
	{
		buf.Alloc(nlevels);
		for(int i = 0; i < nlevels; i++)
			buf[i] = ip[i];
		free(ip);
	}
	return buf;

}

Buffer<int> PixRaster::MakeGrayQuantTargetTable(int nlevels, int depth)
{
	Buffer<int>buf;
	
	l_int32 *ip = makeGrayQuantTargetTable(nlevels, depth);
	if(ip)
	{
		buf.Alloc(nlevels);
		for(int i = 0; i < nlevels; i++)
			buf[i] = ip[i];
		free(ip);
	}
	return buf;

}

bool PixRaster::GenerateMaskByBand32(unsigned refval, int delm, int delp, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixGenerateMaskByBand32(sPix, refval, delm, delp);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::GenerateMaskByDiscr32(unsigned refval1, unsigned refval2, int distflag, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixGenerateMaskByDiscr32(sPix, refval1, refval2, distflag);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::GrayQuantFromHisto(int mPage, double minfract, int maxsize, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	mPage = getTruePage(mPage);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *mPix = pixaGetPix(pixa, mPage, L_CLONE);
	PIX *dPix = pixGrayQuantFromHisto(NULL, sPix, mPix, minfract, maxsize);
	pixDestroy(&sPix);
	pixDestroy(&mPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

bool PixRaster::ThresholdToValue(int threshval, int setval, int page)
{
	if(IsEmpty())
		return false;
	page = getTruePage(page);
	
	PIX *sPix = pixaGetPix(pixa, page, L_CLONE);
	PIX *dPix = pixThresholdToValue(NULL, sPix, threshval, setval);
	pixDestroy(&sPix);
	if(!dPix)
		return false;
	AddPIX(dPix, PIXRASTER_CLONE);
	pixDestroy(&dPix);
	SeekPage(PIXRASTER_LASTPAGE);
	return true;

}

END_UPP_NAMESPACE
