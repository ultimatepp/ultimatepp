#ifndef _LogPosCtrlTest_LogPosCtrlTest_h
#define _LogPosCtrlTest_LogPosCtrlTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <LogPosCtrl/LogPosCtrl.h>

#define LAYOUTFILE <LogPosCtrlTest/LogPosCtrlTest.lay>
#include <CtrlCore/lay.h>

#include <LogPosCtrl/LogPosCtrl.h>

class LogPosCtrlTest : public WithLogPosCtrlTestLayout<TopWindow> {
public:
	typedef LogPosCtrlTest CLASSNAME;
	LogPosCtrlTest();
	
	void ChangeLogPosCB();

	LogPosCtrl le;
};

#endif

