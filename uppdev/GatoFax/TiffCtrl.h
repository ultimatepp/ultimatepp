#ifndef _GatoFax_TiffCtrl_h_
#define _GatoFax_TiffCtrl_h_

#include <CtrlLib/CtrlLib.h>

#include "TiffThumbsCtrl.h"
#include "TiffViewCtrl.h"

using namespace Upp;

//#############################################################################################
// Main TiffCtrl Class
class TiffCtrl : public Ctrl
{
	public :
		typedef TiffCtrl CLASSNAME;

		enum ZoomTypes { ZOOM_NORMAL, ZOOM_WIDTH, ZOOM_PAGE };
		
	private :
	
		// left and right panes
		TiffThumbsCtrl *thumbs;;
		TiffViewCtrl *view;

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
		bool showThumbnails;
		
		// position of image inside control
		// (when it doesn't fit)
		int imageVertPos;
		int imageHorizPos;
		
		// image zoom factor - in percentage from original size
		int zoomFactor;
		
		// pan position - in percent from 0 (left corner) to 100 (right corner)
		int panPosition;
		
		// image zoom type
		ZoomTypes zoomType;
		
		// thumbnails pane width
		int thumbWidth;
		
		// opened flag
		bool isOpened;
		
		// control paint routine
		virtual void Paint(Draw &d);
	
	public :
	
		// constructor
		TiffCtrl();
		
		// destructor
		~TiffCtrl();
		
		// opens an image file in control
		bool Open(String const &fileName);
		
		// closes image file and frees resources
		void Close();
		
		// sets current active page
		int setPage(int page);
		
		// sets thumbnails on/off
		bool ShowThumbnails(bool s);
		
		// get thumbnail panel flag
		bool Thumbnails() { return showThumbnails; }
		
		// sets zoom to page width/full page
		void ZoomWidth();
		void ZoomPage();
		
		// sets the zoom to an arbitrary value - in percentage from original size
		void Zoom(int z);
		
		// pans the image
		void Pan(int Horz, int Vert);
		
		// gets zoom type
		ZoomTypes GetZoomType(void) { return zoomType; }

		// gets zoom factor
		int GetZoomFactor(void) { return zoomFactor; }

		// gets page count
		int GetPageCount();
		
		// gets the raster object
		StreamRaster &GetRaster() { return *raster; }
		
		// gets pages object
		Array<MemoryRaster> &GetPages() { return pages;}
		
		// checks whether image stream is opened
		bool IsOpened() { return isOpened; }
	
} ; // END Class TiffCtrl

#endif
