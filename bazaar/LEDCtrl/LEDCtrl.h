#ifndef _LEDCtrl_LEDCtrl_h
#define _LEDCtrl_LEDCtrl_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;


// Definition of one LED character
// 8 int values are necessary
#define LED_VALUES_PER_CHARACTER		7

class LEDCtrl : public Ctrl {
public:
	typedef LEDCtrl CLASSNAME;
	LEDCtrl();
	LEDCtrl( String Value,
	         int OneLEDSize = 2,			// 2 pixels for one LED
	         Color FgLightOnColor = LtRed,
	         Color FgLightOffColor = Black,
	         Color BgColor = Black );
	~LEDCtrl() ;

	virtual void Paint(Draw& w);

	LEDCtrl& SetString( String Value ) ;
	LEDCtrl& SetFgLightOnColor( Color Value ) ;
	LEDCtrl& SetFgLightOffColor( Color Value ) ;
	LEDCtrl& SetBgColor( Color Value ) ;
	LEDCtrl& SetStringSize( int CharactersNumber ) ;
	LEDCtrl& SetLEDSize( int LEDSize ) ;
	LEDCtrl& SetPosZ( int X, int Y ) ;

protected :
	LEDCtrl& SetDisplayedString( String Value ) ;

private :	
	VectorMap<String, Array<int> > LEDSet ;
	Array<int> UnknownLED ;
	Color FgLightOnColor ;
	Color FgLightOffColor ;
	Color BgColor ;
	String DisplayedString ;
	int OneLEDSize ;
	int CharNumber ;
	int PosX ;
	int PosY ;

	void InitializeLEDSet( void ) ;
	Array<bool> DecomposeLine( int ) ;
};



class AnimatedLEDCtrl : public LEDCtrl {
public:
	typedef AnimatedLEDCtrl CLASSNAME;
	typedef enum {
		LED_SLIDING = 0,
		LED_BLINKING
	} LEDAnimationType ;
	
	AnimatedLEDCtrl();
	AnimatedLEDCtrl( String Value,
	         LEDAnimationType Type = LED_SLIDING,
	         int MaxCharNumber = 20,
	         int Timer = 1000,				// 1 second
	         int OneLEDSize = 2,			// 2 pixels for one LED
	         Color FgLightOnColor = LtRed,
	         Color FgLightOffColor = Black,
	         Color BgColor = Black );
	~AnimatedLEDCtrl() ;

	AnimatedLEDCtrl& SetString( String Value ) ;
	AnimatedLEDCtrl& SetTimer(int _Timer)  {Timer = _Timer; return *this;};			// In ms
	AnimatedLEDCtrl& SetAnimationType( LEDAnimationType Type ) ;
	AnimatedLEDCtrl& SetMaxCharNumber(int _MaxCharNumber) {MaxCharNumber = _MaxCharNumber; return *this;};

private :
	String TheDisplayedString ;
	String TheSourceString ;
	int DisplayedStringLength ;
	int MaxCharNumber ;
	int CharIndex ;
	int Timer ;
	LEDAnimationType Type ;

	void DoSliding( void ) ;
	void DoBlinking( void ) ;
};


#endif

