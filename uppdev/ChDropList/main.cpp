#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow
{
	MultiButtonFrame drop;

	App()
	{
		SetRect( Size( 200, 25 ) );
		drop.AddTo( *this );
		drop.AddButton().Main();
		drop.SetStyle( MultiButtonFrame::StyleFrame() );
		drop.AddButton().Left().SetLabel( "Button" );
	}
};

GUI_APP_MAIN
{
	App().Run();
}
