#include "ImageCache.h"

NAMESPACE_UPP

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor
ImageCache::ImageCache()
{
	// marks cache as not valid
	valid = false;
	
	// clears the imagebuffer
	imageBuffer.Clear();
	
	// sets the ring pointer to topleft corner of imagebuffer
	hPos = vPos = 0;
	
	// sets up the background color
	bkColor = Color(0xef, 0xeb, 0xe7);

} // END constructor class ImageCache

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
ImageCache::~ImageCache()
{

} // END destructor class ImageCache

///////////////////////////////////////////////////////////////////////////////////////////////
// initializes the cache
void ImageCache::Init(Size const &sz, bool clearBackground, const RGBA *bkCol)
{
	// sets up the buffer
	imageBuffer.Create(sz);
	
	// if background present, change it
	if(bkCol)
		bkColor = *bkCol;
	
	// marks cache as invalid
	valid = false;
	
	// resets ring buffer position
	hPos = vPos = 0;
	
	// if requested, clear background
	if(clearBackground)
		Fill(Rect(GetSize()));

} // END ImageCache::Init()

///////////////////////////////////////////////////////////////////////////////////////////////
// fills a zone with a given color
// warning, destRect MUST be normalized, and size is NOT checked
void ImageCache::Fill(Rect const &destRect, const RGBA *color)
{
	// if buffer empty, does nothing
	if(imageBuffer.IsEmpty())
		return;
	
	// if color is null, sets to background
	if(!color)
		color = &bkColor;
	
	// gets destinatin point in real cache coordinates
	Point destPt(destRect.left + hPos, destRect.top + vPos);
	
	// gets dest size
	Size size(destRect.right - destRect.left, destRect.bottom - destRect.top);

	// splits both source and dest rectangles to match cache
	Array<Point> dPoints1;
	Array<Size> sizes1;
	
	// first splits horizontally, if needed
	if(destPt.x < imageBuffer.GetWidth())
	{
		dPoints1.Add(destPt);
		if(destPt.x + size.cx <= imageBuffer.GetWidth())
			sizes1.Add(size);
		else
		{
			int firstSize = imageBuffer.GetWidth() - destPt.x;
			sizes1.Add(Size(firstSize, size.cy));
			dPoints1.Add(Point(0, destPt.y));
			sizes1.Add(Size(size.cx - firstSize, size.cy));
		}
	}
	else
	{
		dPoints1.Add(Point(destPt.x % imageBuffer.GetWidth(), destPt.y));
		sizes1.Add(size);
	}
	// then splits vertically, if needed
	Array<Point> dPoints2;
	Array<Size> sizes2;
	for(int iPart = 0; iPart < sizes1.GetCount(); iPart++)
	{
		Point &destPt = dPoints1[iPart];
		Size &size = sizes1[iPart];
		if(destPt.y < imageBuffer.GetHeight())
		{
			dPoints2.Add(destPt);
			if(destPt.y + size.cy <= imageBuffer.GetHeight())
				sizes2.Add(size);
			else
			{
				int firstSize = imageBuffer.GetHeight() - destPt.y;
				sizes2.Add(Size(size.cx, firstSize));
				dPoints2.Add(Point(destPt.x, 0));
				sizes2.Add(Size(size.cx, size.cy - firstSize));
			}
		}
		else
		{
			dPoints2.Add(Point(destPt.x, destPt.y % imageBuffer.GetHeight()));
			sizes2.Add(size);
		}
	}
	
	// here we have all we need in dPoints2, sizes2
	// we just fill corresponding rectangles
	for(int iRect = 0 ; iRect < sizes2.GetCount(); iRect++)
	{
		Point &dPoint = dPoints2[iRect];
		Size &size = sizes2[iRect];
		for(int iLine = 0; iLine < size.cy; iLine++)
		{
			RGBA *dst = imageBuffer[dPoint.y + iLine] + dPoint.x;
			for(int iCol = 0; iCol < size.cx; iCol++)
				*dst++ = *color;
		}
	}
		
} // END ImageCache::Fill()

