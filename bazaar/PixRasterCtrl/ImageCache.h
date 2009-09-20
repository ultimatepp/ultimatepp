#ifndef _ImageCache_h_
#define _ImageCache_h_

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

class ImageCache
{
	private:
	
		// the image buffer
		ImageBuffer imageBuffer;
	
		// Image position inside buffer
		int hPos, vPos;
	
		// flag indicating image is valid inside cache
		bool valid;
		
		// background color for clearing
		RGBA bkColor;
		
	public:
	
		// constructor
		ImageCache();
		
		// destructor
		~ImageCache();
	
		// initializes the cache
		void Init(Size const &sz, bool clearBackground = true, const RGBA *bkCol = 0);
	
		// fills a zone with a given color
		void Fill(Rect const &destRect, const RGBA *color = 0);
		
		// clears the cache's bacground
		void Clear(const RGBA *color = 0);

		// scrolls the image in cache
		void Scroll(const Size &amount, bool clearBackground = true, const RGBA *bkCol = 0);
		
		// copies an image onto cache at a given position
		void Copy(Point const &destPoint, const Rect &srcRect, Image const &img);
		void Copy(Point const &destPoint, const Rect &srcRect, ImageBuffer &imgBuf);

		// paints the chache on a Draw object
		void Paint(Draw &d, Point const &destPt);
		
		// gets sizes of cache
		Size const GetSize(void);
		int GetWidth(void) { return GetSize().cx; }
		int GetHeight(void) { return GetSize().cy; }
		
		// checks for cache validity
		bool IsValid(void) { return !imageBuffer.IsEmpty() && valid ; }
		void SetValid(bool v = true) { if(!imageBuffer.IsEmpty()) valid = v; else valid = false; }
	
}; // END Class ImageCache

END_UPP_NAMESPACE

#endif
