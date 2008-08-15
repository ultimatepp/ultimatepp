#include "TiffThumbsCtrl.h"
#include "TiffCtrl.h"

// Scale factor between pane size and thumbs sizes
const int	THUMBS_HSIZE_MUL	=	9;
const int	THUMBS_HSIZE_DIV	=	10;

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor
TiffThumbsCtrl::TiffThumbsCtrl(TiffCtrl *t)
{
	// connects to associated TiffCtrl
	tiffCtrl = t;
	
	// adds scrollbar
	AddFrame(scrollBar);
	
	scrollBar <<= THISBACK(OnScroll);
	
} // END constructor class TiffThumbsCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
TiffThumbsCtrl::~TiffThumbsCtrl()
{
	
} // END destructor class TiffThumbsCtrl
		
///////////////////////////////////////////////////////////////////////////////////////////////
// paint routine
void TiffThumbsCtrl::Paint(Draw &d)
{
	// if no image opened, does nothing
	if(!tiffCtrl->IsOpened())
		return;

	// gets the visible area based on scroll
	int top = scrollBar.Get();
	int bottom = top + GetSize().cy;
	
	// gets the raster object
	StreamRaster &raster = tiffCtrl->GetRaster();
	
	// scans position vector and redraws only the visible thumbs
	for(int i = 0 ; i < thumbsRects.GetCount() ; i++)
	{
		Rect &r = thumbsRects[i];
		if(r.bottom >= top && r.top <= bottom)
		{
			// seeks active page
			raster.SeekPage(i);
			
			// rescale the image
			ImageEncoder t;
			Size sz = raster.GetSize();
			sz.cx = iscale(sz.cx, thumbsScale, 100);
			sz.cy = iscale(sz.cy, thumbsScale, 100);
			Rescale(t, sz, raster, raster.GetSize());
			d.DrawImage(r.left, r.top - top, t);
		}
	}
		
} // END TiffThumbCtrl::Paint()
	
///////////////////////////////////////////////////////////////////////////////////////////////
// scrollbar handler
void TiffThumbsCtrl::OnScroll(void)
{
	Refresh();
	
} // END TiffThumbCtrl::OnScroll()
	
///////////////////////////////////////////////////////////////////////////////////////////////
// handles changes in images or ctrl
void TiffThumbsCtrl::Layout(void)
{
	// if no image opened, hides scrollbar and return
	if(!tiffCtrl->IsOpened())
	{
		scrollBar.Hide();
		return;
	}
	
	if(!tiffCtrl)
		return;
	
	// gets the raster object
	StreamRaster &raster = tiffCtrl->GetRaster();
	
	// calculates max image width and total thumbs number and height
	// and fills array of image sizes
	int maxWidth = 0;
	int pageCount = tiffCtrl->GetPageCount();
	thumbsRects.Clear();
	for(int i = 0 ; i < pageCount ; i++)
	{
		// seeks current page
		raster.SeekPage(i);
		
		// gets page size
		Size sz = raster.GetSize();
		
		// adds size to positions vector and updates maximums
		thumbsRects.Add(sz);
		if(sz.cx > maxWidth)
			maxWidth = sz.cx;
	}
	
	// calculates scale factor based on max thumb width
	// slightly smaller than ctrl width
	int maxScaledWidth = iscale(GetSize().cx, THUMBS_HSIZE_MUL, THUMBS_HSIZE_DIV);
	thumbsScale = iscale(maxScaledWidth, 100, maxWidth);
	
	// now calculates real positions
	int vPos = 0;
	for(int i = 0 ; i < pageCount ; i++)
	{
		Rect r = thumbsRects[i];
		r.right = iscale(r.right, thumbsScale, 100);
		r.left = (GetSize().cx - r.right) / 2;
		r.top = vPos;
		r.bottom = iscale(r.bottom, thumbsScale, 100);
		r.bottom += vPos;
		vPos = r.bottom + 10;
		thumbsRects.Set(i, r);
	}
	
	// and finally, sets up scrollbar and shows it
	if(GetSize().cy <= vPos)
	{
		scrollBar.SetPage(GetSize().cy);
		scrollBar.SetTotal(vPos /* - GetSize().cy */);
		scrollBar.Show();
	}
	else
	{
		scrollBar.Hide();
	}
	
} // END TiffThumbsCtrl::Layout()
