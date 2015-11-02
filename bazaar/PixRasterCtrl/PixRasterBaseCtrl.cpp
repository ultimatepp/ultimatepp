#include "PixRasterBaseCtrl.h"
#include "PixRasterCtrl.h"

NAMESPACE_UPP

#define BackColor SColorFace()

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor
PixRasterBaseCtrl::PixRasterBaseCtrl(PixRasterCtrl *t, bool hScroll, bool vScroll)
{
	// connects to associated RasterCtrl
	pixRasterCtrl = t;
	
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
	
	// no marker selected
	selectedMarker = NULL;
	highlightMarker = NULL;
	dragPolygon.Clear();
	
} // END constructor class PixRasterBaseCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
PixRasterBaseCtrl::~PixRasterBaseCtrl()
{
} // END destructor class PixRasterBaseCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// converts a page and an array of page points into an array of view points
Vector<Point> PixRasterBaseCtrl::PointsToView(int page, Vector<Point> const &pts)
{
	// gets the PixRaster object
	PixBase *pixBase = pixRasterCtrl->GetPixBase();

	// calculate view position inside the full tiff image
	// in page coords
	int viewLeft, viewTop;
	if(hScrollBar.IsVisible())
		viewLeft = ScaleToPage(hScrollBar.Get());
	else
		viewLeft = 0;
	if(vScrollBar.IsVisible())
		viewTop = ScaleToPage(vScrollBar.Get());
	else
		viewTop = 0;

	// gets requested page position (in page coordinates)
	int pageTop = 0;
	int gapSize = ScaleToPage(10);
	for(int i = 0; i < page; i++)
		pageTop += pixBase->GetHeightEx(i) + gapSize;

	// if page not visible, just return an empty array
	int pageBottom = pageTop + pixBase->GetHeightEx(page);
	int viewBottom = viewTop + ScaleToPage(GetSize().cy);
	if(viewBottom < pageTop || viewTop > pageBottom)
		return Vector<Point>();
	
	// horizontal gap if page width < ctrl width
	int viewWidth = ScaleToPage(GetSize().cx);
	int pageWidth = pixBase->GetWidthEx(page);
	int hGap;
	if(pageWidth < viewWidth)
		hGap = (viewWidth - pageWidth) / 2;
	else
		hGap = 0;
	
	// calculates the offset of points in page coordinates
	int dx = -viewLeft + hGap;
	int dy = -viewTop + pageTop;
	
	// creates the new array scaling the coordinates
	Vector<Point>res(pts, 1);
	for(int iPoint = 0; iPoint < res.GetCount(); iPoint++)
	{
		res[iPoint].x = ScaleToView(res[iPoint].x + dx);
		res[iPoint].y = ScaleToView(res[iPoint].y + dy);
	}
	return res;
	
} // END PixRasterBaseCtrl::PointsToView()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// converts a view point into page + page point
bool PixRasterBaseCtrl::PointToPage(Point const &srcPt, int &page, Point &destPt)
{
	if(!pixRasterCtrl)
		return false;
			
	// gets the PixRaster object
	PixBase *pixBase = pixRasterCtrl->GetPixBase();
	if(!pixBase || !pixBase->GetPageCount())
		return false;

	// gets point position in raster coordinates
	int yPos = ScaleToPage(vScrollBar.Get() + srcPt.y);

	// gets gap size between pages
	int gapSize = ScaleToPage(10);
	
	// iterates through page positions to find the requested one
	int top = 0;
	for(int iPage = 0; iPage < pixBase->GetPageCount(); iPage++)
	{
		int pageWidth = pixBase->GetWidthEx(iPage);
		int pageHeight = pixBase->GetHeightEx(iPage);
		int bottom = top + pageHeight;
		
		if(yPos >= top && yPos <= bottom)
		{
			int xPos;
			if(ScaleToView(pageWidth) >= GetSize().cx)
				xPos = ScaleToPage(srcPt.x);
			else
				xPos = ScaleToPage(srcPt.x) - (ScaleToPage(GetSize().cx) - pageWidth) / 2;
			if(xPos < 0 || xPos > pageWidth)
				return false;
			destPt = Point(xPos, yPos - top);
			page = iPage;
			return true;
		}
		top += pageHeight + gapSize;
	}
	return false;
} // END PixRasterBaseCtrl::PointToPage()

