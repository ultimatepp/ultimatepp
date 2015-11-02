#include "TestLeptonica.h"

static void BaseLine(Pix & source, PixRaster &pixRaster)
{
	pixRaster.Add(source);
	
	Pix deskewed = source.DeskewLocal();
	CHECKR(deskewed, "Deskew error");
	pixRaster.Add(deskewed);

	Array<int> intArray = deskewed.FindBaselines();
	CHECKR(intArray.GetCount(), "FindBaselines error");

	// deep copy source
	Pix copied(deskewed, 1);
	
	PIX *pix = copied;
	int width = copied.GetWidth();
	for(int iLine = 0; iLine < intArray.GetCount(); iLine++)
	{
		int y = intArray[iLine];
		for(int x = 0; x < width; x++)
			pixSetPixel(pix, x, y, 1);
	}
	pixRaster.Add(copied);
	
}

void TestLeptonica::onBaseLine()
{
	String fileName;
	FileSelector fs;

	Pix source;
	
	if(!PromptYesNo(
		"[= [* Text baseline analysis demo]&&"
		"Please select a scanned, 1 bpp text image&"
		"you can take 'keystone.png' from TestLeptonica folder if you like&&"
		"[* CONTINUE ??]]"
	))
		return;
	
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
		CHECKR(source.Load(s), "Error loading image");
		s.Close();
		
		// apply line removal algothithm
		pixRaster.Clear();
		BaseLine(source, pixRaster);
		
		// refresh the PixRasterCtrl control with the new image contents
		pixRasterCtrl.Reload();
		pixRasterCtrl.SetPage(0);
		
	}
}
