#ifndef _QuickTest_QuickTest_h
#define _QuickTest_QuickTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class QuickTest : public TopWindow {
public:
	virtual void Paint(Draw& w);

	typedef QuickTest CLASSNAME;
	QuickTest();
};

#endif

