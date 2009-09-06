#include "PixRasterThumbsCtrl.h"
#include "PixRasterCtrl.h"

// Scale factor between pane size and thumbs sizes
const int	THUMBS_HSIZE_MUL	=	9;
const int	THUMBS_HSIZE_DIV	=	10;

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor
PixRasterThumbsCtrl::PixRasterThumbsCtrl(PixRasterCtrl *t) : PixRasterBaseCtrl(t, false, true)
{
	
} // END constructor class PixRasterThumbsCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
PixRasterThumbsCtrl::~PixRasterThumbsCtrl()
{
	
} // END destructor class PixRasterThumbsCtrl
		
///////////////////////////////////////////////////////////////////////////////////////////////
// calculates image scale factor
int PixRasterThumbsCtrl::CalcScale(int imScale, int rasterWidth, int maxPageHeight)
{
	// calculates scale factor based on max thumb width
	// slightly smaller than ctrl width
	int maxScaledWidth = iscale(GetSize().cx, THUMBS_HSIZE_MUL, THUMBS_HSIZE_DIV);
	return iscale(maxScaledWidth, 1000, rasterWidth);

} // END PixRasterThumbsCtrl::CalcScale()

///////////////////////////////////////////////////////////////////////////////////////////////
// left click handler --> shows requested page in viewer
void PixRasterThumbsCtrl::LeftDown(Point p, dword keyflags)
{
	// gets point position in raster coordinates
	int clickPos = vScrollBar.Get() + iscale(p.y, vScrollBar.GetPage(), GetView().GetHeight());
	clickPos = iscale(clickPos, 1000, imageScale);
	
	// gets gap size between pages
	int gapSize = iscale(10, 1000, imageScale);
	
	// gets the PixRaster object
	PixRaster *pixRaster = pixRasterCtrl->GetPixRaster();

	// iterates through page positions to find the requested one
	int top = 0;
	for(int iPage = 0; iPage < pixRaster->GetPageCount(); iPage++)
	{
		int bottom = top + pixRaster->GetHeight(iPage);
		
		if(clickPos >= top && clickPos <= bottom)
		{
			pixRasterCtrl->SetPage(iPage);
			break;
		}
		top += pixRaster->GetHeight(iPage) + gapSize;
	}
	
} // END PixRasterThumbsCtrl::LeftDown()
