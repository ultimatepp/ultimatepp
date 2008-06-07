#ifndef _RasterCtrl_h_
#define _RasterCtrl_h_

#include <CtrlLib/CtrlLib.h>

#include "RasterBaseCtrl.h"

using namespace Upp;

class RasterThumbsCtrl;
class RasterViewCtrl;

// Main RasterCtrl Class
class RasterCtrl : public Ctrl
{
	public :
		typedef RasterCtrl CLASSNAME;

	private:
		// left and right panes
		RasterThumbsCtrl *thumbs;;
		RasterViewCtrl *view;

		// splitter for thumbnails/page panes
		Splitter hSplitter;

		// raster object associated fo file
		One<StreamRaster> raster;
		
		// stream associated to image file
		FileIn imageStream;
		
		// array of pages in memoryraster format
		Array<MemoryRaster> pages;
		
		// current activa page in tiff file
		int curPage;
		
		// thumbnail panel flag
		bool hasThumbnails;
		
		// opened flag
		bool isOpened;
		
	public :
	
		// constructor
		RasterCtrl();
		
		// destructor
		~RasterCtrl();
		
		// opens an image file in control
		bool Open(String const &fileName);
		
		// closes image file and frees resources
		void Close();
		
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
		
		// gets the raster object
		StreamRaster &GetRaster() { return *raster; }
		
		// gets pages object
		Array<MemoryRaster> &GetPages() { return pages;}
		
		// sets current page
		void SetPage(int page);
		
		// checks whether image stream is opened
		bool IsOpened() { return isOpened; }
	
} ; // END Class RasterCtrl

#endif