///////////////////////////////////////////////////////////////////////////////////////////////
// clears the cache's bacground
void ImageCache::Clear(const RGBA *bkCol)
{
	// if background present, change it
	if(bkCol)
		bkColor = *bkCol;
	
	// resets ring buffer position
	hPos = vPos = 0;

	// marks cache as invalid
	valid = false;
	
	// if buffer empty, does nothing
	if(!imageBuffer.IsEmpty())
		return;

	// fills buffer with background color
	Fill(Rect(0, 0, imageBuffer.GetWidth(), imageBuffer.GetHeight()));

} // END ImageCache::Clear()

///////////////////////////////////////////////////////////////////////////////////////////////
// scrolls the image in cache
void ImageCache::Scroll(Size const &amount, bool clearBackground, const RGBA *bkCol)
{
	// if cache is not valid or size is null, just restores ring pointer
	if(!valid || imageBuffer.IsEmpty())
	{
		hPos = vPos = 0;
		return;
	}
	
	// advances the ring pointer
	int dx = amount.cx % imageBuffer.GetWidth();
	int dy = amount.cy % imageBuffer.GetHeight();
	hPos -= dx;
	vPos -= dy;
	if(hPos < 0)
		hPos += imageBuffer.GetWidth();
	if(vPos < 0)
		vPos += imageBuffer.GetHeight();
	hPos %= imageBuffer.GetWidth();
	vPos %= imageBuffer.GetHeight();
	
	// if required to change background color, do it
	if(bkCol)
		bkColor = *bkCol;

	// if required, clears the background of scrolled-out parts
	if(clearBackground)
	{
		int start = 0;
		int end = imageBuffer.GetHeight();
		if(dy > 0)
		{
			Fill(Rect(0, 0, imageBuffer.GetWidth(), dy));
			start += dy;
		}
		else if(dy < 0)
		{
			Fill(Rect(0, end + dy, imageBuffer.GetWidth(), end));
			end += dy;
		}
		if(dx > 0)
			Fill(Rect(0, start, dx, end));
		else if(dx < 0)
			Fill(Rect(imageBuffer.GetWidth() + dx, start, imageBuffer.GetWidth(), end));
	}
	
} // END ImageCache::Scroll()

///////////////////////////////////////////////////////////////////////////////////////////////
// copies an image area onto cache at a given position
void ImageCache::Copy(Point const &destPoint, const Rect &srcRect, Image const &img)
{
	// gets destinatin point in real cache coordinates
	Point destPt(destPoint.x + hPos, destPoint.y + vPos);
	
	// gets source starting point
	Point srcPt(srcRect.left, srcRect.top);
	
	// gets dest/src sizes
	Size size(srcRect.right - srcRect.left, srcRect.bottom - srcRect.top);

	// splits both source and dest rectangles to match cache
	Array<Point> sPoints1, dPoints1;
	Array<Size> sizes1;
	
	// first splits horizontally, if needed
	if(destPt.x < imageBuffer.GetWidth())
	{
		dPoints1.Add(destPt);
		sPoints1.Add(srcPt);
		if(destPt.x + size.cx <= imageBuffer.GetWidth())
			sizes1.Add(size);
		else
		{
			int firstSize = imageBuffer.GetWidth() - destPt.x;
			sizes1.Add(Size(firstSize, size.cy));
			dPoints1.Add(Point(0, destPt.y));
			sPoints1.Add(Point(srcPt.x + firstSize, srcPt.y));
			sizes1.Add(Size(size.cx - firstSize, size.cy));
		}
	}
	else
	{
		dPoints1.Add(Point(destPt.x % imageBuffer.GetWidth(), destPt.y));
		sPoints1.Add(srcPt);
		sizes1.Add(size);
	}
	// then splits vertically, if needed
	Array<Point> sPoints2, dPoints2;
	Array<Size> sizes2;
	for(int iPart = 0; iPart < sizes1.GetCount(); iPart++)
	{
		Point &destPt = dPoints1[iPart];
		Point &srcPt = sPoints1[iPart];
		Size &size = sizes1[iPart];
		if(destPt.y < imageBuffer.GetHeight())
		{
			dPoints2.Add(destPt);
			sPoints2.Add(srcPt);
			if(destPt.y + size.cy <= imageBuffer.GetHeight())
				sizes2.Add(size);
			else
			{
				int firstSize = imageBuffer.GetHeight() - destPt.y;
				sizes2.Add(Size(size.cx, firstSize));
				dPoints2.Add(Point(destPt.x, 0));
				sPoints2.Add(Point(srcPt.x, srcPt.y + firstSize));
				sizes2.Add(Size(size.cx, size.cy - firstSize));
			}
		}
		else
		{
			dPoints2.Add(Point(destPt.x, destPt.y % imageBuffer.GetHeight()));
			sPoints2.Add(srcPt);
			sizes2.Add(size);
		}
	}
	
	// here we have all we need in sPoints2, dPoints2, sizes2
	// we just copy corresponding rectangles
	for(int iRect = 0 ; iRect < sizes2.GetCount(); iRect++)
	{
		Point &sPoint = sPoints2[iRect];
		Point &dPoint = dPoints2[iRect];
		Size &size = sizes2[iRect];
		for(int iLine = 0; iLine < size.cy; iLine++)
		{
			const RGBA *src = img[sPoint.y+iLine] + sPoint.x;
			RGBA *dst = imageBuffer[dPoint.y+iLine] + dPoint.x;
			RGBA *test = imageBuffer;
			memmove(dst, src, sizeof(RGBA) * size.cx);
		}
	}
	
} // END ImageCache::Copy()