///////////////////////////////////////////////////////////////////////////////////////////////
// left mouse button handlers
void PixRasterBaseCtrl::LeftDown(Point p, dword keyflags)
{
	// translates mouse coordinate in page and page coordinates
	int page;
	Point pagePt;
	if(!PointToPage(p, page, pagePt))
	   return;
	   
	// found a page on point, scan it to get the marker on it (if any)

	// gets the PixRaster object
	PixBase *pixBase = pixRasterCtrl->GetPixBase();

	int minDist = ScaleToPage(5);
	Markers *markers = pixBase->GetMarkersEx(page);
	for(int i = 0; i < markers->GetCount(); i++)
	{
		Marker &marker = (*markers)[i];
		int index;
		Marker::HitKind hitKind = marker.Hit(pagePt, minDist, index);
		if(hitKind == Marker::Miss)
			continue;
		
		// found a marker on cursor -- start drag op
		dragPolygon = PointsToView(page, marker.DragOutline(pagePt, pagePt, minDist));
		selectedMarker = &marker;
		dragPoint = pagePt;
		dragPage = page;
	}
	
	
} // END PixRasterBaseCtrl::LeftDown()

void PixRasterBaseCtrl::LeftUp(Point p, dword keyflags)
{
	Point endDragPoint;
	int endDragPage;
	
	// if dragging a marker, stop and update it
	if(selectedMarker)
	{
		int minDist = ScaleToPage(5);
		PointToPage(p, endDragPage, endDragPoint);
		// if dest point still on page, complete dragging op
		// otherwise abort it
		if(endDragPage == dragPage)
			selectedMarker->Drag(dragPoint, endDragPoint, minDist);
		selectedMarker = NULL;
		pixRasterCtrl->Refresh();
	}
	
} // END PixRasterBaseCtrl::LeftDown()

///////////////////////////////////////////////////////////////////////////////////////////////
// right mouse button handlers
void PixRasterBaseCtrl::RightDown(Point p, dword keyflags)
{
	
} // END PixRasterBaseCtrl::RightDown()

///////////////////////////////////////////////////////////////////////////////////////////////
// middle mouse button pan handlers
void PixRasterBaseCtrl::MiddleDown(Point p, dword keyflags)
{
	panPoint = p;
	panHScroll = hScrollBar.Get();
	panVScroll = vScrollBar.Get();
	
} // END PixRasterBaseCtrl::MiddleDown()

void PixRasterBaseCtrl::MouseMove(Point p, dword keyflags)
{
	Point endDragPoint;
	int endDragPage;
	
	if(!pixRasterCtrl || !pixRasterCtrl->GetPixBase())
		return;
	
	// check what we're dragging....
	if(keyflags & K_MOUSEMIDDLE)
	{
		// moves with middle button pressed -- panning image
		
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
	}
	else if((keyflags & K_MOUSELEFT) && selectedMarker)
	{
		// dragging a marker
		
		int minDist = ScaleToPage(5);
		PointToPage(p, endDragPage, endDragPoint);

		if(endDragPage == dragPage)
			dragPolygon = PointsToView(dragPage, selectedMarker->DragOutline(dragPoint, endDragPoint, minDist));
		else
			dragPolygon.Clear();
		pixRasterCtrl->Refresh();
	}
	else
	{
		// nothing else, just highlight marker under cursor
		// and set the appropriate cursor
		int page;
		Point pagePt;
		int minDist = ScaleToPage(5);
		Marker *newMarker = NULL;
		if(PointToPage(p, page, pagePt))
		{
			PixBase *pixBase = pixRasterCtrl->GetPixBase();
			Markers *markers = pixBase->GetMarkersEx(page);
			for(int iMarker = 0; iMarker < markers->GetCount(); iMarker++)
			{
				Marker &marker = (*markers)[iMarker];
				int index;
				if(marker.Hit(pagePt, minDist, index) != Marker::Miss)
				{
					newMarker = &marker;
					break;
				}
			}
		}
		if(highlightMarker != newMarker)
		{
			highlightMarker = newMarker;
			pixRasterCtrl->Refresh();
		}
	}
	
} // END PixRasterBaseCtrl::MouseMove()

