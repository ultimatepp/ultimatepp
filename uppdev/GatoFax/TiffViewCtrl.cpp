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
	
	// marks cache as invalid
	cacheValid = false;
	
} // END constructor class TiffViewCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
TiffViewCtrl::~TiffViewCtrl()
{
	
} // END destructor class TiffViewCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// shifts the image in cache
void TiffViewCtrl::ShiftCache(const Size &amount)
{
	RGBA *src, *dst;
	
	// if image is completely shifted out, or shift is null, does nothing
	if(
		abs(amount.cx) >= imageCache.GetWidth()		|| 
		abs(amount.cy) >= imageCache.GetHeight()	||
		(amount.cx == 0 && amount.cy == 0)
	)
		return;
	
	// different behaviour if shifting up or down
	int iLine, jLine;
	if(amount.cy > 0)
	{
		// different behaviour if shifting left/right
		if(amount.cx > 0)
		{
			// shift down, start from bottom
			for(jLine = imageCache.GetHeight()-1, iLine = jLine-amount.cy; iLine >= 0; iLine--, jLine--)
				// shift right
				memmove(imageCache[jLine]+amount.cx, imageCache[iLine], (imageCache.GetWidth()-amount.cx)*sizeof(RGBA));
		}
		else
		{
			// shift down, start from bottom
			for(jLine = imageCache.GetHeight()-1, iLine = jLine-amount.cy; iLine >= 0; iLine--, jLine--)
				// shift left
				memmove(imageCache[jLine], imageCache[iLine]-amount.cx, (imageCache.GetWidth()+amount.cx)*sizeof(RGBA));
		}
			
	}
	else
	{
		// different behaviour if shifting left/right
		if(amount.cx > 0)
		{
			// shift up, start from top
			for(jLine = 0, iLine = jLine-amount.cy; jLine < imageCache.GetHeight(); iLine++, jLine++)
				// shift right
				memmove(imageCache[jLine]+amount.cx, imageCache[iLine], (imageCache.GetWidth()-amount.cx)*sizeof(RGBA));
		}
		else
		{
			// shift up, start from top
			for(jLine = 0, iLine = jLine-amount.cy; jLine < imageCache.GetHeight(); iLine++, jLine++)
				// shift left
				memmove(imageCache[jLine], imageCache[iLine]-amount.cx, (imageCache.GetWidth()+amount.cx)*sizeof(RGBA));
		}
	}
} // END TiffViewCtrl::ShiftCache()


///////////////////////////////////////////////////////////////////////////////////////////////
// repaint images on image cache
void TiffViewCtrl::PaintCache()
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
	
	// if cache is not valid, gets full cache area
cacheValid = false;
	if(!cacheValid)
	{
		rects.Add(Rect(imageCache.GetSize()));
		cacheValid = true;
	}
	else
	{
		// shifts imageCache
//@@		ShiftCache(Size(cacheLeft-oldCacheLeft, cacheTop-oldCacheTop));
	
		// calculates the (1 or 2) rects that must be repainted
		int startLine = 0;
		int endLine = imageCache.GetHeight();
		
		// top rectangle, if any
		if(cacheTop > oldCacheTop)
		{
			rects.Add(Rect(0, 0, imageCache.GetWidth(), cacheTop - oldCacheTop));
			startLine += cacheTop - oldCacheTop;
		}
		// bottom rectangle, if any
		else if(cacheTop < oldCacheTop)
		{
			rects.Add(Rect(0, imageCache.GetHeight()-(oldCacheTop-cacheTop), imageCache.GetWidth(), imageCache.GetHeight()));
			endLine -= cacheTop - oldCacheTop;
		}
		// left rectangle, if any
		if(cacheLeft > oldCacheLeft)
			rects.Add(Rect(0, startLine, cacheLeft - oldCacheLeft, endLine));
		else if(cacheLeft < oldCacheLeft)
			rects.Add(Rect(imageCache.GetWidth()-(oldCacheLeft-cacheLeft), startLine, imageCache.GetWidth(), endLine));
	}
	
	// gets the image pages
	Array<MemoryRaster>&pages = tiffCtrl->GetPages();
	
