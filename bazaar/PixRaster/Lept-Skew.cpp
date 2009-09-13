#include "PixRaster.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simple top-level deskew interfaces
Pix Pix::Deskew(int ReductionFactor)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixDeskew(pix, ReductionFactor);
	if(!dPix)
		return Pix();
	return Pix(&dPix);

} // END Pix::Deskew()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::FindSkewAndDeskew(int ReductionFactor, double *skewAngle, double *confidenceFactor)
{
	if(IsEmpty())
		return Pix();
	
	l_float32 dSkew, dConf;
	PIX *dPix = pixFindSkewAndDeskew(pix, ReductionFactor, &dSkew, &dConf);
	if(!dPix)
		return Pix();
	if(skewAngle)
		*skewAngle = dSkew;
	if(confidenceFactor)
		*confidenceFactor = dConf;
	return Pix(&dPix);
	
} // END Pix::FindSkewAndDeskew()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simple top-level skew angle finding interface
bool Pix::FindSkew(double *pangle, double *pconf)
{
	if(IsEmpty())
		return false;

	l_float32 dSkew, dConf;
	int res = pixFindSkew(pix, &dSkew, &dConf);
	if(res)
		return false;
	if(pangle)
		*pangle = dSkew;
	if(pconf)
		*pconf = dConf;
	return true;
	
} // END Pix::FindSkew()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Basic angle-finding functions with all parameters
bool Pix::FindSkewSweep(double *pangle, int reduction, double sweeprange, double sweepdelta)
{
	if(IsEmpty())
		return false;

	l_float32 dSkew;
	int res = pixFindSkewSweep(pix, &dSkew, reduction, sweeprange, sweepdelta);
	if(res)
		return false;
	if(pangle)
		*pangle = dSkew;
	return true;
	
} // END Pix::FindSkewSweep()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Angle-finding functions with all parameters
bool Pix::FindSkewSweepAndSearch(
	double *pangle, double *pconf,
	int redsweep, int redsearch,
	double sweeprange, double sweepdelta,
	double minbsdelta)
{
	if(IsEmpty())
		return false;

	l_float32 dSkew, dConf;
	int res = pixFindSkewSweepAndSearch(pix, &dSkew, &dConf, redsweep, redsearch, sweeprange, sweepdelta, minbsdelta);
	if(res)
		return false;
	if(pangle)
		*pangle = dSkew;
	if(pconf)
		*pconf = dConf;
	return true;
	
} // END Pix::FindSkewSweepAndSearch()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::DeskewLocal()
{
	if(IsEmpty())
		return Pix();

	PIX *dPix = pixDeskewLocal(pix, 0, 0, 0, 0.0, 0.0, 0.0);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
	
} // END Pix::DeskewLocal()

END_UPP_NAMESPACE
