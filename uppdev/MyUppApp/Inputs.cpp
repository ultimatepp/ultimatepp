////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#include "Inputs.h"

#define IMAGECLASS InputsImg
#define IMAGEFILE <MyUppApp/Inputs.iml>
#include <Draw/iml_source.h>


// do the necessary setup work in the constructor
Inputs::Inputs()
{
	CtrlLayout(*this,"Inputs");
	Icon(InputsImg::Inputs()); // Set an icon for the window
}
// end of file
