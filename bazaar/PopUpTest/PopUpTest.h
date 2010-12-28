#ifndef _PopUpTest_PopUpTest_h
#define _PopUpTest_PopUpTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <PopUpC/PopUpC.h>

#define LAYOUTFILE <PopUpTest/PopUpTest.lay>
#include <CtrlCore/lay.h>

class PopUpTest : public WithPopUpTestLayout<TopWindow> {
public:
	typedef PopUpTest CLASSNAME;
	PopUpTest();
	
	void DoDrop();
	void OkCB();
	void CancelCB();
	
	WithPopUpLay<PopUpC> pu;
};

#endif

