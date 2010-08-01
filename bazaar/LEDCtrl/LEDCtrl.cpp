#include "LEDCtrl.h"

typedef struct {
	char Character ;
	int  Values[LED_VALUES_PER_CHARACTER] ;
} LEDCharacter ;

	
// Initialization of each character LED
static LEDCharacter LEDDefinition [] =
	{ 
		{ ' ',  0,  0,  0,  0,  0,  0,  0 },
		{ '!',  4,  4,  4,  4,  4,  0,  4 },
		{ '"', 10, 10,  0,  0,  0,  0,  0 },
		{ '#', 10, 10, 31, 10, 31, 10, 10 },
		{ '$',  4, 14, 16, 14,  1, 14,  4 },
		{ '%',  1,  9,  2,  4,  8, 18, 16 },
		{ '&', 12, 18, 18, 12, 19, 18, 13 },
		{ '\'', 24,  8,  8,  0,  0,  0,  0 },
		{ '(',  2,  4,  8,  8,  8,  4,  2 },
		{ ')',  8,  4,  2,  2,  2,  4,  8 },
		{ '*',  0,  4, 21, 14, 21,  4,  0 },
		{ '+',  0,  4,  4, 31,  4,  4,  0 },
		{ ',',  0,  0,  0,  0,  8,  8, 16 },
		{ '-',  0,  0,  0, 14,  0,  0,  0 },
		{ '.',  0,  0,  0,  0,  0,  0, 16 },
		{ '/',  1,  1,  2,  4,  8, 16, 16 },
		{ '0', 14, 17, 19, 21, 25, 17, 14 },
		{ '1',  4, 12, 20,  4,  4,  4, 14 },
		{ '2', 14, 17,  1,  2,  4,  8, 31 },
		{ '3', 14, 17,  1, 14,  1, 17, 14 },
		{ '4',  2,  6, 10, 18, 31,  2,  2 },
		{ '5', 31, 16, 16, 14,  1,  1, 30 },
		{ '6',  6,  8, 16, 30, 17, 17, 14 },
		{ '7', 31,  1,  2,  4,  8,  8,  8 },
		{ '8', 14, 17, 17, 14, 17, 17, 14 },
		{ '9', 14, 17, 17, 15,  1,  1, 14 },
		{ ':',  0,  0, 16,  0, 16,  0,  0 },
		{ ';',  0,  0,  8,  0,  8,  8, 16 },
		{ '<',  0,  2,  4,  8,  4,  2,  0 },
		{ '=',  0,  0, 31,  0, 31,  0,  0 },
		{ '>',  0,  8,  4,  2,  4,  8,  0 },
		{ '?', 14, 17,  1,  2,  4,  0,  4 },
		{ '@', 14, 17, 23, 21, 23, 16, 14 },
		{ 'A', 14, 17, 17, 17, 31, 17, 17 },
		{ 'B', 30, 17, 17, 30, 17, 17, 30 },
		{ 'C', 14, 17, 16, 16, 16, 17, 14 },
		{ 'D', 28, 18, 17, 17, 17, 18, 28 },
		{ 'E', 31, 16, 16, 30, 16, 16, 31 },
		{ 'F', 31, 16, 16, 30, 16, 16, 16 },
		{ 'G', 14, 17, 16, 23, 17, 17, 14 },
		{ 'H', 17, 17, 17, 31, 17, 17, 17 },
		{ 'I', 14,  4,  4,  4,  4,  4, 14 },
		{ 'J',  7,  2,  2,  2,  2, 18, 12 },
		{ 'K', 17, 18, 20, 24, 20, 18, 17 },
		{ 'L', 16, 16, 16, 16, 16, 16, 31 },
		{ 'M', 17, 27, 21, 21, 17, 17, 17 },
		{ 'N', 17, 25, 21, 19, 17, 17, 17 },
		{ 'O', 14, 17, 17, 17, 17, 17, 14 },
		{ 'P', 30, 17, 17, 30, 16, 16, 16 },
		{ 'Q', 14, 17, 17, 17, 21, 14,  1 },
		{ 'R', 30, 17, 17, 30, 20, 18, 17 },
		{ 'S', 14, 17, 16, 14,  1, 17, 14 },
		{ 'T', 31,  4,  4,  4,  4,  4,  4 },
		{ 'U', 17, 17, 17, 17, 17, 17, 14 },
		{ 'V', 17, 17, 17, 17, 17, 10,  4 },
		{ 'W', 17, 17, 17, 21, 21, 27, 17 },
		{ 'X', 17, 17, 10,  4, 10, 17, 17 },
		{ 'Y', 17, 17, 10,  4,  4,  4,  4 },
		{ 'Z', 31,  1,  2,  4,  8, 16, 31 },
		{ '[', 14,  8,  8,  8,  8,  8, 14 },
	    { '\\',  0, 16,  8,  4,  2,  1,  0 },
		{ ']', 14,  2,  2,  2,  2,  2, 14 },
		{ '^',  4, 10,  0,  0,  0,  0,  0 },
		{ '_',  0,  0,  0,  0,  0,  0, 31 },
		{ '`',  8,  4,  0,  0,  0,  0,  0 },
		{ 'a',  0,  0, 14,  1, 15, 17, 15 },
		{ 'b', 16, 16, 30, 17, 17, 17, 30 },
		{ 'c',  0,  0, 15, 16, 16, 16, 15 },
		{ 'd',  1,  1, 15, 17, 17, 17, 15 },
		{ 'e',  0,  0, 14, 17, 31, 16, 14 },
		{ 'f',  6,  8,  8, 28,  8,  8,  8 },
		{ 'g',  0, 14, 17, 17, 15,  1, 14 },
		{ 'h', 16, 16, 22, 25, 17, 17, 17 },
		{ 'i',  4,  0, 12,  4,  4,  4, 14 },
		{ 'j',  2,  0,  6,  2,  2, 10,  4 },
		{ 'k', 16, 16, 18, 20, 24, 20, 18 },
		{ 'l', 12,  4,  4,  4,  4,  4, 14 },
		{ 'm',  0,  0, 26, 21, 21, 21, 21 },
		{ 'n',  0,  0, 22, 25, 17, 17, 17 },
		{ 'o',  0,  0, 14, 17, 17, 17, 14 },
		{ 'p',  0,  0, 28, 18, 18, 28, 16 },
		{ 'q',  0,  0, 14, 18, 18, 14,  2 },
		{ 'r',  0,  0, 22, 24, 16, 16, 16 },
		{ 's',  0,  0, 14, 16, 14,  1, 14 },
		{ 't',  8,  8, 28,  8,  8, 10,  4 },
		{ 'u',  0,  0, 17, 17, 17, 17, 14 },
		{ 'v',  0,  0, 17, 17, 17, 10,  4 },
		{ 'w',  0,  0, 17, 17, 21, 21, 10 },
		{ 'x',  0,  0, 17, 10,  4, 10, 17 },
		{ 'y',  0,  0, 17, 10,  4,  4,  4 },
		{ 'z',  0,  0, 31,  2,  4,  8, 31 },
		{ '{',  2,  4,  4,  8,  4,  4,  2 },
		{ '|',  4,  4,  4,  4,  4,  4,  4 },
		{ '}',  8,  4,  4,  2,  4,  4,  8 },
	    { '°',  4, 10,  4,  0,  0,  0,  0 },
	    { '~',  0,  0,  8, 21,  2,  0,  0 },
	    { '¨',  0, 10,  0,  0,  0,  0,  0 }
	};
		

