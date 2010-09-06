#ifndef _GridTest_Panel_h_
#define _GridTest_Panel_h_

#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>

using namespace Upp;

#define  LAYOUTFILE <GridCtrlTest/Panel.lay>
#include <CtrlCore/lay.h>

struct Panel : WithPanelLayout<ParentCtrl>
{
	GridCtrl* grid;

	WithEditOptionsLayout<ParentCtrl> editopt;
	WithLookOptionsLayout<ParentCtrl> lookopt;
	WithColorsOptionsLayout<ParentCtrl> colsopt;
	WithNumberLayout<ParentCtrl> numsopt;
	
	void LookOptions(int n);
	void ColorsOptions(int n);
	void EditOptions(int n);
	void Actions(int n);
	
	void Init(GridCtrl& g);
	
	Panel();
	typedef Panel CLASSNAME;
};

#endif
