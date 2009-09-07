#include "TestLeptonica.h"

static void BaseLine(PixRaster &pixRaster)
{
	CHECKR(pixRaster.DeskewLocal(), "Deskew error");
	Array<int> intArray = pixRaster.FindBaselines();
	CHECKR(intArray.GetCount(), "FindBaselines error");
	PIX *pix = pixRaster;
	int width = pixRaster.GetWidth();
	for(int iLine = 0; iLine < intArray.GetCount(); iLine++)
	{
		int y = intArray[iLine];
		for(int x = 0; x < width; x++)
			pixSetPixel(pix, x, y, 1);
	}
	
}

void TestLeptonica::onBaseLine()
{
	String fileName;
	FileSelector fs;
	PIX *pix;
	
	if(!PromptYesNo(
		"[= [* Text baseline analysis demo]&&"
		"Please select a scanned, 1 bpp text image&"
		"you can take 'keystone.png' from TestLeptonica folder if you like&&"
		"[* CONTINUE ??]]"
	))
		return;
	
	fs.ReadOnlyOption();
	if(fs.ExecuteOpen("Please select image for text baseline analysis:"))
	{
		FileIn s;
		if(!s.Open(~fs))
		{
			PromptOK("Error opening image");
			s.Close();
			return;
		}

		// Loads pixraster from source raster
		CHECKR(pixRaster.Load(s), "Error loading image");
		s.Close();
		
		// apply line removal algothithm
		BaseLine(pixRaster);
		
		// refresh the PixRasterCtrl control with the new image contents
		pixRasterCtrl.Reload();
		pixRasterCtrl.SetPage(0);
		
	}
}
