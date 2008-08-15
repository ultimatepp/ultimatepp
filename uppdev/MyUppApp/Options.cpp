////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#include "Options.h"

#define IMAGECLASS OptionsImg
#define IMAGEFILE <MyUppApp/Options.iml>
#include <Draw/iml_source.h>

// get user options, process and save.
void
Options::onbtnSave()
{
	int dontsplash = optSplash.Get();    // get the value from option control
	int savewinpos = optSaveWinPos.Get();// get the value from option control

	Ctrl* owner = GetOwner(); // get a handle to the Options dislog owner which is MyUppApp main window.
	Rect rc;                  // create a struct to store window position and size

	if ( savewinpos != 1 ) {  // if Save Window option is not checked.
		rc.left = 0;          // set window position x to 0
		rc.top = 0;           // set window position y to 0
	} else {
		rc = owner->GetRect(); // get the position and size of MyUppApp main window
	}

	String cfg;  // create a string to store config data
	cfg << "Splash="     << dontsplash << "\n"    // save user's choice
	       "SaveWinPos=" << savewinpos << "\n"    // save user's choice
	       "PosX="       << rc.left    << "\n"    // save window x position
	       "PosY="       << rc.top     << "\n";   // save window y position

	if( !SaveFile(cfgfile, cfg) )  // save the config file
		Exclamation("Error saving configuration!"); // if failed show error dialog
	else
	    PromptOK("Options saved!");  // show success dialog

	Break(IDOK); // close this dialog
}


// do setup work here
Options::Options()
{
	CtrlLayout(*this,"Options");
	Icon(OptionsImg::Options());

	// set callback for the save button
	btnSave <<= THISBACK(onbtnSave);

	// get the values from the config file
	VectorMap<String, String> cfg = LoadIniFile(cfgfile);
	int dontsplash = ScanInt(cfg.Get("Splash", Null));
	int savewinpos = ScanInt(cfg.Get("SaveWinPos", Null));

    // set option controls with current value from config file
	optSplash = dontsplash;
	optSaveWinPos = savewinpos;
}
// end of file
