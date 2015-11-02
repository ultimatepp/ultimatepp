#include "PixRaster.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Pix Pix::CombineMasked(Pix &aPix, Pix &maskPix)
{
	if(IsEmpty())
		return Pix();
	
	PIX *dPix = pixCopy(NULL, pix);
	int res = pixCombineMasked(dPix, aPix, maskPix);
	if(res)
	{
		pixDestroy(&dPix);
		return Pix();
	}
	return Pix(&dPix);

} // END Pix::CombineMasked()

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// extracts a rectangular area from PIX
Pix Pix::ClipRectangle(Rect r)
{
	if(IsEmpty())
		return Pix();

	BOX box;
	box.x = r.left;
	box.y = r.top;
	box.w = r.right - r.left;
	box.h = r.bottom - r.top;

	PIX *dPix = pixClipRectangle(pix, &box, NULL);
	if(!dPix)
		return Pix();
	return Pix(&dPix);
}

END_UPP_NAMESPACE
