#ifndef _TestLEDCtrl_TestLEDCtrl_h
#define _TestLEDCtrl_TestLEDCtrl_h

#include <CtrlLib/CtrlLib.h>
#include "LEDCtrl/LEDCtrl.h"

using namespace Upp;

#define LAYOUTFILE <TestLEDCtrl/TestLEDCtrl.lay>
#include <CtrlCore/lay.h>



class TestLEDCtrl : public WithTestLEDCtrlLayout<TopWindow> {
public:
	typedef TestLEDCtrl CLASSNAME;
	TestLEDCtrl();
	
private :
	LEDCtrl MyLED ;
	AnimatedLEDCtrl MyAnimLed, MyBlinkLed ;
	
	void TerminerApplication( void ) ;
};

#endif

