#include "TestLeptonica.h"

static void PageLayout(Pix &source, PixRaster &pixRaster)
{
	pixRaster.Add(source);
	PixRaster regions = source.GetRegionsBinary();
	CHECKR(regions, "Error getting page layout");
	pixRaster.Add(regions);
}

void TestLeptonica::onPageLayout()
{
	String fileName;
	FileSelector fs;
	
	Pix source;
	
	if(!PromptYesNo(
		"[= [* Page layout analysis demo]&&"
		"Please select a 1 bpp scanned image with mixed text and graphics&"
		"you can take one from TestLeptonica folder if you like&&"
		"[* CONTINUE ??]]"
	))
		return;
	
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
		CHECKR(source.Load(s), "Error loading image");
		s.Close();
		
		// apply line removal algothithm
		pixRaster.Clear();
		PageLayout(source, pixRaster);
		
		// refresh the PixRasterCtrl control with the new image contents
		pixRasterCtrl.Reload();
		pixRasterCtrl.SetPage(0);
		
	}
}
