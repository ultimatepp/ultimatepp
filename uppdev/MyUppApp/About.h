////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MyUppApp_About_h_
#define _MyUppApp_About_h_

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <MyUppApp/About.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS AboutImg
#define IMAGEFILE <MyUppApp/About.iml>
#include <Draw/iml_header.h>


class About : public WithAboutLayout<TopWindow> {
	bool Key(dword key, int count);  // respond to key press
public:
	typedef About CLASSNAME;
	About();
};
#endif