// Classe LEDCtrl
LEDCtrl::LEDCtrl()
{
	InitializeLEDSet( ) ;
	DisplayedString = "" ;
	CharNumber = 0 ;
	OneLEDSize = 2 ;
	FgLightOnColor = LtRed ;
	FgLightOffColor = Black ;
	BgColor = Black ;
	PosX = 0 ;
	PosY = 0 ;
}


LEDCtrl::LEDCtrl( String Value,
	         		int OneLEDSize,			// 2 pixels for one LED
	        		 Color FgLightOnColor,
	        		 Color FgLightOffColor,
	        		 Color BgColor ) : OneLEDSize(OneLEDSize), FgLightOnColor(FgLightOnColor), 
	        		 	FgLightOffColor(FgLightOffColor), BgColor(BgColor)
{
	InitializeLEDSet( ) ;

	DisplayedString = Value ;
	CharNumber = DisplayedString.GetLength( ) ;
	PosX = 0 ;
	PosY = 0 ;
}

LEDCtrl& LEDCtrl::SetString( String Value ) {
	DisplayedString = Value ;
	CharNumber = DisplayedString.GetLength( ) ;
	return *this;
}

LEDCtrl::~LEDCtrl()
{
	LEDSet.Clear( ) ;
	UnknownLED.Clear( ) ;
}


