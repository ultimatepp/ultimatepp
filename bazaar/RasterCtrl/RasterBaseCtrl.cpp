#include "RasterBaseCtrl.h"
#include "RasterCtrl.h"

#define BackColor SColorFace()

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor
RasterBaseCtrl::RasterBaseCtrl(RasterCtrl *t, bool hScroll, bool vScroll)
{
	// connects to associated RasterCtrl
	rasterCtrl = t;
	
	// whether control has scrollbars
	hasHScrollBar = hScroll;
	hasVScrollBar = vScroll;
	
	// adds scrollbar
	if(hasHScrollBar)
	{
		AddFrame(hScrollBar.Horz());
		hScrollBar <<= THISBACK(OnScroll);
	}
	if(hasVScrollBar)
	{
		AddFrame(vScrollBar.Vert());
		vScrollBar <<= THISBACK(OnScroll);
	}
	
	// marks cache as invalid
	imageCache.SetValid(false);
	
} // END constructor class RasterBaseCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
RasterBaseCtrl::~RasterBaseCtrl()
{
} // END destructor class RasterBaseCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// middle mouse button pan handlers
void RasterBaseCtrl::MiddleDown(Point p, dword keyflags)
{
	panPoint = p;
	panHScroll = hScrollBar.Get();
	panVScroll = vScrollBar.Get();
	
} // END RasterBaseCtrl::MiddleDown()

void RasterBaseCtrl::MouseMove(Point p, dword keyflags)
{
	// reacts only to moves with middle button pressed
	if(!(keyflags & K_MOUSEMIDDLE))
		return;
	
	// gets distance between current and pan point
	int dx = panPoint.x - p.x + panHScroll;
	int dy = panPoint.y - p.y + panVScroll;

	// gets max scrolling values	
	int hMax = hScrollBar.GetTotal();
	int vMax = vScrollBar.GetTotal();
	
	// sets new pan position
	if(dx < 0) dx = 0;
	if(dx > hMax) dx = hMax;
	if(dy < 0) dy = 0;
	if(dy > vMax) dy = vMax;
	hScrollBar.Set(dx);
	vScrollBar.Set(dy);

	Refresh();
	
} // END RasterBaseCtrl::MouseMove()

Image RasterBaseCtrl::CursorImage(Point p, dword keyflags)
{
	if(keyflags & K_MOUSEMIDDLE)
		return Image::Hand();
	else
		return Image::Arrow();
	
}

///////////////////////////////////////////////////////////////////////////////////////////////
// mouse wheel handler
void RasterBaseCtrl::MouseWheel(Point p, int zdelta, dword keyflags)
{
	int max = vScrollBar.GetTotal();
	int step = max / 100;
	if(step <= 0)
		step = 1;
	int newPos = vScrollBar.Get();
	if(zdelta < 0)
		newPos += step;
	else
		newPos -= step;
	if(newPos > max)
		newPos = max;
	if(newPos < 0)
		newPos = 0;
	vScrollBar.Set(newPos);

	Refresh();
	
} // END RasterBaseCtrl::MouseWheel()

