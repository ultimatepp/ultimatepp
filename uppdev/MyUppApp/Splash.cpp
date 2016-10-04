////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#include "Splash.h"

#define IMAGECLASS SplashImg
#define IMAGEFILE <MyUppApp/Splash.iml>
#include <Draw/iml_source.h>


// if Enter key is pressed; close the Splash window
bool
Splash::Key(dword key, int count)
{
	if (key == K_ENTER)
		closeSplash();
	return true;
}


// kill the timer and close the Splash window
void
Splash::closeSplash()
{
	//KillTimeCallback(1); // Kill Timer with ID 1 (deprecated)
	mytimer_.Kill();        // new in version 0.0.3
	Close();
}


// the constructor gets splash data from topic; set timer; pop up the splash window
Splash::Splash()
{
	CtrlLayout(*this);

	// get the Splash data from a topic
	rtvSplash = GetTopic("topic://MyUppApp/app/Splash$en-us").text;

	// calculations needed to set the Splash window center on screen
	Size scr = GetMaxSize();
	Size scp = GetSize();
	int posx = scr.cx - scr.cx/2 - scp.cx/2;
	int posy = scr.cy - scr.cy/2 - scp.cy/2;
	int width = scp.cx;
	int height = scp.cy;
	SetRect(posx, posy, width, height);

	// show the Splash window
	PopUp();

	// start the timer running
	//SetTimeCallback(-5000, THISBACK(closeSplash),1) (deprecated)
	mytimer_.Set(5000,THISBACK(closeSplash)); // new in version 0.0.3
}
// end of file
