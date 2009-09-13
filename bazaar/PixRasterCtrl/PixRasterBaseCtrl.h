#ifndef _PixRasterBaseCtrl_h_
#define _PixRasterBaseCtrl_h_

#include <CtrlLib/CtrlLib.h>

#include "ImageCache.h"

using namespace Upp;

// types of zoom inside view
enum ZoomTypes { ZOOM_NORMAL, ZOOM_WIDTH, ZOOM_PAGE };
		
// forward declaration
class PixRasterCtrl;

// Tiff view ctrl class
class PixRasterBaseCtrl : public Ctrl
{
	public :
		typedef PixRasterBaseCtrl CLASSNAME;
	
	protected :
	
		// Associated control
		PixRasterCtrl *pixRasterCtrl;
		
		// scrollbars
		bool hasHScrollBar, hasVScrollBar;
		ScrollBar hScrollBar, vScrollBar;
		
		// images scale -- in percent of real size
		int imageScale;

		// calculates image scale factor
		// pure virtual function, MUST be redefined on derived classes
		virtual int CalcScale(int imageScale, int rasterWidth, int maxPageHeight) = 0;

		// scrollbar handler
		virtual void OnScroll(void);
	
		// paint routine
		virtual void Paint(Draw &d);
		
		// ring 2 dimensional buffer to speed up scroll/pan operations
		ImageCache imageCache;
		int cacheLeft, cacheTop;
		
		// repaint images on image cache
		virtual void PaintCache(void);
		
		// repaint polygon markers over the images
		virtual void PaintMarkers(void);
		
		// mouse wheel handler
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
		
		// pan point on middle mouse button pan operations
		Point panPoint;
		int panHScroll, panVScroll;

		// mouse button pan handlers
		virtual void MiddleDown(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
		
		virtual Image CursorImage(Point p, dword keyflags);
		
	public :
	
		// constructor
		PixRasterBaseCtrl(PixRasterCtrl *r, bool hScroll, bool vScroll);
		
		// destructor
		~PixRasterBaseCtrl();
		
		// handles changes in images or ctrl
		virtual void Layout(void);	
		
} ; // END class PixRasterBaseCtrl

#endif
