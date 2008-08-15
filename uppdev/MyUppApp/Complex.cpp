////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#include "Complex.h"

#define IMAGECLASS ComplexImg
#define IMAGEFILE <MyUppApp/Complex.iml>
#include <Draw/iml_source.h>


// do the necessary setup work in the constructor
Complex::Complex()
{
	CtrlLayout(*this,"Complex");
	Icon(ComplexImg::Complex()); // Set an icon for the window
}
// end of file
