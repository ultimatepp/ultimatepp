#include "RasterCtrl.h"

#include "RasterThumbsCtrl.h"
#include "RasterViewCtrl.h"

///////////////////////////////////////////////////////////////////////////////////////////////
// constructor
RasterCtrl::RasterCtrl()
{
	// sets to not opened
	isOpened = false;
	
	// creates child controls and inserts them in splitter
	thumbs = new RasterThumbsCtrl(this);
	view = new RasterViewCtrl(this);
	hSplitter << *thumbs << *view ;
	Add(hSplitter);
	
	// sets splitter position
	hSplitter.Horz().SetPos(2000);
	hasThumbnails = true;
	
	// clears raster object
	raster.Clear();
	
	// signals image change to thumbs and view
	thumbs->Layout();
	view->Layout();

} // END Constructor class RasterCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
RasterCtrl::~RasterCtrl()
{
	// removes associated controls
	delete thumbs;
	delete view;
	thumbs = NULL;
	view = NULL;
	
	// closes the image stream
	Close();
	
} // END Destructor class RasterCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// opens an image file in control
bool RasterCtrl::Open(String const &fileName)
{
	// if already opened, closes current stream
	if(isOpened)
		Close();
	
	// opens file and associate to raster
	if(!imageStream.Open(fileName))
		return false;
	raster = StreamRaster::OpenAny(imageStream);
	if(!raster)
		return false;
	
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
	
	// if multipage, shows thumbnails, otherwise not
	if(raster->GetPageCount() > 1)
		ShowThumbnails(true);
	else
		ShowThumbnails(false);
	
	// signal page changed to thumbs and view
	thumbs->Layout();
	view->Layout();
	
	return true;
	
} // END RasterCtrl::Open()

///////////////////////////////////////////////////////////////////////////////////////////////
// closes image file and frees resources
void RasterCtrl::Close()
{
	imageStream.Close();
	
	// signals that stream is closed
	isOpened = false;
	
	// frees buffered pages
	pages.Clear();
	
	// signal page changed to thumbs and view
	if(thumbs)
		thumbs->Layout();
	if(view)
		view->Layout();
	
} // END RasterCtrl::Close()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets thumbnails on/off
bool RasterCtrl::ShowThumbnails(bool s)
{
	bool prev = hasThumbnails;
	
	if(s)
		hSplitter.Horz().SetPos(2000);
	else
		hSplitter.Horz().SetPos(0);
	hasThumbnails = s;
	return prev;
	
} // END RasterCtrl::ShowThumbnails()

///////////////////////////////////////////////////////////////////////////////////////////////
// gets page count
int RasterCtrl::GetPageCount()
{
	if(isOpened)
		return raster->GetPageCount();
	else
		return 0;

} // END RasterCtrl::GetPageCount()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets zoom to page width/full page
void RasterCtrl::ZoomWidth()
{
	view->ZoomWidth();
	
} // END RasterCtrl::ZoomWidth()

void RasterCtrl::ZoomPage()
{
	view->ZoomPage();

} // END RasterCtrl::ZoomPage()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets the zoom to an arbitrary value - in percentage from original size
void RasterCtrl::Zoom(int z)
{
	view->Zoom(z);

} // END RasterCtrl::Zoom()

///////////////////////////////////////////////////////////////////////////////////////////////
// zooms inside/outside by fixed factors
void RasterCtrl::Zoom(bool in)
{
	view->Zoom(in);

} // END RasterCtrl::Zoom()

///////////////////////////////////////////////////////////////////////////////////////////////
// gets zoom type
ZoomTypes RasterCtrl::GetZoomType(void)
{
	return view->GetZoomType();

} // END RasterCtrl::GetZoomType()

///////////////////////////////////////////////////////////////////////////////////////////////
// gets zoom factor
int RasterCtrl::GetZoomFactor(void)
{
	return view->GetZoomFactor();

} // END RasterCtrl::GetZoomFactor()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets current page
void RasterCtrl::SetPage(int page)
{
	view->SetPage(page);

} // END RasterCtrl::SetPage()
