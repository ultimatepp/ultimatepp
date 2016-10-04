#include "TiffViewCtrl.h"
#include "TiffCtrl.h"

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor
TiffViewCtrl::TiffViewCtrl(TiffCtrl *t)
{
	// connects to associated TiffCtrl
	tiffCtrl = t;
	
	// adds scrollbar
	AddFrame(hScrollBar.Horz());
	AddFrame(vScrollBar.Vert());
	
	hScrollBar <<= THISBACK(OnScroll);
	vScrollBar <<= THISBACK(OnScroll);
	
	// clears cached mages
	CachedImages.Clear();
	
} // END constructor class TiffViewCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
TiffViewCtrl::~TiffViewCtrl()
{
	
} // END destructor class TiffViewCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// paint routine
void TiffViewCtrl::Paint(Draw &d)
{
	// if no image opened, does nothing
	if(!tiffCtrl->IsOpened())
		return;

	// gets the visible area based on scrollars
	int top = vScrollBar.Get();
	int bottom = top + GetSize().cy;
	int left = hScrollBar.Get();
	int right = left + GetSize().cx;
	
	// gets the raster object
	StreamRaster &raster = tiffCtrl->GetRaster();
	
	// scans position vector and redraws only the visible images
	for(int i = 0 ; i < imageRects.GetCount() ; i++)
	{
		Rect &r = imageRects[i];
		if(r.bottom >= top && r.top <= bottom && r.right >= left && r.left <= right)
		{
			// seeks active page
			raster.SeekPage(i);
			
			// get cached image (if any)
			CachedImage &cachedImage = CachedImages[i];
			
			// calculates new sizes
			Size sz = raster.GetSize();
			sz.cx = iscale(sz.cx, imageScale, 100);
			sz.cy = iscale(sz.cy, imageScale, 100);
			
			// checks if cached image s valid
			if(!cachedImage.IsValid(sz))
			{
				// no, rescale and cache it
				ImageEncoder t;
				Rescale(t, sz, raster, raster.GetSize());
				cachedImage.image = t;
				
				// sets cached image as valid
				cachedImage.SetCached(sz);
			}

			// if larger image is wider than view, get pos from horiz scrollbar
			if(hScrollBar.IsVisible())
				d.DrawImage(r.left - left, r.top - top, cachedImage.image);
			// otherwise center it on control
			else
				d.DrawImage((GetSize().cx - r.right)/2, r.top - top, cachedImage.image);
		}
	}

} // END TiffViewCtrl::Paint()

///////////////////////////////////////////////////////////////////////////////////////////////
// scrollbar handler
void TiffViewCtrl::OnScroll(void)
{
	Refresh();
	
} // END TiffViewCtrl::OnScroll()
	
///////////////////////////////////////////////////////////////////////////////////////////////
// handles changes in images or ctrl
void TiffViewCtrl::Layout(void)
{
	// if no image opened, hides scrollbar and return
	if(!tiffCtrl->IsOpened())
	{
		// hides scrollbars
		hScrollBar.Hide();
		vScrollBar.Hide();
		
		// wipe cached images
		CachedImages.Clear();
		return;
	}
	
	// sets up cached images, if needed
	if(!CachedImages.GetCount())
		CachedImages.AddN(tiffCtrl->GetPageCount());
	
	// gets the raster object
	StreamRaster &raster = tiffCtrl->GetRaster();
	
	// calculates max image width and height and total thumbs number and height
	// and fills array of image sizes
	int maxWidth = 0;
	int maxHeight = 0;
	int pageCount = tiffCtrl->GetPageCount();
	imageRects.Clear();
	for(int i = 0 ; i < pageCount ; i++)
	{
		// seeks current page
		raster.SeekPage(i);
		
		// gets page size
		Size sz = raster.GetSize();
		
		// adds size to positions vector and updates maximums
		imageRects.Add(sz);
		if(sz.cx > maxWidth)
			maxWidth = sz.cx;
		if(sz.cy > maxHeight)
			maxHeight = sz.cy;
	}

	// depending on zoom mode, calculates image scale factor
	switch(tiffCtrl->GetZoomType())
	{
		// ZOOM_PAGE - must fit full page inside view
		case TiffCtrl::ZOOM_PAGE :
			imageScale = iscale(GetSize().cy, 100, maxHeight);
			break;
			
		// ZOOM_WIDTH - must fit page width inside view
		case TiffCtrl::ZOOM_WIDTH :
			imageScale = iscale(GetSize().cx, 100, maxWidth);
			break;
			
		// FREE ZOOM - must get zoom factor from main control
		default :
			imageScale = tiffCtrl->GetZoomFactor();
			break;
	} // switch zoomType

	// now calculates real image positions
	int vPos = 0;
	for(int i = 0 ; i < pageCount ; i++)
	{
		Rect r = imageRects[i];
		r.right = iscale(r.right, imageScale, 100);
		r.left = 0;
		r.top = vPos;
		r.bottom = iscale(r.bottom, imageScale, 100);
		r.bottom += vPos;
		vPos = r.bottom + 10;
		imageRects.Set(i, r);
	}

	// and finally, sets up scrollbars and shows them
	if(GetSize().cy <= vPos)
	{
		vScrollBar.SetPage(GetSize().cy);
		vScrollBar.SetTotal(vPos);
		vScrollBar.Show();
	}
	else
	{
		vScrollBar.Hide();
	}
	int scaledMaxWidth = iscale(maxWidth, imageScale, 100);
	if(GetSize().cx <= scaledMaxWidth)
	{
		hScrollBar.SetPage(GetSize().cx);
		hScrollBar.SetTotal(scaledMaxWidth);
		hScrollBar.Show();
	}
	else
	{
		hScrollBar.Hide();
	}
	
	// updates image
	Refresh();
			
} // END TiffViewCtrl::::Layout()
