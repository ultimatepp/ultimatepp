#ifndef _PixRasterViewCtrl_h_
#define _PixRasterViewCtrl_h_

#include <CtrlLib/CtrlLib.h>

#include "PixRasterBaseCtrl.h"

NAMESPACE_UPP

// Tiff view ctrl class
class PixRasterViewCtrl : public PixRasterBaseCtrl
{
	public :
		typedef PixRasterViewCtrl CLASSNAME;
	
	private :
	
		// image zoom type
		ZoomTypes zoomType;
		
		// calculates image scale factor
		virtual int CalcScale(int imageScale, int rasterWidth, int maxPageHeight);

	public :
	
		// constructor
		PixRasterViewCtrl(PixRasterCtrl *t);
		
		// destructor
		~PixRasterViewCtrl();
		
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
		
} ; // END class PixRasterViewCtrl

END_UPP_NAMESPACE

#endif
