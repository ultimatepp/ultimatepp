#include "TestLEDCtrl.h"



void TestLEDCtrl::TerminerApplication( void )
{
	Close( ) ;
}


TestLEDCtrl::TestLEDCtrl()
{
	Sizeable().Zoomable( ) ;
	
	CtrlLayout(*this, "LED Ctrl tests");
	WhenClose = THISBACK( TerminerApplication ) ;

	MyLED.SetString("<>@^&~\"{A¨\\'Z`(E#|_R)\"N°Y}=!-+*/0:12.3[45],;?%")
		 .SetLEDSize(3).SetFgLightOnColor(White).SetFgLightOffColor(Blue).SetBgColor(LtBlue);
	Add( MyLED.SetPosZ(5, 5) ) ;

	MyAnimLed.SetString( "This is one animated message with all known characters !! - 0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz");		
	MyAnimLed.SetTimer(120).SetMaxCharNumber(20).SetAnimationType(AnimatedLEDCtrl::LED_SLIDING)
		.SetLEDSize(2).SetFgLightOnColor(Yellow).SetFgLightOffColor(Black);
	Add( MyAnimLed.SetPosZ(5, 50) ) ;
	
	MyBlinkLed.SetString( "And a blinking one. Wonderful, isn't it ?");
	MyBlinkLed.SetTimer(500).SetMaxCharNumber(41).SetAnimationType(AnimatedLEDCtrl::LED_BLINKING)
		.SetLEDSize(3).SetFgLightOnColor(LtGreen).SetFgLightOffColor(Green);
	Add( MyBlinkLed.SetPosZ(5, 80) ) ;
}


GUI_APP_MAIN
{
	TestLEDCtrl().Run();
}

