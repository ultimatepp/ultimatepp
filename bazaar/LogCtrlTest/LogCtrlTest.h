#ifndef _LogCtrlTest_LogCtrlTest_h
#define _LogCtrlTest_LogCtrlTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <LogCtrl/LogCtrl.h>

#define LAYOUTFILE <LogCtrlTest/LogCtrlTest.lay>
#include <CtrlCore/lay.h>



class LogCtrlTest : public WithLogCtrlTestLayout<TopWindow> {
public:
	typedef LogCtrlTest CLASSNAME;
	LogCtrlTest();
	
	void Clear() { log.Clear(); }
	void Save() { log.Save(); }
	
	LogCtrl log;
};

#endif

