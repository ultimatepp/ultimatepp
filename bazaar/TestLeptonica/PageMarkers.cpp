#include "TestLeptonica.h"

static void PageMarkers(Pix & source, PixRaster &pixRaster)
{
	Markers *markers;
	
	pixRaster.Add(source, true);
	pixRaster.Add(source, true);
	
	markers = pixRaster[0].GetMarkers();
	markers->Add(new Marker(Point(100, 100), Point(300, 400)));
	markers->Add(new Marker(Point(0, 500), Point(300, 800)));
	markers = pixRaster[1].GetMarkers();
	markers->Add(new Marker(Point(0, 0), Point(200, 100)));
	markers->Add(new Marker(Point(400, 500), Point(900, 600)));
	
}

void TestLeptonica::onPageMarkers()
{
	String fileName;
	FileSelector fs;

	Pix source;
	
	FileIn s("/home/massimo/tmp/TestLept1.tif");

	// Loads pixraster from source raster
	CHECKR(source.Load(s), "Error loading image");
	s.Close();
	
	// apply line removal algothithm
	pixRaster.Clear();
	PageMarkers(source, pixRaster);
	
	// refresh the PixRasterCtrl control with the new image contents
	pixRasterCtrl.Reload();
	pixRasterCtrl.SetPage(0);
		
}