void ImageCache::Copy(Point const &destPoint, const Rect &srcRect, ImageBuffer &imgBuf)
{
	Image img = imgBuf;
	Copy(destPoint, srcRect, img);
	imgBuf = img;
	
} // END ImageCache::Copy()

///////////////////////////////////////////////////////////////////////////////////////////////
// paints the chache on a Draw object
void ImageCache::Paint(Draw &d, Point const &destPt)
{
	// first, copies buffer into image
	// in order to not wipe it using DrawImage()
	Image img = imageBuffer;

	// goes vertically, splitting if needed
	if(vPos > 0)
	{
		int vSize1 = img.GetHeight() - vPos;
		// first vertical part
		if(hPos > 0)
		{
			int hSize1 = img.GetWidth() - hPos;
			d.DrawImage(destPt.x, destPt.y, img, Rect(hPos, vPos, hSize1 + hPos, vSize1 + vPos));
			d.DrawImage(destPt.x + hSize1, destPt.y, img, Rect(0, vPos, img.GetWidth() - hSize1, vSize1 + vPos));
			d.DrawImage(destPt.x, destPt.y + vSize1, img, Rect(hPos, 0, hSize1 + hPos, img.GetHeight() - vSize1));
			d.DrawImage(destPt.x + hSize1, destPt.y + vSize1, img, Rect(0, 0, img.GetWidth() - hSize1, img.GetHeight() - vSize1));
		}
		else
		{
			d.DrawImage(destPt.x, destPt.y, img, Rect(0, vPos, img.GetWidth(), vSize1 + vPos));
			d.DrawImage(destPt.x, destPt.y + vSize1, img, Rect(0, 0, img.GetWidth(), img.GetHeight() - vSize1));
		}
	}
	else
	{
		if(hPos > 0)
		{
			int hSize1 = img.GetWidth() - hPos;
			d.DrawImage(destPt.x, destPt.y, img, Rect(hPos, 0, hSize1 + hPos, img.GetHeight()));
			d.DrawImage(destPt.x + hSize1, destPt.y, img, Rect(0, 0, img.GetWidth() - hSize1, img.GetHeight()));
		}
		else
			d.DrawImage(destPt.x, destPt.y, img, Rect(img.GetSize()));
	}

	// restores imagebuffer	
	imageBuffer = img;

} // END ImageCache::Paint()


// gets sizes of cache
Size const ImageCache::GetSize(void)
{
	if(imageBuffer.IsEmpty())
		return Size(0, 0);
	return imageBuffer.GetSize();
	
} // EBND ImageCache::GetSize()

END_UPP_NAMESPACE
