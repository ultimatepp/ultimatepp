#include "kmedis.h"

//---------------------------------------------------------------------------
#define TOP_WINDOW_TITLE "KMEdis"

//---------------------------------------------------------------------------
Kmedis::Kmedis()
{
    CtrlLayout(*this, TOP_WINDOW_TITLE);
    CenterScreen();

    //----------------------------------------------------------
    SetTimeCallback(1500, THISBACK(Close));
}


void Kmedis::Close()
{
    RLOG("B");    
    WithKmedisLayout<TopWindow>::Close();
    RLOG("C");
}

//---------------------------------------------------------------------------
GUI_APP_MAIN
{
    Kmedis kmedis;
    RLOG("A");
//  kmedis.Minimize();
    kmedis.Run();
    RLOG("D");
}
