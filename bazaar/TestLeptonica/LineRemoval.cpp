#include "TestLeptonica.h"

#include <plugin/tif/tif.h>

void TestLeptonica::onLineRemoval()
{
	String fileName;
	FileSelector fs;
	PIX *pix;
	
	if(!PromptYesNo(
		"[= [* Line removal demo]&&"
		"Please select the 'dave-orig.png' image on TestLeptonica folder&"
		"or some equivalent GrayScale image with horizontal stripes on it&&"
		"[* CONTINUE ??]]"
	))
		return;
	
	fs.ReadOnlyOption();
	if(fs.ExecuteOpen("Please select image for line removal:"))
	{
		FileIn s;
		if(!s.Open(~fs))
		{
			PromptOK("Error opening image");
			return;
		}
		One<StreamRaster>streamRaster = StreamRaster::OpenAny(s);

		// Loads pixraster from source raster
		pixRaster.Load(*streamRaster);
		
		// gets source image page
		int source = pixRaster.GetActivePage();

		// threshold the image
		CHECKR(pixRaster.ThresholdToBinary(170), "Error thresholding the image");
		int thresholded = pixRaster.GetActivePage();
		
		// find image skew of thresholded image
		double angle;
	    CHECKR(pixRaster.FindSkew(&angle, NULL, thresholded), "Error finding skew value");
	    
	    // rotate original image using found threshold value
	    double deg2rad = M_PI / 180.0;
		CHECKR(pixRaster.RotateAMGray(deg2rad * angle, 255, source), "Error rotating image");
		int rotated = pixRaster.GetActivePage();

		// extract the lines to be removed
		CHECKR(pixRaster.CloseGray(51, 1, rotated), "CloseGray error");
		int closed = pixRaster.GetActivePage();

		// solidify the lines to be removed
		CHECKR(pixRaster.ErodeGray(1, 5, closed), "ErodeGray error");
		int eroded = pixRaster.GetActivePage();
		
		// clean the background of those lines
		CHECKR(pixRaster.ThresholdToValue(210, 255, eroded), "ThresholdToValue error");
		int thresh2 = pixRaster.GetActivePage();
		
		CHECKR(pixRaster.ThresholdToValue(200, 0, thresh2), "ThresholdToValue error");
		int thresh3 = pixRaster.GetActivePage();
		
		// get paint-through mask for changed pixels
		CHECKR(pixRaster.ThresholdToBinary(210, thresh3), "ThresholdToBinary error");
		int thresh4 = pixRaster.GetActivePage();
		
		// invert the gray line image in order to whiten
		// the lines in the origina image
		CHECKR(pixRaster.Invert(thresh3), "Invert error");
		int inverted = pixRaster.GetActivePage();
		
		// add inverted line image to original (deskewed) one
		// so all horizontal black lines will be erased
		// unrderlying parts of image are erase too
		CHECKR(pixRaster.AddGray(rotated, inverted), "AddGray error");
		int grayAdded = pixRaster.GetActivePage();

		// "opens" the image with wite stripes, so the stripes
		// gets closed my surrounding pixels. As this changes
		// also the rest of image, we will use that one just for
		// filling the stripes. That's done by aid of mask created before...
		CHECKR(pixRaster.OpenGray(1, 9, grayAdded), "OpenGray error");
		int opened2 = pixRaster.GetActivePage();

		// now to get the final result, we must combine the image with black stripes
		// removed with the one with grays opened to fill the stripes; we use the mask
		// created before to fill just the stripes and leave the rest of image unchanged
		CHECKR(pixRaster.CombineMasked(grayAdded, opened2, thresh4), "CombineMasked error");
		int final = pixRaster.GetActivePage();
		
		// refresh the PixRasterCtrl control with the new image contents
		pixRasterCtrl.Reload();
		pixRasterCtrl.SetPage(0);
		
	}
}