if(vScrollBar.Get() != 0)
	cacheTop = cacheTop;	

	// repaints rectangle(s) inside cache
	for(int iRect = 0; iRect < rects.GetCount(); iRect++)
	{
		// gets current rectangle
		Rect &rect = rects[iRect];
		
		// gets rectangle corners in full tiff coordinates
		Rect tiffRect(
			iscale(rect.left + cacheLeft, 100, imageScale), 
			iscale(rect.top + cacheTop, 100, imageScale), 
			iscale(rect.right + cacheLeft, 100, imageScale), 
			iscale(rect.bottom + cacheTop, 100, imageScale)
		);
		
		// now scans the tiff images and repaints stuffs inside current cache rectangle
		int currentTop = 0;
		for(int i = 0 ; i < pages.GetCount() ; i++)
		{
			// gets current page		
			MemoryRaster &page = pages[i];
			
			// gets rect in current tiff rect coordinates
			Rect sourceRect(tiffRect.left, tiffRect.top-currentTop, tiffRect.right, tiffRect.bottom-currentTop);
			
			// intersects with current page
			sourceRect.Intersect(Rect(page.GetSize()));
			
			// checks if some part of it must be redraw
			if(!sourceRect.IsEmpty())
			{
ASSERT(!imageCache.IsEmpty());
				// ok, must redraw image part
				
				// get the destination size
				Size destSize(
					iscale(sourceRect.right-sourceRect.left, imageScale, 100),
					iscale(sourceRect.bottom-sourceRect.top, imageScale, 100)
				);
Cerr() << "cacheTopLeft:(" << cacheLeft << "," << cacheTop << ")\n";
Cerr() << "cacheSize:(" << imageCache.GetWidth() << "," << imageCache.GetHeight() << ")\n";
Cerr() << "rect:" << rect << "\n";
Cerr() << "tiffRect:" << tiffRect << "\n";
Cerr() << "sourceRect:" << sourceRect << "\n";
Cerr() << "destSize:" << destSize << "\n";

				// extract and rescale image area
				ImageEncoder t;
				Rescale(t, destSize, page, sourceRect);
				
				Image img = Image(t);
Cerr() << "Img   Len:" << img.GetLength() << "\n";
Cerr() << "cache Len:" << imageCache.GetLength() << "\n";
				// copies rescaled image inside cache rectangle
				const RGBA *rgba = ~img;
ASSERT(img.GetLength() != 0 && imageCache.GetLength() != 0);
//				Copy(~imageCache, rgba, min(img.GetLength(), imageCache.GetLength()));

				for(int iLine = 0 ; iLine < img.GetHeight(); iLine++)
				{
					Copy(imageCache[i+rect.top]+rect.left, rgba, img.GetWidth());
//					memmove(imageCache[i+rect.top]+rect.left, rgba, t.GetWidth()* sizeof(RGBA));
					rgba += img.GetWidth();
				}

//Image img(t);
//imageCache = img;
			}
			
			currentTop += page.GetHeight() + iscale(10, 100, imageScale);
		}
	}
	
	
} // END TiffViewCtrl::PaintCache()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// paint routine
void TiffViewCtrl::Paint(Draw &d)
{
	// if no image opened, does nothing
	if(!tiffCtrl->IsOpened())
		return;
	
	// paints image inside cache, if needed
	PaintCache();
	
	// paints image cache inside control
	if(imageCache.GetWidth() >= GetSize().cx)
		d.DrawImage(0, 0, imageCache);
	else
		d.DrawImage((GetSize().cx - imageCache.GetWidth()) / 2, 0, imageCache);

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
		
		return;
	}

	// gets the image pages
	Array<MemoryRaster>&pages = tiffCtrl->GetPages();

	// calculates max width and height
	// and total Tiff height, for all pages, with no gaps by now
	tiffWidth = 0;
	tiffHeight = 0;
	int maxHeight = 0;
	int pageCount = pages.GetCount();
	for(int i = 0 ; i < pageCount ; i++)
	{
		// gets page size
		Size sz = pages[i].GetSize();
		
		// updates width, maximum height and total height
		if(sz.cx > tiffWidth)
			tiffWidth = sz.cx;
		if(sz.cy > maxHeight)
			maxHeight = sz.cy;
		tiffHeight += sz.cy;
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
			imageScale = iscale(GetSize().cx, 100, tiffWidth);
			break;
			
		// FREE ZOOM - must get zoom factor from main control
		default :
			imageScale = tiffCtrl->GetZoomFactor();
			break;
	} // switch zoomType
	
	// now calculate gaps to be exactly 10 pixels in every zoom factor
	int gapSize = iscale(10, 100, imageScale);
	
	// adds total gaps sizes to total height
	tiffHeight += (pageCount -1) * gapSize;

	// and finally, sets up scrollbars and shows them
	// and calculate cache sizes
	int scaledTiffHeight = iscale(tiffHeight, imageScale, 100);
	int cacheHeight;
	if(GetSize().cy <= scaledTiffHeight)
	{
		vScrollBar.SetPage(GetSize().cy);
		vScrollBar.SetTotal(scaledTiffHeight);
		vScrollBar.Show();
		cacheHeight = GetSize().cy;
	}
	else
	{
		vScrollBar.Hide();
		cacheHeight = scaledTiffHeight;
	}
	int scaledTiffWidth = iscale(tiffWidth, imageScale, 100);
	int cacheWidth;
	if(GetSize().cx <= scaledTiffWidth)
	{
		hScrollBar.SetPage(GetSize().cx);
		hScrollBar.SetTotal(scaledTiffWidth);
		hScrollBar.Show();
		cacheWidth = GetSize().cx;
	}
	else
	{
		hScrollBar.Hide();
		cacheWidth = scaledTiffWidth;
	}
	
	// creates cache image, empty by now
	imageCache.Create(cacheWidth, cacheHeight);
	
	// marks cache as invalid
	cacheValid = false;
	
	// updates image
	Refresh();
			
} // END TiffViewCtrl::::Layout()
