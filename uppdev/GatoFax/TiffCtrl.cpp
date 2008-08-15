#include "TiffCtrl.h"

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor
TiffCtrl::TiffCtrl()
{
	// creates child controls and inserts them in splitter
	
	// sets splitter position
	
	// sets to not opened
	isOpened = false;
	
	// clears raster object
	raster.Clear();
	
	// sets zoom mode to full page
	zoomType = ZOOM_WIDTH;
	
	// initializes zoom factor (should not be necessary, but...)
	zoomFactor = 100;
	
	// initializes pan position to left corner
	panPosition = 0;
	
	// signals image change to thumbs and view
	thumbs->Layout();
	view->Layout();

	thumbs = new TiffThumbsCtrl(this);
	view = new TiffViewCtrl(this);
	hSplitter << *thumbs << *view ;
	hSplitter.Horz().SetPos(2000);
	Add(hSplitter);
} // END Constructor class TiffCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
TiffCtrl::~TiffCtrl()
{
	// removes associated controls
	delete thumbs;
	delete view;
	
	// closes the image stream
	Close();
	
} // END Destructor class TiffCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// control paint routine
void TiffCtrl::Paint(Draw &d)
{
	Ctrl::Paint(d);
/*
	ImageEncoder t;
	raster->SeekPage(pageno);
	Rescale(t, GetSize(), *raster, raster->GetSize());
	w.DrawImage(0, 0, t);
*/	
	
} // END TiffCtrl::Paint()

///////////////////////////////////////////////////////////////////////////////////////////////
// opens an image file in control
bool TiffCtrl::Open(String const &fileName)
{
	// if already opened, closes current stream
	if(isOpened)
		Close();
	
	// opens file and associate to raster
	imageStream.Open(fileName);
	raster = StreamRaster::OpenAny(imageStream);
	
	// loads pages in array of memoryrasters
	pages.Clear();
	for(curPage = 0; curPage < raster->GetPageCount(); curPage++)
	{
		raster->SeekPage(curPage);
		pages.Add().Load(*raster);
	}
	curPage = 0;
	
	// signals that stream is opened
	isOpened = true;
	
	// signal page changed to thumbs and view
	thumbs->Layout();
	view->Layout();
	
	return true;
} // END TiffCtrl::Open()

///////////////////////////////////////////////////////////////////////////////////////////////
// closes image file and frees resources
void TiffCtrl::Close()
{
	imageStream.Close();
	
	// signals that stream is closed
	isOpened = false;
	
	// frees buffered pages
	pages.Clear();
	
	// signal page changed to thumbs and view
	thumbs->Layout();
	view->Layout();
	
} // END TiffCtrl::Close()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets current active page
int TiffCtrl::setPage(int page)
{
	return 0;
} // END TiffCtrl::setPage()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets thumbnails on/off
bool TiffCtrl::ShowThumbnails(bool s)
{
	return false;
} // END TiffCtrl::ShowThumbnails()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets zoom to page width
void TiffCtrl::ZoomWidth()
{
	// sets mode to page width
	zoomType = ZOOM_WIDTH;
	
	// signals layout change
	view->Layout();
	
} // END TiffCtrl::ZoomWidth()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets zoom to full page
void TiffCtrl::ZoomPage()
{
	// sets mode to page height
	zoomType = ZOOM_PAGE;

	// signals layout change
	view->Layout();
	
} // END TiffCtrl::ZoomPage()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets the zoom to an arbitrary value - in percentage from original size
void TiffCtrl::Zoom(int z)
{
	// checks and adjusts zoom factor
	if(z <= 0) z = 1;
	if(z > 150) z = 150;
	
	// sets it
	zoomType = ZOOM_NORMAL;
	zoomFactor = z ;

	// signals layout change
	view->Layout();
	
} // END TiffCtrl::Zoom()

///////////////////////////////////////////////////////////////////////////////////////////////
// pans the image
void TiffCtrl::Pan(int Horz, int Vert)
{

} // END TiffCtrl::Pan()

///////////////////////////////////////////////////////////////////////////////////////////////
// gets page count
int TiffCtrl::GetPageCount()
{
	if(isOpened)
		return raster->GetPageCount();
	else
		return 0;

} // END TiffCtrl::GetPageCount()
		

