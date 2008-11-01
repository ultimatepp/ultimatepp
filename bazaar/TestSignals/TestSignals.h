#ifndef _TestSignals_TestSignals_h
#define _TestSignals_TestSignals_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <TestSignals/TestSignals.lay>
#include <CtrlCore/lay.h>



class TestSignals : public WithTestSignalsLayout<TopWindow>
{
	private:
	
		// handler for SIGHUP signal
		void onHup(void);
		
		// handler for SIGQUIT signal
		void onQuit(void);

	public:
		typedef TestSignals CLASSNAME;
		TestSignals();
};

#endif

