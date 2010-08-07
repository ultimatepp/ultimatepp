#include "PixRaster.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::DitherToBinary()
{
	if(IsEmpty())
		return Pix();
	
	PIX *dPix = pixDitherToBinary(pix);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::DitherToBinary()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::DitherToBinarySpec(int lowerclip, int upperclip)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixDitherToBinarySpec(pix, lowerclip, upperclip);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::DitherToBinarySpec()

// threshold the image -- warning, operates ONLY on grayscale pixmaps
// original image is unchanged - a new modified image at raster's end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::ThresholdToBinary(int threshold)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixThresholdToBinary(pix, threshold);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::ThresholdToBinary()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::VarThresholdToBinary(Pix &thresholdPix)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixVarThresholdToBinary(pix, thresholdPix);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::VarThresholdToBinary()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::DitherToBinaryLUT(int lowerclip, int upperclip)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixDitherToBinaryLUT(pix, lowerclip, upperclip);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::DitherToBinaryLUT()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::GenerateMaskByValue(int val, bool useCMap)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixGenerateMaskByValue(pix, val, useCMap ? 1 : 0);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::GenerateMaskByValue()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::GenerateMaskByBand(int lower, int upper, int inband, bool useCMap)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixGenerateMaskByBand(pix, lower, upper, inband, useCMap ? 1 : 0);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::GenerateMaskByBand()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::DitherTo2bpp(int cmapflag)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixDitherTo2bpp(pix, cmapflag);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::DitherTo2bpp()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::DitherTo2bppSpec(int lowerclip, int upperclip, int cmapflag)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixDitherTo2bppSpec(pix, lowerclip, upperclip, cmapflag);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::DitherTo2bppSpec()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::ThresholdTo2bpp(int nlevels, int cmapflag)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixThresholdTo2bpp(pix, nlevels, cmapflag);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::ThresholdTo2bpp()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::ThresholdTo4bpp(int nlevels, int cmapflag)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixThresholdTo4bpp(pix, nlevels, cmapflag);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::ThresholdTo4bpp()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::ThresholdOn8bpp(int nlevels, int cmapflag)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixThresholdOn8bpp(pix, nlevels, cmapflag);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::ThresholdOn8bpp()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::ThresholdGrayArb(const char  *edgevals, int outdepth, int use_average, int setblack, int setwhite)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixThresholdGrayArb(pix, edgevals, outdepth, use_average, setblack, setwhite);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::ThresholdGrayArb()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Buffer<int> Pix::MakeGrayQuantIndexTable(int nlevels)
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

} // END Pix::MakeGrayQuantIndexTable()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Buffer<int> Pix::MakeGrayQuantTargetTable(int nlevels, int depth)
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

} // END Pix::MakeGrayQuantTargetTable()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::GenerateMaskByBand32(unsigned refval, int delm, int delp)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixGenerateMaskByBand32(pix, refval, delm, delp);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::GenerateMaskByBand32()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::GenerateMaskByDiscr32(unsigned refval1, unsigned refval2, int distflag)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixGenerateMaskByDiscr32(pix, refval1, refval2, distflag);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::GenerateMaskByDiscr32()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::GrayQuantFromHisto(Pix &mPix, double minfract, int maxsize)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixGrayQuantFromHisto(NULL, pix, mPix, minfract, maxsize);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::GrayQuantFromHisto()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::ThresholdToValue(int threshval, int setval)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixThresholdToValue(NULL, pix, threshval, setval);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::ThresholdToValue()

END_UPP_NAMESPACE
