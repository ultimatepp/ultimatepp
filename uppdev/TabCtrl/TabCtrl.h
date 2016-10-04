#ifndef _TabCtrl_TabCtrl_h
#define _TabCtrl_TabCtrl_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <TabCtrl/TabCtrl.lay>
#include <CtrlCore/lay.h>

struct TabCtrlTest : public WithTabCtrlLayout<TopWindow> {
	Button b;
	ArrayCtrl arr;
	
	Array<Button> ins;
	
	void AddTab();

	typedef TabCtrlTest CLASSNAME;
	TabCtrlTest();
};

#endif