Image PixRasterBaseCtrl::CursorImage(Point p, dword keyflags)
{
	if(keyflags & K_MOUSEMIDDLE)
		return Image::Hand();
	else
		return Image::Arrow();
	
}

///////////////////////////////////////////////////////////////////////////////////////////////
// mouse wheel handler
void PixRasterBaseCtrl::MouseWheel(Point p, int zdelta, dword keyflags)
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
	
} // END PixRasterBaseCtrl::MouseWheel()

///////////////////////////////////////////////////////////////////////////////////////////////
// repaint images on image cache
void PixRasterBaseCtrl::PaintCache()
{
	Array<Rect> rects;
	
	// if no associated PixRaster object, do nothing
	if(!pixRasterCtrl || !pixRasterCtrl->GetPixBase())
		return;
	
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
	
	// gets associated PixRaster object
	PixBase *pixBase = pixRasterCtrl->GetPixBase();
	
	// scans the PixRaster pages to see if some of them fits in
	// rectangles that must be repainted
	int currentTop = 0;
	int currentPage = pixBase->GetActivePage();
	for(int i = 0 ; i < pixBase->GetPageCount() ; i++)
	{
		// sets the active page
		pixBase->SeekPage(i);
		
		// translates current top of page in view coordinates
		int viewCurrentTop = ScaleToView(currentTop);
		
		// gets current page size and translates it in view coordinates
		Rect viewPageRect(
			-cacheLeft,
			-cacheTop + viewCurrentTop,
			ScaleToView(pixBase->GetSize().cx) - cacheLeft,
			ScaleToView(pixBase->GetSize().cy) - cacheTop + viewCurrentTop
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
					ScaleToPage(rect.left + cacheLeft), 
					ScaleToPage(rect.top + cacheTop) - currentTop, 
					ScaleToPage(rect.right + cacheLeft), 
					ScaleToPage(rect.bottom + cacheTop) -currentTop
				);
				
				// rescales the image area
				ImageEncoder t;
				Rescale(t, rect.GetSize(), *pixBase, tiffRect);
		
				// and copies insiede the cache area
				Image img = Image(t);
				ASSERT(rect.top >= 0 && rect.top + img.GetHeight() <= imageCache.GetHeight());
				ASSERT(rect.left >= 0 && rect.left + img.GetWidth() <= imageCache.GetWidth());
				imageCache.Copy(Point(rect.left, rect.top), Rect(0, 0, img.GetWidth(), img.GetHeight()), img);
			}
		}
		currentTop += pixBase->GetHeight() + ScaleToPage(10);
	}
	
	// restore PixRaster's active page
	pixBase->SeekPage(currentPage);
	
	
} // END PixRasterBaseCtrl::PaintCache()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// repaint polygon markers over the images
void PixRasterBaseCtrl::PaintMarkers(Draw &d)
{
	// if no associated PixRaster object, do nothing
	if(!pixRasterCtrl || !pixRasterCtrl->GetPixBase())
		return;
	
	// gets associated PixRaster object
	PixBase *pixBase = pixRasterCtrl->GetPixBase();
	
	// calculate view position inside the full tiff image
	int left, top;
	if(hScrollBar.IsVisible())
		left = hScrollBar.Get();
	else
		left = 0;
	if(vScrollBar.IsVisible())
		top = vScrollBar.Get();
	else
		top = 0;

	// loop for all pages, to see which of them fits the view
	int currentTop = 0;
	int currentPage = pixBase->GetActivePage();
	for(int i = 0 ; i < pixBase->GetPageCount() ; i++)
	{
		// sets the active page
		pixBase->SeekPage(i);
		
		// translates current top of page in view coordinates
		int viewCurrentTop = ScaleToView(currentTop);
		
		// gets current page size and translates it in view coordinates
		Rect viewPageRect(
			-left,
			-top + viewCurrentTop,
			ScaleToView(pixBase->GetSize().cx) - left,
			ScaleToView(pixBase->GetSize().cy) - top + viewCurrentTop
		);
		
		// this is when page view is smaller than ctrl view
		int hGap;
		int pw = viewPageRect.right - viewPageRect.left;
		if(pw >= GetSize().cx)
			hGap = 0;
		else
			hGap = (GetSize().cx - pw) / 2;

		// checks wether the page fits the view..
		viewPageRect.Intersect(GetView());
		if(!viewPageRect.IsEmpty())
		{
		
			// this page is inside view, let's take its markers
			Markers &markers = *pixBase->GetMarkers();
			for(int iMarker = 0; iMarker < markers.GetCount(); iMarker++)
			{
				Marker &marker = markers[iMarker];
				
				// don't paint marker being dragged... it will be done by
				// drag routine itself
				if(&marker == selectedMarker)
					continue;
				
				switch(marker.GetKind())
				{
					case Marker::EmptyMarker:
						continue;
						
					default:
						Vector<Point> pts = marker.GetPoints();
						Buffer<Point> points(pts.GetCount());
						for(int i = 0; i < pts.GetCount(); i++)
						{
							points[i].x = ScaleToView(pts[i].x) - left + hGap;
							points[i].y = ScaleToView(pts[i].y) - top + viewCurrentTop;
						}
						if(&marker != highlightMarker)
							d.DrawPolygon(points, pts.GetCount(),
							marker.GetFillColor(),
							marker.GetBorderThickness(),
							marker.GetBorderColor(),
							marker.GetBorderLineType(),
							White);
						else
							d.DrawPolygon(points, pts.GetCount(),
							marker.GetSelFillColor(),
							marker.GetSelBorderThickness(),
							marker.GetSelBorderColor(),
							marker.GetSelBorderLineType(),
							White);
						break;
				}
			}
		}
		currentTop += pixBase->GetHeight() + ScaleToPage(10);
	}

	// restore PixRaster's active page
	pixBase->SeekPage(currentPage);
	
} // END PixRasterBaseCtrl::PaintMarkers()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// paint routine
void PixRasterBaseCtrl::Paint(Draw &d)
{
	// clears background
	d.DrawRect(GetSize(), SColorFace());

	// if no associated PixRaster, does nothing
	if(!pixRasterCtrl->GetPixBase())
		return;

	// paints image inside cache, if needed
	PaintCache();
	
	// paints image cache inside control
	if(imageCache.GetWidth() >= GetSize().cx)
		imageCache.Paint(d, Point(0, 0));
	else
		imageCache.Paint(d, Point((GetSize().cx - imageCache.GetWidth()) / 2, 0));
	
	// paints markers inside control
	PaintMarkers(d);
	
	// if dragging, paints rubber banded polygon
	if(selectedMarker && dragPolygon.GetCount())
	{
		d.DrawPolygon(dragPolygon, dragPolygon.GetCount(),
		selectedMarker->GetFillColor(),
		selectedMarker->GetBorderThickness(),
		selectedMarker->GetBorderColor(),
		PEN_DOT,
		White);
	}

} // END LeptonicaBaseCtrl::Paint()

