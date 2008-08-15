#ifndef _TestApp_TestApp_h
#define _TestApp_TestApp_h

#include <CtrlLib/CtrlLib.h>
//#include <GridCtrl/GridCtrl.h>
using namespace Upp;

#define LAYOUTFILE <TestApp/TestApp.lay>
#include <CtrlCore/lay.h>



class TestApp : public WithTestAppLayout<TopWindow> {
public:
	typedef TestApp CLASSNAME;
	EditString editstring;
	Button b1, b2;
//	void Remove() {grid.DoRemove();}
	void sigBar(Bar& bar);
	TestApp();
};

#endif
