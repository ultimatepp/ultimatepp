#ifndef _PixRasterCtrl_h_
#define _PixRasterCtrl_h_

#include <CtrlLib/CtrlLib.h>

#include <PixRaster/PixRaster.h>

#include "PixRasterBaseCtrl.h"

using namespace Upp;

class PixRasterThumbsCtrl;
class PixRasterViewCtrl;

// Main RasterCtrl Class
class PixRasterCtrl : public Ctrl
{
	public :
		typedef PixRasterCtrl CLASSNAME;

	private:
		// left and right panes
		PixRasterThumbsCtrl *thumbs;;
		PixRasterViewCtrl *view;

		// splitter for thumbnails/page panes
		Splitter hSplitter;

		// PixRaster object associated to the control
		PixRaster *pixRaster;
		
		// thumbnail panel flag
		bool hasThumbnails;
		
		// initialize the control
		void Create(PixRaster *_pixRaster);
		
	public :
	
		// constructors
		PixRasterCtrl(PixRaster *pixRaster);
		PixRasterCtrl();
		
		// destructor
		~PixRasterCtrl();
		
		// sets thumbnails on/off
		bool ShowThumbnails(bool s);
		
		// get thumbnail panel flag
		bool HasThumbnails() { return hasThumbnails; }
		
		// sets zoom to page width/full page
		void ZoomWidth();
		void ZoomPage();
		
		// sets the zoom to an arbitrary value - in percentage from original size
		void Zoom(int z);
		
		// zooms inside/outside by fixed factors
		void Zoom(bool in);
		
		// gets zoom type
		ZoomTypes GetZoomType(void);

		// gets zoom factor
		int GetZoomFactor(void);

		// gets page count
		int GetPageCount();
		
		// sets the PixRaster object
		void SetPixRaster(PixRaster *_pixRaster);
		
		// gets the PixRaster object
		PixRaster *GetPixRaster() { return pixRaster; }
		
		// sets current page
		void SetPage(int page);
		
		// reloads ctrl content -- needed when changing images in
		// associated PixRaster control
		void Reload(void);
		
} ; // END Class PixRasterCtrl

#endif
