#ifndef _TEST_H_
#define _TEST_H_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <TestArrayCtrl/Test.lay>
#include <CtrlCore/lay.h>

#include "TestArrayCtrl.h"

class TestTabCtrl : public WithTestTabCtrlLayout<TopWindow>{

	typedef _TestArrayCtrl< WithTestArrayCtrlLayout<ParentCtrl> > TestArrayCtrl;

public:
	TestArrayCtrl _testArrayCtrl;

	TestTabCtrl();

	typedef TestTabCtrl CLASSNAME;
};

#endif