///////////////////////////////////////////////////////////////////////////////////////////////
// scrollbar handler
void PixRasterBaseCtrl::OnScroll(void)
{
	Refresh();
	
} // END PixRasterBaseCtrl::OnScroll()
	
///////////////////////////////////////////////////////////////////////////////////////////////
// handles changes in images or ctrl
void PixRasterBaseCtrl::Layout(void)
{
	// this to avoid recursion
	static bool inside = false;
	
	// if no associated PixRaster or no pages to display, hides scrollbar and return
	if(!pixRasterCtrl->GetPixBase() || !pixRasterCtrl->GetPageCount())
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

	// gets the PixRaster object
	PixBase *pixBase = pixRasterCtrl->GetPixBase();

	// calculates max width and height
	// and total Tiff height, for all pages, with no gaps by now
	int rasterWidth = 0;
	int rasterHeight = 0;
	int maxHeight = 0;
	int pageCount = pixBase->GetPageCount();
	for(int i = 0 ; i < pageCount ; i++)
	{
		// sets current page
		pixBase->SeekPage(i);
		
		// gets page size
		Size sz = pixBase->GetSizeEx(i);
		
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
	int gapSize = ScaleToPage(10);
	
	// adds total gaps sizes to total height
	rasterHeight += (pageCount -1) * gapSize;

	// and finally, sets up scrollbars and shows them
	// and calculate cache sizes
	int scaledRasterHeight = ScaleToView(rasterHeight);
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
	int scaledRasterWidth = ScaleToView(rasterWidth);
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
	
} // END PixRasterBaseCtrl::Layout()

END_UPP_NAMESPACE