void LEDCtrl::InitializeLEDSet( )
{
	int TabSize = sizeof( LEDDefinition ) / sizeof( LEDCharacter ) ;
	LEDSet.Clear( ) ;
	for( int Index = 0 ; Index < TabSize ; Index++ )
	{
		Array<int> IntTab ;
		for( int Line = 0 ; Line < LED_VALUES_PER_CHARACTER ; Line++ )
		{
			IntTab.Add( LEDDefinition [Index].Values [Line] ) ;
		}
		LEDSet.Add( Format( "%c", LEDDefinition [Index].Character ), IntTab ) ;
	}
	// And also for one unkonwn character
	for( int Line = 0 ; Line < LED_VALUES_PER_CHARACTER ; Line++ )
	{
		UnknownLED.Add( 31 ) ;		// All LEDs light on
	}
	
	return ;
}


Array<bool> LEDCtrl::DecomposeLine( int Value )
{
	Array<bool> TheDigits ;

    TheDigits.Clear( ) ;
    TheDigits.Add( ( ( Value & 16 ) >> 4 ) ) ;
    TheDigits.Add( ( ( Value &  8 ) >> 3 ) ) ;
    TheDigits.Add( ( ( Value &  4 ) >> 2 ) ) ;
    TheDigits.Add( ( ( Value &  2 ) >> 1 ) ) ;
    TheDigits.Add( ( ( Value &  1 ) >> 0 ) ) ;
    
	return TheDigits ;
}


void LEDCtrl::Paint( Draw& w )
{
	int IndX, IndY ;
	int Depl ;
	int UpPosX, UpPosY ;
	int BottomPosX, BottomPosY ;
	Color ColorOn, ColorOff ;
	Array<int> TheChar ;
	Array<bool> TheDigits ;


	Size sz = GetSize();
	w.DrawRect(0, 0, sz.cx, sz.cy, BgColor);
	
	if( DisplayedString.GetLength( ) == 0 )
	{
		return ;
	}
	
	Depl = 5 ;
    for( int Index = 0 ; Index < CharNumber ; Index++ )
    {
        IndY = 0 ;
        // <<= seems to copy the Array<int>, else = "core". Why ?
        TheChar <<= LEDSet.Get( Format( "%c", DisplayedString [ Index ] ), UnknownLED ) ;
        for( int Line = 0 ; Line < LED_VALUES_PER_CHARACTER ; Line++ )
        {
        	TheDigits = DecomposeLine( TheChar [Line] ) ;
            IndX = 0 ;
            for( int Value = 0 ; Value < TheDigits.GetCount( ) ; Value++ )
            {
                UpPosX = Depl + IndX ;
                UpPosY = OneLEDSize * IndY + 4 ;
                BottomPosX = UpPosX + OneLEDSize ;
                BottomPosY = UpPosY + OneLEDSize ;

                if( TheDigits [Value] == true )
                {
                    ColorOn  = FgLightOnColor ;
                    ColorOff = FgLightOffColor ;
                }
                else
                {
                    ColorOn  = FgLightOffColor ;
                    ColorOff = BgColor ;
                }
				w.DrawRect(UpPosX, UpPosY, OneLEDSize, OneLEDSize, ColorOn);
				w.DrawLine(UpPosX, UpPosY, BottomPosX, UpPosY, 1, ColorOff) ;
				w.DrawLine(UpPosX, BottomPosY, BottomPosX, BottomPosY, 1, ColorOff) ;
				w.DrawLine(UpPosX, UpPosY, UpPosX, BottomPosY, 1, ColorOff) ;
				w.DrawLine(BottomPosX, UpPosY, BottomPosX, BottomPosY, 1, ColorOff) ;
                IndX += OneLEDSize ;
            }
            IndY += 1 ;
        }
        Depl += ( OneLEDSize * 5 ) + 2 ;
    }
	
	return ;
}


LEDCtrl& LEDCtrl::SetDisplayedString( String Value )
{
	DisplayedString = Value ;
	CharNumber = DisplayedString.GetLength( ) ;
	SetPosZ( PosX, PosY ) ;
	Refresh() ;
	return *this ;
}


