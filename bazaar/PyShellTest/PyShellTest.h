#ifndef _PyShellTest_PyShellTest_h
#define _PyShellTest_PyShellTest_h

#include <PyShell/PyShell.h>

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define LAYOUTFILE <PyShellTest/PyShellTest.lay>
#include <CtrlCore/lay.h>

class PyShellTest : public WithPyShellTestLayout<TopWindow> {
public:
	typedef PyShellTest CLASSNAME;
	PyShellTest();
	~PyShellTest();

	void StartNew();

protected:
	PyShell ps;
};

#endif

