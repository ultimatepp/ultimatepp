#include "PixRasterCtrl.h"

#include "PixRasterThumbsCtrl.h"
#include "PixRasterViewCtrl.h"

NAMESPACE_UPP

// initialize the control
void PixRasterCtrl::Create(PixBase *_pixBase)
{
	// sets the PixBase object
	pixBase = _pixBase;
	
	// creates child controls and inserts them in splitter
	thumbs = new PixRasterThumbsCtrl(this);
	view = new PixRasterViewCtrl(this);
	hSplitter << *thumbs << *view ;
	Add(hSplitter);
	
	// sets splitter position
	hSplitter.Horz().SetPos(2000);
	hasThumbnails = true;
	
	// signals image change to thumbs and view
	thumbs->Layout();
	view->Layout();

} // END PixRasterCtrl::Create()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// constructors
PixRasterCtrl::PixRasterCtrl(PixBase *_pixBase)
{
	Create(_pixBase);
	
} // END Constructor class PixRasterCtrl

PixRasterCtrl::PixRasterCtrl()
{
	Create(NULL);
	
} // END Constructor class PixRasterCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// destructor
PixRasterCtrl::~PixRasterCtrl()
{
	// removes associated controls
	delete thumbs;
	delete view;
	thumbs = NULL;
	view = NULL;
	
} // END Destructor class PixRasterCtrl

///////////////////////////////////////////////////////////////////////////////////////////////
// sets the PixBase object
void PixRasterCtrl::SetPixBase(PixBase *_pixBase)
{
	pixBase = _pixBase;
	
	// signals image change to thumbs and view
	thumbs->Layout();
	view->Layout();

} // END PixRasterCtrl::SetPixRaster()
		
///////////////////////////////////////////////////////////////////////////////////////////////
// sets thumbnails on/off
bool PixRasterCtrl::ShowThumbnails(bool s)
{
	bool prev = hasThumbnails;
	
	if(s)
		hSplitter.Horz().SetPos(2000);
	else
		hSplitter.Horz().SetPos(0);
	hasThumbnails = s;
	return prev;
	
} // END PixRasterCtrl::ShowThumbnails()

///////////////////////////////////////////////////////////////////////////////////////////////
// gets page count
int PixRasterCtrl::GetPageCount()
{
	if(pixBase)
		return pixBase->GetPageCount();
	else
		return 0;

} // END PixRasterCtrl::GetPageCount()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets zoom to page width/full page
void PixRasterCtrl::ZoomWidth()
{
	view->ZoomWidth();
	
} // END PixRasterCtrl::ZoomWidth()

void PixRasterCtrl::ZoomPage()
{
	view->ZoomPage();

} // END PixRasterCtrl::ZoomPage()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets the zoom to an arbitrary value - in percentage from original size
void PixRasterCtrl::Zoom(int z)
{
	view->Zoom(z);

} // END PixRasterCtrl::Zoom()

///////////////////////////////////////////////////////////////////////////////////////////////
// zooms inside/outside by fixed factors
void PixRasterCtrl::Zoom(bool in)
{
	view->Zoom(in);

} // END PixRasterCtrl::Zoom()

///////////////////////////////////////////////////////////////////////////////////////////////
// gets zoom type
ZoomTypes PixRasterCtrl::GetZoomType(void)
{
	return view->GetZoomType();

} // END PixRasterCtrl::GetZoomType()

///////////////////////////////////////////////////////////////////////////////////////////////
// gets zoom factor
int PixRasterCtrl::GetZoomFactor(void)
{
	return view->GetZoomFactor();

} // END PixRasterCtrl::GetZoomFactor()

///////////////////////////////////////////////////////////////////////////////////////////////
// sets current page
void PixRasterCtrl::SetPage(int page)
{
	view->SetPage(page);

} // END PixRasterCtrl::SetPage()

///////////////////////////////////////////////////////////////////////////////////////////////
// reloads ctrl content -- needed when changing images in
// associated PixBase control
void PixRasterCtrl::Reload(void)
{
	thumbs->Layout();
	view->Layout();
}
		
END_UPP_NAMESPACE
