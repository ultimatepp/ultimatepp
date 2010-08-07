#include "PixRaster.h"

NAMESPACE_UPP

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// top level page segmenting
PixRaster Pix::GetRegionsBinary()
{
	PixRaster pixRaster;
	
	if(IsEmpty())
		return PixRaster();
	
	PIX *ht, *tl, *tb;
	int res = pixGetRegionsBinary(pix, &ht, &tl, &tb, 0);
	if(res)
		return PixRaster();
	Pix htPix(&ht);
	Pix tlPix(&tl);
	Pix tbPix(&tb);
	pixRaster.Add(htPix);
	pixRaster.Add(tlPix);
	pixRaster.Add(tbPix);
	
	// seeks the ht page
	pixRaster.SeekPage(-2);
	
	return pixRaster;
	
} // END Pix::GetRegionsBinary()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// text baseline finding routine
Array<int> Pix::FindBaselines()
{
	NUMA *numa;
	int res;
	l_int32 y;
	Array<int> intArray;

	if(IsEmpty())
		return Array<int>();
	numa = pixFindBaselines(pix, NULL, 0);
	if(!numa)
		return Array<int>();
	for(int iLine = 0; iLine < numaGetCount(numa); iLine++)
	{
		res = numaGetIValue(numa, iLine, &y);
		if(res)
		{
			numaDestroy(&numa);
			return Array<int>();
		}
		intArray.Add(y);
	}
	numaDestroy(&numa);
	return intArray;

} // END Pix::FindBaseLines()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// foreground bounding box locating routine
// scans from a given border through image center to locate
// first non-background pixel.
// scanFlag is one of L_FROM_LEFT, L_FROM_RIGHT, L_FROM_TOP, L_FROM_BOTTOM
// returns true on success, false otherwise
bool Pix::ScanForForeground(int &loc, enum ScanModes scanMode)
{
	int res = pixScanForForeground(pix, NULL, (l_int32)scanMode, (l_int32 *)&loc);
	return (res == 0);
}

END_UPP_NAMESPACE
