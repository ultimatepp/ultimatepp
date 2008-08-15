////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MyUppApp_Static_h_
#define _MyUppApp_Static_h_

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <MyUppApp/Static.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS StaticImg
#define IMAGEFILE <MyUppApp/Static.iml>
#include <Draw/iml_header.h>


class Static : public WithStaticLayout<TopWindow> {
public:
	typedef Static CLASSNAME;
	Static();
};
#endif
