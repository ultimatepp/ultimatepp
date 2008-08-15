////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MyUppApp_Splash_h_
#define _MyUppApp_Splash_h_

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <MyUppApp/Splash.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS SplashImg
#define IMAGEFILE <MyUppApp/Splash.iml>
#include <Draw/iml_header.h>


class Splash : public WithSplashLayout<TopWindow> {
	TimeCallback mytimer_;  // a timer variable; new in version 0.0.3

	void closeSplash();
	bool Key(dword key, int count);
public:
	typedef Splash CLASSNAME;
	Splash();
};
#endif
