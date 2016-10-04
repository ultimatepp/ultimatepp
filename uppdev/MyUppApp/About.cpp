////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#include "About.h"

#define IMAGECLASS AboutImg
#define IMAGEFILE <MyUppApp/About.iml>
#include <Draw/iml_source.h>

// If the "Enter" key is pressed, close this window.
bool
About::Key(dword key, int count)
{
	if (key == K_ENTER)
		Break(IDOK);
	return true;
}

// This displays the "About" window with icon, the About text and image.
About::About()
{
	CtrlLayout(*this,"About");
	Icon(AboutImg::About());
	rtvAbout = GetTopic("topic://MyUppApp/app/About$en-us").text;
}
// end of file
