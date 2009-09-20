#include "PixRasterThumbsCtrl.h"
#include "PixRasterCtrl.h"

NAMESPACE_UPP

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
	clickPos = ScaleToPage(clickPos);
	
	// gets gap size between pages
	int gapSize = ScaleToPage(10);
	
	// gets the PixRaster object
	PixBase *pixBase = pixRasterCtrl->GetPixBase();

	// iterates through page positions to find the requested one
	int top = 0;
	for(int iPage = 0; iPage < pixBase->GetPageCount(); iPage++)
	{
		int bottom = top + pixBase->GetHeightEx(iPage);
		
		if(clickPos >= top && clickPos <= bottom)
		{
			pixRasterCtrl->SetPage(iPage);
			break;
		}
		top += pixBase->GetHeightEx(iPage) + gapSize;
	}
	
	// calls base class handler -- used for dragging
	PixRasterBaseCtrl::LeftDown(p, keyflags);
	
} // END PixRasterThumbsCtrl::LeftDown()

END_UPP_NAMESPACE
