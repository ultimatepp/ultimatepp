#ifndef _PyConsoleCtrlTest_PyConsoleCtrlTest_h
#define _PyConsoleCtrlTest_PyConsoleCtrlTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <PyConsoleCtrl/PyConsoleCtrl.h>

#define LAYOUTFILE <PyConsoleCtrlTest/PyConsoleCtrlTest.lay>
#include <CtrlCore/lay.h>

class PyConsoleCtrlTest : public WithPyConsoleCtrlTestLayout<TopWindow> {
public:
	typedef PyConsoleCtrlTest CLASSNAME;
	PyConsoleCtrlTest();
};

#endif

