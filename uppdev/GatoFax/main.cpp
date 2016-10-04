#include "GatoFax.h"

#define IMAGEFILE <GatoFax/GatoFax.iml>
#include <Draw/iml_source.h>

////////////////////////////////////////////////////////////////////////////////////////////////
void GatoMain::MenuNew()
{

} // END GatoMain::MenuNew()

////////////////////////////////////////////////////////////////////////////////////////////////
void GatoMain::MenuExit()
{
    if ( PromptOKCancel( "Exit MyApp?" ) )
        Break();

} // END GatoMain::MenuExit()

////////////////////////////////////////////////////////////////////////////////////////////////
void GatoMain::FileMenu( Bar& bar )
{
    bar.Add( "New", THISBACK( MenuNew ) );
    bar.Add( "Exit", THISBACK( MenuExit ) );

} // END GatoMain::FileMenu()

////////////////////////////////////////////////////////////////////////////////////////////////
void GatoMain::MenuView(int i)
{
	switch(i)
	{
		case -1 :
			view.ZoomWidth();
			break;
		case -2 :
			view.ZoomPage();
			break;
		default :
			view.Zoom(i);
			break;
	}

} // END GatoMain::MenuView()

////////////////////////////////////////////////////////////////////////////////////////////////
void GatoMain::ViewMenu( Bar& bar )
{
    bar.Add( "Page width", THISBACK1( MenuView,  -1 ) );
    bar.Add( "Full page",  THISBACK1( MenuView,  -2 ) );
    bar.Add( "Zoom 10%",   THISBACK1( MenuView,  10 ) );
    bar.Add( "Zoom 20%",   THISBACK1( MenuView,  20 ) );
    bar.Add( "Zoom 50%",   THISBACK1( MenuView,  50 ) );
    bar.Add( "Zoom 100%",  THISBACK1( MenuView, 100 ) );
    bar.Add( "Zoom 150%",  THISBACK1( MenuView, 150 ) );

} // END GatoMain::ViewMenu()

////////////////////////////////////////////////////////////////////////////////////////////////
void GatoMain::MainMenu( Bar& bar )
{
    bar.Add( "File", THISBACK( FileMenu ) );
    bar.Add( "View", THISBACK( ViewMenu ) );

} // END GatoMain::MainMenu()

////////////////////////////////////////////////////////////////////////////////////////////////
bool GatoMain::Key(dword key, int)
{
/*	
	if(key == K_PAGEUP && pageno > 0)
	{
		pageno--;
		Refresh();
	}
	if(key == K_PAGEDOWN && pageno < raster->GetPageCount() - 1)
	{
		pageno++;
		Refresh();
	}
*/
	return true;
}

GatoMain::GatoMain()
{
	view.Open("e:/fax000000002.tif");

	// sets up layout
    CtrlLayout( *this, "Window title" );

	// sets up frame, menu and statusbar
    AddFrame( menu );
    AddFrame( TopSeparatorFrame() );
//	AddFrame(tool);
    AddFrame( status );
    AddFrame( InsetFrame() );

	// populates menu
    menu.Set( THISBACK( MainMenu ) );

	// populate and sizes splitters
    vSplitter.Vert().SetPos( 8000 ); //.HSizePos(10, 10).VSizePos(10,10));
    hSplitter.Horz().SetPos( 2000 );
    hSplitter << line << faxList;
    vSplitter << view << hSplitter;

    Sizeable().Zoomable();
	BackPaint();

	Refresh();

}

GUI_APP_MAIN
{
    GatoMain().Run();
}

