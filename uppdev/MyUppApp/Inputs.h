////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MyUppApp_Inputs_h_
#define _MyUppApp_Inputs_h_

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <MyUppApp/Inputs.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS InputsImg
#define IMAGEFILE <MyUppApp/Inputs.iml>
#include <Draw/iml_header.h>


class Inputs : public WithInputsLayout<TopWindow> {
public:
	typedef Inputs CLASSNAME;
	Inputs();
};
#endif
