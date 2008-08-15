////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MyUppApp_Push_h_
#define _MyUppApp_Push_h_

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <MyUppApp/Push.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS PushImg
#define IMAGEFILE <MyUppApp/Push.iml>
#include <Draw/iml_header.h>


class Push : public WithPushLayout<TopWindow> {
public:
	typedef Push CLASSNAME;
	Push();
};
#endif
