#include "PixRaster.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::CombineMasked(Pix &aPix, Pix &maskPix)
{
	if(IsEmpty())
		return false;
	
	PIX *dPix = pixCopy(NULL, pix);
	int res = pixCombineMasked(dPix, aPix, maskPix);
	if(res)
	{
		pixDestroy(&dPix);
		return Pix();
	}
	return Pix(&dPix);

} // END Pix::CombineMasked()

END_UPP_NAMESPACE
