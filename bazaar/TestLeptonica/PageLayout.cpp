#include "TestLeptonica.h"

static void PageLayout(PixRaster &pixRaster)
{
	CHECKR(pixRaster.GetRegionsBinary(), "Error getting page layout");	
}

void TestLeptonica::onPageLayout()
{
	String fileName;
	FileSelector fs;
	PIX *pix;
	
	if(!PromptYesNo(
		"[= [* Page layout analysis demo]&&"
		"Please select a 1 bpp scanned image with mixed text and graphics&"
		"you can take one from TestLeptonica folder if you like&&"
		"[* CONTINUE ??]]"
	))
		return;
	
	fs.ReadOnlyOption();
	if(fs.ExecuteOpen("Please select image for page layout analysis:"))
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
		PageLayout(pixRaster);
		
		// refresh the PixRasterCtrl control with the new image contents
		pixRasterCtrl.Reload();
		pixRasterCtrl.SetPage(0);
		
	}
}