///////////////////////////////////////////////////////////////////////////////////////////////
// repaint images on image cache
void RasterBaseCtrl::PaintCache()
{
	Array<Rect> rects;
	
	// backups old imageCache positions inside the full tiff image
	int oldCacheLeft = cacheLeft;
	int oldCacheTop = cacheTop;
	
	// calculate new imageCache position inside the full tiff image
	if(hScrollBar.IsVisible())
		cacheLeft = hScrollBar.Get();
	else
		cacheLeft = 0;
	if(vScrollBar.IsVisible())
		cacheTop = vScrollBar.Get();
	else
		cacheTop = 0;

	// if cache is not valid OR scrolled out a full window, gets full cache area
	if(
		!imageCache.IsValid() || 
		abs(oldCacheLeft-cacheLeft) >= imageCache.GetWidth() ||
		abs(oldCacheTop-cacheTop) >= imageCache.GetHeight()
	)
	{
		rects.Add(Rect(imageCache.GetSize()));
		imageCache.SetValid(true);
		oldCacheLeft = oldCacheTop = 0;
	}
	else
	{
		// shifts imageCache
		imageCache.Scroll(Size(oldCacheLeft-cacheLeft, oldCacheTop-cacheTop));
	
		// calculates the (1 or 2) rects that must be repainted
		int startLine = 0;
		int endLine = imageCache.GetHeight();
		
		// top rectangle, if any
		if(cacheTop < oldCacheTop)
		{
			rects.Add(Rect(0, 0, imageCache.GetWidth(), oldCacheTop-cacheTop));
			startLine += oldCacheTop-cacheTop;
		}
		// bottom rectangle, if any
		else if(cacheTop > oldCacheTop)
		{
			rects.Add(Rect(0, imageCache.GetHeight()-(cacheTop-oldCacheTop), imageCache.GetWidth(), imageCache.GetHeight()));
			endLine += oldCacheTop-cacheTop;
		}
		// left rectangle, if any
		if(cacheLeft < oldCacheLeft)
			rects.Add(Rect(0, startLine, oldCacheLeft - cacheLeft, endLine));
		else if(cacheLeft > oldCacheLeft)
			rects.Add(Rect(imageCache.GetWidth()-(cacheLeft-oldCacheLeft), startLine, imageCache.GetWidth(), endLine));
	}
	
	// gets the image pages
	Array<MemoryRaster>&pages = rasterCtrl->GetPages();
	
	// scans the tiff pages to see if some of them fits in
	// rectangles that must be repainted
	int currentTop = 0;
	for(int i = 0 ; i < pages.GetCount() ; i++)
	{
		// translates current top of page in view coordinates
		int viewCurrentTop = iscale(currentTop, imageScale, 1000);
		
		// gets current page		
		MemoryRaster &page = pages[i];

		// gets current page size and translates it in view coordinates
		Rect viewPageRect(
			-cacheLeft,
			-cacheTop + viewCurrentTop,
			iscale(page.GetSize().cx, imageScale, 1000) - cacheLeft,
			iscale(page.GetSize().cy, imageScale, 1000) - cacheTop + viewCurrentTop
		);
		
		// now scans the rectangles that must be repainted
		// and checks if current page has some area on it
		for(int iRect = 0; iRect < rects.GetCount(); iRect++)
		{
			// gets current rectangle
			Rect rect = rects[iRect];
			
			// intersects with page rect
			rect.Intersect(viewPageRect);
			
			// if intersection is not empty, do repaint the stuff
			if(!rect.IsEmpty())
			{
				// gets back the tiff rectangle
				Rect tiffRect(
					iscale(rect.left + cacheLeft, 1000, imageScale), 
					iscale(rect.top + cacheTop, 1000, imageScale) - currentTop, 
					iscale(rect.right + cacheLeft, 1000, imageScale), 
					iscale(rect.bottom + cacheTop, 1000, imageScale) -currentTop
				);
				
				// rescales the image area
				ImageEncoder t;
				Rescale(t, rect.GetSize(), page, tiffRect);
		
				// and copies insiede the cache area
				Image img = Image(t);
				ASSERT(rect.top >= 0 && rect.top + img.GetHeight() <= imageCache.GetHeight());
				ASSERT(rect.left >= 0 && rect.left + img.GetWidth() <= imageCache.GetWidth());
				imageCache.Copy(Point(rect.left, rect.top), Rect(0, 0, img.GetWidth(), img.GetHeight()), img);
			}
		}
		currentTop += page.GetHeight() + iscale(10, 1000, imageScale);
	}
	
	
} // END RasterBaseCtrl::PaintCache()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// paint routine
void RasterBaseCtrl::Paint(Draw &d)
{
	// clears background
	d.DrawRect(GetSize(), SColorFace());

	// if no image opened, does nothing
	if(!rasterCtrl->IsOpened())
		return;

	// paints image inside cache, if needed
	PaintCache();
	
	// paints image cache inside control
	if(imageCache.GetWidth() >= GetSize().cx)
		imageCache.Paint(d, Point(0, 0));
	else
		imageCache.Paint(d, Point((GetSize().cx - imageCache.GetWidth()) / 2, 0));

} // END RasterBaseCtrl::Paint()

