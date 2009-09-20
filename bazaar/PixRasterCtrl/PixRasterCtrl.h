#ifndef _PixRasterCtrl_h_
#define _PixRasterCtrl_h_

#include <CtrlLib/CtrlLib.h>

#include <PixRaster/PixRaster.h>

#include "PixRasterBaseCtrl.h"

NAMESPACE_UPP

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
		PixBase *pixBase;
		
		// thumbnail panel flag
		bool hasThumbnails;
		
		// initialize the control
		void Create(PixBase *_pixBase);
		
	public :
	
		// constructors
		PixRasterCtrl(PixBase *pixBase);
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
		void SetPixBase(PixBase *_pixBase);
		
		// gets the PixRaster object
		PixBase *GetPixBase() { return pixBase; }
		
		// sets current page
		void SetPage(int page);
		
		// reloads ctrl content -- needed when changing images in
		// associated PixRaster control
		void Reload(void);
		
} ; // END Class PixRasterCtrl

END_UPP_NAMESPACE

#endif
