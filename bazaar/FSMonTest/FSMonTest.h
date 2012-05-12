#ifndef _FSMonTest_FSMonTest_h
#define _FSMonTest_FSMonTest_h

#include <FSMon/FSMon.h>

using namespace Upp;

#define LAYOUTFILE <FSMonTest/FSMonTest.lay>
#include <CtrlCore/lay.h>



class FSMonTest : public WithFSMonTestLayout<TopWindow>
{
	private:
		void Log(String const &s);
		
		void startCb(void);
		void stopCb(void);
		void quitCb(void);
		
		FSMon fsmMon;
		
		void monitorCb(void);
	
	protected:
	public:
		typedef FSMonTest CLASSNAME;

		FSMonTest();
};

#endif