LEDCtrl& LEDCtrl::SetFgLightOnColor( Color Value )
{
	FgLightOnColor = Value ;
	Refresh() ;
	return *this ;
}


LEDCtrl& LEDCtrl::SetFgLightOffColor( Color Value )
{
	FgLightOffColor = Value ;
	Refresh() ;
	return *this ;
}


LEDCtrl& LEDCtrl::SetBgColor( Color Value )
{
	BgColor = Value ;
	Refresh() ;
	return *this ;
}


LEDCtrl& LEDCtrl::SetStringSize( int CharactersNumber )
{
	CharNumber = CharactersNumber ;
	SetPosZ( PosX, PosY ) ;
	Refresh() ;
	return *this ;
}


LEDCtrl& LEDCtrl::SetLEDSize( int LEDSize )
{
	OneLEDSize = LEDSize ;
	SetPosZ( PosX, PosY ) ;
	Refresh() ;
	return *this ;
}


LEDCtrl& LEDCtrl::SetPosZ( int X, int Y )
{
	int VSize = OneLEDSize * 7 + 8 ;
	int HSize = ( ( OneLEDSize * 5 ) + 2 ) * CharNumber + 8 ;

	LeftPosZ(X, HSize).TopPosZ(Y, VSize) ;
	PosX = X ;
	PosY = Y ;
	
	return *this ;
}



// Classe AnimatedLEDCtrl
AnimatedLEDCtrl::AnimatedLEDCtrl()
	: LEDCtrl( )
{
}


AnimatedLEDCtrl::AnimatedLEDCtrl( String Value,
									LEDAnimationType Type,
									int MaxCharNumber,
									int Timer,				// 1 second by default
									int OneLEDSize,			// 2 pixels for one LED
									Color FgLightOnColor,
									Color FgLightOffColor,
									Color BgColor ) : MaxCharNumber(MaxCharNumber), Timer(Timer),
	Type(Type), LEDCtrl( "", OneLEDSize, FgLightOnColor, FgLightOffColor, BgColor )
{
	CharIndex = 0 ;
	SetString(Value);
}

AnimatedLEDCtrl& AnimatedLEDCtrl::SetString( String Value ) {
	TheSourceString = Value ;
	SetAnimationType( Type ) ;
	return *this;
}


AnimatedLEDCtrl::~AnimatedLEDCtrl()
{
}


AnimatedLEDCtrl& AnimatedLEDCtrl::SetAnimationType( LEDAnimationType Type )
{
	this->Type = Type ;
	TheDisplayedString = "" ;
	switch( Type )
	{
		case LED_SLIDING :
			// The string to be displayed must contain MaxCharNumber spaces + the string Value
			for( int Index = 0 ; Index < MaxCharNumber ; Index++ )
			{
				TheDisplayedString += " " ;
			}

			TheDisplayedString += TheSourceString ;
			DisplayedStringLength = MaxCharNumber + TheSourceString.GetLength( ) ;
				
			SetStringSize( MaxCharNumber ) ;
			SetTimeCallback( (-1) * Timer, THISBACK(DoSliding));
			break ;
			
		case LED_BLINKING :
			TheDisplayedString += TheSourceString ;
			SetStringSize( TheSourceString.GetLength( ) ) ;
			SetTimeCallback( (-1) * Timer, THISBACK(DoBlinking)); 
			break ;
	}
	return *this;
}


void AnimatedLEDCtrl::DoSliding( void )
{
	String TheNewString ;

	TheNewString = TheDisplayedString.Mid( CharIndex, MaxCharNumber ) ;
	int Index = TheNewString.GetLength( ) ;
	while( Index < MaxCharNumber )
	{
		TheNewString += " " ;
		Index++ ;
	}
	SetDisplayedString( TheNewString ) ;
	CharIndex++ ;
	if( CharIndex >= DisplayedStringLength )
	{
		CharIndex = 0 ;
	}
}


void AnimatedLEDCtrl::DoBlinking( void )
{
	static bool State = true ;
	String TheNewString = "" ;

	if( State == true )
	{
		TheNewString += TheDisplayedString.Mid( 0, MaxCharNumber ) ;
		State = false ;
	}
	else
	{
		for( int Index = 0 ; Index < TheDisplayedString.GetLength( ) ; Index++ )
		{
			TheNewString += " " ;
		}
		State = true ;
	}
	
	SetDisplayedString( TheNewString ) ;
	
	return ;
}



