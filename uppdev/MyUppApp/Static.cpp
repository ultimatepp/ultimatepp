////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#include "Static.h"

#define IMAGECLASS StaticImg
#define IMAGEFILE <MyUppApp/Static.iml>
#include <Draw/iml_source.h>


// do the necessary setup work in the constructor
Static::Static()
{
	CtrlLayout(*this,"Static");
	Icon(StaticImg::Static()); // Set an icon for the window
}
// end of file
