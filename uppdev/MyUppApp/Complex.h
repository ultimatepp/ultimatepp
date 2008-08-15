////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MyUppApp_Complex_h_
#define _MyUppApp_Complex_h_

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <MyUppApp/Complex.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS ComplexImg
#define IMAGEFILE <MyUppApp/Complex.iml>
#include <Draw/iml_header.h>


class Complex : public WithComplexLayout<TopWindow> {
public:
	typedef Complex CLASSNAME;
	Complex();
};
#endif
