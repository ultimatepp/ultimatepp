#ifndef _kmedis_h
#define _kmedis_h
//---------------------------------------------------------------------------

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <kmedis/kmedis.lay>
#include <CtrlCore/lay.h>

//---------------------------------------------------------------------------
class Kmedis: public WithKmedisLayout<TopWindow> {
public:
    typedef Kmedis CLASSNAME;
    Kmedis();
    
    virtual void Close();
};

//---------------------------------------------------------------------------
#endif
