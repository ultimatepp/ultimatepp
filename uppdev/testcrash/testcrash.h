#ifndef _testcrash_testcrash_h
#define _testcrash_testcrash_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <testcrash/testcrash.lay>
#include <CtrlCore/lay.h>



class testcrash : public WithtestcrashLayout<TopWindow> {
public:
	typedef testcrash CLASSNAME;
	testcrash();
	void testing();
};

#endif

