#include "PixRasterViewCtrl.h"
#include "PixRasterCtrl.h"

NAMESPACE_UPP

#define BackColor SColorFace()

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor
PixRasterViewCtrl::PixRasterViewCtrl(PixRasterCtrl *t) : PixRasterBaseCtrl(t, true, true)
{
	// sets zoom mode to full page
	zoomType = ZOOM_WIDTH;
	
} // END constructor class PixRasterViewCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
PixRasterViewCtrl::~PixRasterViewCtrl()
{
} // END destructor class PixRasterViewCtrl


///////////////////////////////////////////////////////////////////////////////////////////////
// calculates image scale factor
int PixRasterViewCtrl::CalcScale(int imScale, int rasterWidth, int maxPageHeight)
{
	// depending on zoom mode, calculates image scale factor
	int imageScale;
	switch(zoomType)
	{
		// ZOOM_PAGE - must fit full page inside view
		case ZOOM_PAGE :
			imageScale = iscale(GetSize().cy, 1000, maxPageHeight);
			break;
			
		// ZOOM_WIDTH - must fit page width inside view
		case ZOOM_WIDTH :
			imageScale = iscale(GetSize().cx, 1000, rasterWidth);
			break;
			
		// FREE ZOOM - leaves imageScale as it is
		default :
			imageScale = imScale;
			break;
	} // switch zoomType
	
	return imageScale;

} // END PixRasterViewCtrl::CalcScale()

///////////////////////////////////////////////////////////////////////////////////////////////
// mouse wheel handler
void PixRasterViewCtrl::MouseWheel(Point p, int zdelta, dword keyflags)
{
	// if shift pressed, do a zoom
	if(keyflags & K_SHIFT)
	{
		if(zdelta < 0)
			Zoom(false);
		else		
			Zoom(true);

		Refresh();
	}
	else
		PixRasterBaseCtrl::MouseWheel(p, zdelta, keyflags);
	
} // END PixRasterViewCtrl::MouseWheel()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets zoom to page width
void PixRasterViewCtrl::ZoomWidth()
{
	// sets mode to page width
	zoomType = ZOOM_WIDTH;
	
	// signals layout change
	Layout();
	
} // END PixRasterViewCtrl::ZoomWidth()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets zoom to full page
void PixRasterViewCtrl::ZoomPage()
{
	// sets mode to page height
	zoomType = ZOOM_PAGE;

	// signals layout change
	Layout();
	
} // END PixRasterViewCtrl::ZoomPage()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets the zoom to an arbitrary value - in percentage from original size
void PixRasterViewCtrl::Zoom(int z)
{
	// checks and adjusts zoom factor
	if(z <= 0) z = 1;
	if(z > 300) z = 300;
	
	// sets it
	zoomType = ZOOM_NORMAL;
	imageScale = 10 * z ;

	// signals layout change
	Layout();
	
} // END PixRasterViewCtrl::Zoom()

///////////////////////////////////////////////////////////////////////////////////////////////
// zooms inside/outside by fixed factors
void PixRasterViewCtrl::Zoom(bool in)
{
	static const int zooms[] = { 1, 2, 5, 10, 15, 20, 25, 50, 75, 100, 150, 200, 250, 300 };
	static const int nZooms = sizeof(zooms) / sizeof(int);

	int zoomFactor = imageScale / 10;
	bool found = false;
	if(in)
	{
		for(int i = 0; i < nZooms; i++)
		{
			if(zooms[i] > zoomFactor)
			{
				zoomFactor = zooms[i];
				found = true;
				break;
			}
		}
		if(!found)
			zoomFactor = zooms[nZooms-1];
	}
	else
	{
		for(int i = nZooms-1; i >=0; i--)
		{
			if(zooms[i] < zoomFactor)
			{
				zoomFactor = zooms[i];
				found = true;
				break;
			}
		}
		if(!found)
			zoomFactor = zooms[0];
	}
	
	Zoom(zoomFactor);
			   
} // END PixRasterViewCtrl::Zoom()

///////////////////////////////////////////////////////////////////////////////////////////////
// gets zoom factor of current view
int PixRasterViewCtrl::GetZoomFactor(void)
{
	// imageScale is given in 1/1000
	return imageScale / 10;
	
} // END PixRasterViewCtrl::GetZoomFactor()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets current page
void PixRasterViewCtrl::SetPage(int page)
{
	// gets the PixRaster object
	PixBase *pixBase = pixRasterCtrl->GetPixBase();

	// calculate gaps to be exactly 10 pixels in every zoom factor
	int gapSize = iscale(10, 1000, imageScale);
	
	// iterate thru pages to find requested page position
	int rasterPos = 0;
	int pageCount = pixBase->GetPageCount();
	if(page > pageCount)
		page = pageCount;
	for(int i = 0 ; i < page ; i++)
	{
		// gets page size
		Size sz = pixBase->GetSizeEx(i);
		
		rasterPos += sz.cy + gapSize;
	}
	
	// sets vertical scrollbar to requested position
	vScrollBar.Set(iscale(rasterPos, imageScale, 1000));
	
	// updates layout
	Layout();
	
} // END PixRasterViewCtrl::SetPage()

END_UPP_NAMESPACE
