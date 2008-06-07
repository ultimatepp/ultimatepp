#ifndef _RasterViewCtrl_h_
#define _RasterViewCtrl_h_

#include <CtrlLib/CtrlLib.h>

#include "RasterBaseCtrl.h"

using namespace Upp;

// Tiff view ctrl class
class RasterViewCtrl : public RasterBaseCtrl
{
	public :
		typedef RasterViewCtrl CLASSNAME;
	
	private :
	
		// image zoom type
		ZoomTypes zoomType;
		
		// calculates image scale factor
		virtual int CalcScale(int imageScale, int rasterWidth, int maxPageHeight);

	public :
	
		// constructor
		RasterViewCtrl(RasterCtrl *t);
		
		// destructor
		~RasterViewCtrl();
		
		// mouse wheel handler
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
		
		// sets zoom to page width/full page
		virtual void ZoomWidth();
		virtual void ZoomPage();
		
		// sets the zoom to an arbitrary value - in percentage from original size
		virtual void Zoom(int z);
		
		// zooms inside/outside by fixed factors
		virtual void Zoom(bool in);
		
		// gets zoom type
		virtual ZoomTypes GetZoomType(void) { return zoomType; }

		// gets zoom factor of current view
		virtual int GetZoomFactor(void);
		
		// sets current page
		virtual void SetPage(int page);
		
} ; // END class RasterViewCtrl

#endif
