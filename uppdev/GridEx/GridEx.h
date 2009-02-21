#ifndef _GridEx_GridEx_h
#define _GridEx_GridEx_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <GridCtrl/GridCtrl.h>
#define LAYOUTFILE <GridEx/GridEx.lay>
#include <CtrlCore/lay.h>



class GridEx : public WithGridExLayout<TopWindow> {
public:
	typedef GridEx CLASSNAME;
	GridEx();
};

#endif

