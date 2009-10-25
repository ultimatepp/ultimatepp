#ifndef _testOnglet_testOnglet_h
#define _testOnglet_testOnglet_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <testOnglet/testOnglet.lay>
#include <CtrlCore/lay.h>

class testOnglet : public WithtestOngletLayout<TopWindow> {
public:
	StatusBar statusBar;
	MenuBar menuBar;

	void mainBar(Bar& bar);

	void closeOnglet();
	typedef testOnglet CLASSNAME;
	testOnglet();
};

#endif

