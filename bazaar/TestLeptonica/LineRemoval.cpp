#include "TestLeptonica.h"

static void RemoveLines(Pix &source, PixRaster &dest)
{
		dest.Add(source);
		
		// threshold the image
		Pix thresholded = source.ThresholdToBinary(170);
		CHECKR(thresholded, "Error thresholding the image");
		dest.Add(thresholded);
		
		// find image skew of thresholded image
		double angle;
	    CHECKR(thresholded.FindSkew(&angle, NULL), "Error finding skew value");
	    
	    // rotate original image using found threshold value
	    double deg2rad = M_PI / 180.0;
		Pix rotated = source.RotateAMGray(deg2rad * angle, 255);
		CHECKR(rotated, "Error rotating image");
		dest.Add(rotated);

		// extract the lines to be removed
		Pix closed = rotated.CloseGray(51, 1);
		CHECKR(closed, "CloseGray error");
		dest.Add(closed);

		// solidify the lines to be removed
		Pix eroded = closed.ErodeGray(1, 5);
		CHECKR(eroded, "ErodeGray error");
		dest.Add(eroded);
		
		// clean the background of those lines
		Pix thresh2 = eroded.ThresholdToValue(210, 255);
		CHECKR(thresh2, "ThresholdToValue error");
		dest.Add(thresh2);
		
		Pix thresh3 = thresh2.ThresholdToValue(200, 0);
		CHECKR(thresh3, "ThresholdToValue error");
		dest.Add(thresh3);
		
		// get paint-through mask for changed pixels
		Pix thresh4 = thresh3.ThresholdToBinary(210);
		CHECKR(thresh4, "ThresholdToBinary error");
		dest.Add(thresh4);
		
		// invert the gray line image in order to whiten
		// the lines in the origina image
		Pix inverted = thresh3.Invert();
		CHECKR(inverted, "Invert error");
		dest.Add(inverted);
		
		// add inverted line image to original (deskewed) one
		// so all horizontal black lines will be erased
		// unrderlying parts of image are erase too
		Pix grayAdded = rotated.AddGray(inverted);
		CHECKR(grayAdded, "AddGray error");
		dest.Add(grayAdded);

		// "opens" the image with wite stripes, so the stripes
		// gets closed my surrounding pixels. As this changes
		// also the rest of image, we will use that one just for
		// filling the stripes. That's done by aid of mask created before...
		Pix opened2 = grayAdded.OpenGray(1, 9);
		CHECKR(opened2, "OpenGray error");
		dest.Add(opened2);

		// now to get the final result, we must combine the image with black stripes
		// removed with the one with grays opened to fill the stripes; we use the mask
		// created before to fill just the stripes and leave the rest of image unchanged
		Pix final = grayAdded.CombineMasked(opened2, thresh4);
		CHECKR(final, "CombineMasked error");
		dest.Add(final);
}

void TestLeptonica::onLineRemoval()
{
	String fileName;
	FileSelector fs;
	
	Pix source;
	
	if(!PromptYesNo(
		"[= [* Line removal demo]&&"
		"Please select the 'dave-orig.png' image on TestLeptonica folder&"
		"or some equivalent GrayScale image with horizontal stripes on it&&"
		"[* CONTINUE ??]]"
	))
		return;
	
	if(fs.ExecuteOpen("Please select image for line removal:"))
	{
		FileIn s;
		if(!s.Open(~fs))
		{
			PromptOK("Error opening image");
			s.Close();
			return;
		}

		// Loads pixraster from source raster
		CHECKR(source.Load(s), "Error loading image");
		s.Close();
		
		// apply line removal algothithm
		pixRaster.Clear();
		RemoveLines(source, pixRaster);
		
		// refresh the PixRasterCtrl control with the new image contents
		pixRasterCtrl.Reload();
		pixRasterCtrl.SetPage(0);
		
	}
}