///////////////////////////////////////////////////////////////////////////////////////////////
// scrollbar handler
void RasterBaseCtrl::OnScroll(void)
{
	Refresh();
	
} // END RasterBaseCtrl::OnScroll()
	
///////////////////////////////////////////////////////////////////////////////////////////////
// handles changes in images or ctrl
void RasterBaseCtrl::Layout(void)
{
	// this to avoid recursion
	static bool inside = false;
	
	// if no image opened, hides scrollbar and return
	if(!rasterCtrl->IsOpened())
	{
		hScrollBar.Hide();
		vScrollBar.Hide();
		return;
	}
	
	// if already doing layout, just return
	if(inside)
		return;
	
	// marks inside layout
	inside = true;
	
	// stores scroll positions, in order to go on right position
	// after zoom operations
	int hScrollPos = hScrollBar.Get();
	int hScrollMax = hScrollBar.GetTotal();
	int vScrollPos = vScrollBar.Get();
	int vScrollMax = vScrollBar.GetTotal();

	// gets the image pages
	Array<MemoryRaster>&pages = rasterCtrl->GetPages();

	// calculates max width and height
	// and total Tiff height, for all pages, with no gaps by now
	int rasterWidth = 0;
	int rasterHeight = 0;
	int maxHeight = 0;
	int pageCount = pages.GetCount();
	for(int i = 0 ; i < pageCount ; i++)
	{
		// gets page size
		Size sz = pages[i].GetSize();
		
		// updates width, maximum height and total height
		if(sz.cx > rasterWidth)
			rasterWidth = sz.cx;
		if(sz.cy > maxHeight)
			maxHeight = sz.cy;
		rasterHeight += sz.cy;
	}
	
	// calculates image scale factor
	imageScale = CalcScale(imageScale, rasterWidth, maxHeight);
	
	// stops layouting if imagescale is null
	if(!imageScale)
	{
		inside = false;
		return;
	}

	// now calculate gaps to be exactly 10 pixels in every zoom factor
	int gapSize = iscale(10, 1000, imageScale);
	
	// adds total gaps sizes to total height
	rasterHeight += (pageCount -1) * gapSize;

	// and finally, sets up scrollbars and shows them
	// and calculate cache sizes
	int scaledRasterHeight = iscale(rasterHeight, imageScale, 1000);
	int cacheHeight;
	vScrollBar.SetPage(GetSize().cy);
	vScrollBar.SetTotal(scaledRasterHeight);
	if(vScrollMax)
		vScrollBar.Set(iscale(vScrollPos, scaledRasterHeight, vScrollMax));
	else
		vScrollBar.Set(0);
	if(GetSize().cy <= scaledRasterHeight)
	{
		if(hasVScrollBar)
			vScrollBar.Show();
		cacheHeight = GetSize().cy;
	}
	else
	{
		vScrollBar.Hide();
		cacheHeight = scaledRasterHeight;
	}
	int scaledRasterWidth = iscale(rasterWidth, imageScale, 1000);
	int cacheWidth;
	hScrollBar.SetPage(GetSize().cx);
	hScrollBar.SetTotal(scaledRasterWidth);
	if(hScrollMax)
		hScrollBar.Set(iscale(hScrollPos, scaledRasterWidth, hScrollMax));
	else
		hScrollBar.Set(0);
	if(GetSize().cx <= scaledRasterWidth)
	{
		if(hasHScrollBar)
			hScrollBar.Show();
		cacheWidth = GetSize().cx;
	}
	else
	{
		hScrollBar.Hide();
		cacheWidth = scaledRasterWidth;
	}
	
	// creates cache image, empty by now
	imageCache.Init(Size(cacheWidth, cacheHeight));
	
	// updates image
	Refresh();
	
	// mark layout terminated
	inside = false;
	
} // END RasterBaseCtrl::Layout()
