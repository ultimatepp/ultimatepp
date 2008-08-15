////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#include "Push.h"

#define IMAGECLASS PushImg
#define IMAGEFILE <MyUppApp/Push.iml>
#include <Draw/iml_source.h>


// do the necessary setup work in the constructor
Push::Push()
{
	CtrlLayout(*this,"Push");
	Icon(PushImg::Push()); // Set an icon for the window
}
// end of file
