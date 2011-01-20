#ifndef _SysExecTest_SysExecTest_h
#define _SysExecTest_SysExecTest_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SysExecTest/SysExecTest.lay>
#include <CtrlCore/lay.h>

class SysExecTest : public WithSysExecTestLayout<TopWindow>
{
	private:
		StatusBar status;
		
		void btnCb(int n);
		
	public:
		typedef SysExecTest CLASSNAME;
		
		SysExecTest();
};

#endif

