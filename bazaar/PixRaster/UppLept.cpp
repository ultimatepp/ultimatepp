#include "PixRaster.h"

#include <Draw/Draw.h>

NAMESPACE_UPP

///////////////////////////////////////////////////////////////////////////////
// All Leptonica routines operates on Pix data structures; here 2 conversion
// routines are provided to go to/from Upp Images
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Creation of Pix image from an Upp Image
// As Upp Image is in RGBA format, it's simply converted
// to an RGB Leptonica Pix; alpha channel is discarded
PIX *Image2Pix(const Image &img)
{
	PIX *pix;
	
	// Get image sizes
	int width = img.GetWidth();
	int height = img.GetHeight();
	
	// Get image sizes in dots
	int sizeX = img.GetDots().cx;
	int sizeY = img.GetDots().cy;
	
	// Allocates the Pix structure
	pix = pixCreateNoInit(width, height, 32);
	if(!pix)
		return NULL;
	
	// sets image resolution (if any)
	// don't know if it's correct, yet....
	pixSetResolution(pix, sizeX / width, sizeY / height);
	
	// transfer the image bytes
	memcpy(pixGetData(pix), ~img, 4*width*height);
	
	return pix;
	
} // END Image2Pix()

///////////////////////////////////////////////////////////////////////////////
// Creation of Upp Image from Pix
// Pix is first converted to an RGB format, then to Upp Image
Image Pix2Image(PIX * pix)
{
	bool destroyCopy = false;

	// colormapped and non-32 bit depth pixs need conversion before
	if(pixGetColormap(pix) || pixGetDepth(pix) != 32)
	{
		// we need to destroy converted pix on exit
		destroyCopy = true;

		// convert pix to 32 bit format
		pix = pixConvertTo32(pix);
		if(!pix)
			return Image();
	}

	// just copy data buffer to a new imagebuffer
	int width = pixGetWidth(pix);
	int height = pixGetHeight(pix);
	ImageBuffer img(width, height);
	memcpy(~img, pixGetData(pix), 4*width*height);
	
	// sets image type to opaque
	// not sure if we must set alpha bit to 255......
	img.SetKind(IMAGE_OPAQUE);
	
	if(destroyCopy)
		pixDestroy(&pix);
	
	return img;
}

END_UPP_NAMESPACE
