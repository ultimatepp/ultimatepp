#include "RasterCtrlTest.h"

////////////////////////////////////////////////////////////////////////////////////////////////
void RasterCtrlTestWindow::Menu_File_Open_cb()
{
	String fileName;
	FileSelector fs;
	if(fs.ExecuteOpen("Please select a graphic file to view:"))
		rasterCtrl.Open(fs);

} // END RasterCtrlTestWindow::Menu_File_Open()

////////////////////////////////////////////////////////////////////////////////////////////////
void RasterCtrlTestWindow::Menu_File_Exit_cb()
{
    if ( PromptOKCancel( "Exit RasterCtrlTest ?" ) )
        Break();

} // END RasterCtrlTestWindow::Menu_File_Exit_cb()

////////////////////////////////////////////////////////////////////////////////////////////////
void RasterCtrlTestWindow::Menu_File( Bar& bar )
{
    bar.Add( "Open", THISBACK( Menu_File_Open_cb ) );
    bar.Add( "Exit", THISBACK( Menu_File_Exit_cb ) );

} // END RasterCtrlTestWindow::Menu_File()

////////////////////////////////////////////////////////////////////////////////////////////////
void RasterCtrlTestWindow::Menu_View_cb(int i)
{
	switch(i)
	{
		case -1 :
			rasterCtrl.ZoomWidth();
			break;
		case -2 :
			rasterCtrl.ZoomPage();
			break;
		case -3:
			rasterCtrl.ShowThumbnails(!rasterCtrl.HasThumbnails());
			break;
		default :
			rasterCtrl.Zoom(i);
			break;
	}

} // END RasterCtrlTestWindow::Menu_View_cb()

////////////////////////////////////////////////////////////////////////////////////////////////
void RasterCtrlTestWindow::Menu_View( Bar& bar )
{
    bar.Add( "Page width", THISBACK1( Menu_View_cb,  -1 ) );
    bar.Add( "Full page",  THISBACK1( Menu_View_cb,  -2 ) );
    bar.Add( "Zoom 10%",   THISBACK1( Menu_View_cb,  10 ) );
    bar.Add( "Zoom 20%",   THISBACK1( Menu_View_cb,  20 ) );
    bar.Add( "Zoom 50%",   THISBACK1( Menu_View_cb,  50 ) );
    bar.Add( "Zoom 100%",  THISBACK1( Menu_View_cb, 100 ) );
    bar.Add( "Zoom 150%",  THISBACK1( Menu_View_cb, 150 ) );
    bar.Add( "Toggle thumbnails" , THISBACK1( Menu_View_cb, -3 ) );

} // END RasterCtrlTestWindow::ViewMenu()

////////////////////////////////////////////////////////////////////////////////////////////////
void RasterCtrlTestWindow::MainMenu( Bar& bar )
{
    bar.Add( "File", THISBACK( Menu_File ) );
    bar.Add( "View", THISBACK( Menu_View ) );

} // END RasterCtrlTestWindow::MainMenu()


RasterCtrlTestWindow::RasterCtrlTestWindow()
{
	// sets up frame, menu and statusbar
    AddFrame( menu );
    AddFrame( TopSeparatorFrame() );
    AddFrame( status );
    AddFrame( InsetFrame() );

	// populates menu
    menu.Set( THISBACK( MainMenu ) );

	// adds raster control
	Add(rasterCtrl.HSizePos().VSizePos());

    Sizeable().Zoomable();
	BackPaint();

//	view.Open("/home/massimo/sources/uppdev/gatofax/fax000000002.tif");
//	view.Open("/home/massimo/pluto.tif");
	Refresh();

}

GUI_APP_MAIN
{
    RasterCtrlTestWindow().Run();
}

