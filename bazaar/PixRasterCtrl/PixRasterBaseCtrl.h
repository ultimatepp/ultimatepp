#ifndef _PixRasterBaseCtrl_h_
#define _PixRasterBaseCtrl_h_

#include <CtrlLib/CtrlLib.h>

#include "ImageCache.h"

NAMESPACE_UPP;

// types of zoom inside view
enum ZoomTypes { ZOOM_NORMAL, ZOOM_WIDTH, ZOOM_PAGE };
		
// forward declarations
class PixRasterCtrl;
class Marker;

// Tiff view ctrl class
class PixRasterBaseCtrl : public LocalLoop
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
		
		// scales a coordinate from page to view
		inline int ScaleToView(int x) { return iscale(x, imageScale, 1000); }
		
		// scales a coordinate from view to page
		inline int ScaleToPage(int x) { return iscale(x, 1000, imageScale); }
		
		// converts a page and an array of page points into an array of view points
		Vector<Point> PointsToView(int page, Vector<Point> const &pts);
		
		// converts a view point into page + page point
		bool PointToPage(Point const &srcPt, int &page, Point &destPt);

		// scrollbar handler
		virtual void OnScroll();
	
		// paint routine
		virtual void Paint(Draw &d);
		
		// ring 2 dimensional buffer to speed up scroll/pan operations
		ImageCache imageCache;
		int cacheLeft, cacheTop;
		
		// repaint images on image cache
		virtual void PaintCache();
		
		// repaint polygon markers over the images
		virtual void PaintMarkers(Draw &d);
		
		// mouse button handlers
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
		virtual void MiddleDown(Point p, dword keyflags);
		virtual void RightDown(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
		
		// mouse wheel handler
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
		
		// point for pan operations
		Point panPoint;
		int panHScroll, panVScroll;

		virtual Image CursorImage(Point p, dword keyflags);
		
		// highlighted marker (under mouse cursor)
		Marker *highlightMarker;
		
		// selected marker and drag mode
		Marker *selectedMarker;
		
		// page on which dragging is done
		int dragPage;
		
		// starting drag point
		Point dragPoint;
		
		// current polygon in dragging ops
		Vector<Point>dragPolygon;
		
	public :
	
		// constructor
		PixRasterBaseCtrl(PixRasterCtrl *r, bool hScroll, bool vScroll);
		
		// destructor
		~PixRasterBaseCtrl();
		
		// handles changes in images or ctrl
		virtual void Layout(void);	
		
} ; // END class PixRasterBaseCtrl

END_UPP_NAMESPACE

#endif
