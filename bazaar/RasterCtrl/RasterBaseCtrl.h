#ifndef _RasterBaseCtrl_h_
#define _RasterBaseCtrl_h_

#include <CtrlLib/CtrlLib.h>

#include "ImageCache.h"

using namespace Upp;

// types of zoom inside view
enum ZoomTypes { ZOOM_NORMAL, ZOOM_WIDTH, ZOOM_PAGE };
		
// forward declaration
class RasterCtrl;

// Tiff view ctrl class
class RasterBaseCtrl : public Ctrl
{
	public :
		typedef RasterBaseCtrl CLASSNAME;
	
	protected :
	
		// Associated control
		RasterCtrl *rasterCtrl;
		
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
		RasterBaseCtrl(RasterCtrl *r, bool hScroll, bool vScroll);
		
		// destructor
		~RasterBaseCtrl();
		
		// handles changes in images or ctrl
		virtual void Layout(void);	
		
} ; // END class RasterBaseCtrl

#endif
