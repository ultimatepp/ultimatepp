#include "PixRaster.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// toplevel scale function
Pix Pix::Scale(double scalex, double scaley)
{
	if(IsEmpty())
		return Pix();
	PIX *dPix = pixScale(pix, (l_float32)scalex, (l_float32)scaley);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
}

Pix Pix::Fit(int width, int height, bool keepRatio)
{
	// gets original sizes
	int w = GetWidth();
	int h = GetHeight();
	
	// gets scale factor in both directions
	double xScale = (double)width / (double)w;
	double yScale = (double)height / (double)h;
	if(keepRatio)
	{
		xScale = min(xScale, yScale);
		yScale = xScale;
	}
	return Scale(xScale, yScale);
}

END_UPP_NAMESPACE
