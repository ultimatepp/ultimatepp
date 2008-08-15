#ifndef _TiffViewCtrl_h_
#define _TiffViewCtrl_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

// forward declaration
class TiffCtrl;

//#############################################################################################
// Tiff view ctrl class
class TiffViewCtrl : public Ctrl
{
	public :
		typedef TiffViewCtrl CLASSNAME;
	
	private :
	
		// Associated control
		TiffCtrl *tiffCtrl;
		
		// scrollbars
		ScrollBar hScrollBar, vScrollBar;
		
		// images scale -- in percent of real size
		int imageScale;

		// array of images positions and sizes
		Vector<Rect> imageRects;
	
		// scrollbar handler
		void OnScroll(void);
	
		// paint routine
		virtual void Paint(Draw &d);
		
		// struct defining cached image buffers and sizes
		struct CachedImage : Moveable<CachedImage>
		{
			// flag marking cached status
			bool cached;
			
			// the image encoder
			Image image;
			
			// the size used encoding it
			Size size;
			
			// constructor
			CachedImage() { cached = false; size.Clear(); }
			
			// checks if cached image is valid
			bool IsValid(Size const &sz) { return cached && sz == size; }
			
			// sets the cached status
			bool SetCached(Size const &sz) { size = sz; cached = true; }
			
		}; // END struct CachedImages
		
		// Array of cached images
		Vector<CachedImage> CachedImages;
	
	public :
	
		// constructor
		TiffViewCtrl(TiffCtrl *t);
		
		// destructor
		~TiffViewCtrl();
		
		// handles changes in images or ctrl
		virtual void Layout(void);		
		
} ; // END class TiffViewCtrl

#endif
