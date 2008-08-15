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

		// total dimension of image file, in unscaled pixels
		int tiffWidth, tiffHeight;

		// scrollbar handler
		void OnScroll(void);
	
		// paint routine
		virtual void Paint(Draw &d);
		
		// buffer to speed up scroll/pan operations
		ImageBuffer imageCache;
		int cacheLeft, cacheTop;
		bool cacheValid;
		
		// shifts the image in cache
		void ShiftCache(const Size &amount);
		
		// repaint images on image cache
		void PaintCache(void);
		
	public :
	
		// constructor
		TiffViewCtrl(TiffCtrl *t);
		
		// destructor
		~TiffViewCtrl();
		
		// handles changes in images or ctrl
		virtual void Layout(void);		
		
} ; // END class TiffViewCtrl

#endif
