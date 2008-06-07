#include "RasterThumbsCtrl.h"
#include "RasterCtrl.h"

// Scale factor between pane size and thumbs sizes
const int	THUMBS_HSIZE_MUL	=	9;
const int	THUMBS_HSIZE_DIV	=	10;

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor
RasterThumbsCtrl::RasterThumbsCtrl(RasterCtrl *t) : RasterBaseCtrl(t, false, true)
{
	
} // END constructor class RasterThumbsCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
RasterThumbsCtrl::~RasterThumbsCtrl()
{
	
} // END destructor class RasterThumbsCtrl
		
///////////////////////////////////////////////////////////////////////////////////////////////
// calculates image scale factor
int RasterThumbsCtrl::CalcScale(int imScale, int rasterWidth, int maxPageHeight)
{
	// calculates scale factor based on max thumb width
	// slightly smaller than ctrl width
	int maxScaledWidth = iscale(GetSize().cx, THUMBS_HSIZE_MUL, THUMBS_HSIZE_DIV);
	return iscale(maxScaledWidth, 1000, rasterWidth);

} // END RasterThumbsCtrl::CalcScale()

///////////////////////////////////////////////////////////////////////////////////////////////
// left click handler --> shows requested page in viewer
void RasterThumbsCtrl::LeftDown(Point p, dword keyflags)
{
	// gets point position in raster coordinates
	int clickPos = vScrollBar.Get() + iscale(p.y, vScrollBar.GetPage(), GetView().GetHeight());
	clickPos = iscale(clickPos, 1000, imageScale);
	
	// gets gap size between pages
	int gapSize = iscale(10, 1000, imageScale);
	
	// gets the page array
	Array<MemoryRaster> &pages = rasterCtrl->GetPages();

	// iterates through page positions to find the requested one
	int top = 0;
	for(int iPage = 0; iPage < pages.GetCount(); iPage++)
	{
		MemoryRaster &page = pages[iPage];
		int bottom = top + page.GetHeight();
		
		if(clickPos >= top && clickPos <= bottom)
		{
			rasterCtrl->SetPage(iPage);
			break;
		}
		top += page.GetHeight() + gapSize;
	}
} // END RasterThumbsCtrl::LeftDown()
