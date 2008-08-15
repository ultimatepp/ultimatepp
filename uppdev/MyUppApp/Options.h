////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MyUppApp_Options_h_
#define _MyUppApp_Options_h_

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <MyUppApp/Options.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS OptionsImg
#define IMAGEFILE <MyUppApp/Options.iml>
#include <Draw/iml_header.h>

const String cfgfile = "myuppapp.cfg"; // MyUppApp's configuration file

class Options : public WithOptionsLayout<TopWindow> {
	void onbtnSave();  // get and save settings when Save button is clicked.
public:
	typedef Options CLASSNAME;
	Options();
};
#endif
