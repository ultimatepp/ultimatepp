////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
////////////////////////////////////////////////////////////////////////////////////////////////
#include "MyUppApp.h"
#include "Splash.h"
#include "Options.h"
#include "About.h"
#include "Editors.h"
#include "Inputs.h"
#include "Push.h"
#include "Static.h"
#include "Complex.h"

#define IMAGECLASS MyUppAppImg
#define IMAGEFILE <MyUppApp/MyUppApp.iml>
#include <Draw/iml_source.h>

// In TheIDE see Assist; Topic; MyUppApp
#define TOPICFILE <MyUppApp/app.tpp/all.i>
#include <Core/topic_group.h>


// set up the menu options in the menu bar at the top
void
MyUppApp::MainMenu(Bar& menu)
{
	// set menu name; callback function
	menu.Add("File", THISBACK(FileMenu));
	menu.Add("Dialogs", THISBACK(DialogsMenu));
	menu.Add("Tools", THISBACK(ToolsMenu));
	menu.Add("Help", THISBACK(HelpMenu));
}


// set up the File "dropdown" menu item
void
MyUppApp::FileMenu(Bar& menu)
{
	// 1. set menu name; icon; callback function
	// 2. set shortcut key
	// 3. set help text to be displayed in status bar
	menu.Add("Exit", MyUppAppImg::Exit(),THISBACK(exitMyUppApp))
	    .Key(K_CTRL_X)
	    .Help("Quit running this program");
}


// set up the Editors "dropdown" menu items
void
MyUppApp::DialogsMenu(Bar& menu)
{
	// 1. set menu name; icon; callback function
	// 2. set shortcut key
	// 3. set help text to be displayed in status bar
	menu.Add("Editors..", MyUppAppImg::Editors(), THISBACK(showdlgEditors))
	    .Key(K_CTRL_E)
	    .Help("Explore Ultimate++ DocEdit control");

	menu.Add("Inputs..", MyUppAppImg::Inputs(), THISBACK(showdlgInputs))
	    .Key(K_CTRL_I)
	    .Help("Explore Ultimate++ Input controls");

	menu.Add("Push..", MyUppAppImg::Push(), THISBACK(showdlgPush))
	    .Key(K_CTRL_P)
	    .Help("Explore Ultimate++ Push controls");

	menu.Add("Static..", MyUppAppImg::Static(), THISBACK(showdlgStatic))
	    .Key(K_CTRL_S)
	    .Help("Explore Ultimate++ Static controls");

	menu.Add("Complex..", MyUppAppImg::Complex(), THISBACK(showdlgComplex))
	    .Key(K_CTRL_C)
	    .Help("Explore Ultimate++ Complex controls");
}


// set up the Options "dropdown" menu item
void
MyUppApp::ToolsMenu(Bar& menu)
{
	menu.Add("Options..", THISBACK(showdlgToolsOptions)) // add menu option; set callback function
	    .Help("Change program's settings");     // set text to display in status bar
}


// set up the Help "dropdown" menu items
void
MyUppApp::HelpMenu(Bar& menu)
{
	menu.Add("Contents..", THISBACK(showdlgHelp))  // add menu option; set callback function
	    .Key(K_F1)                            // set shortcut key to F1
	    .Help("Display the help file for this program"); // set text to display in status bar
	menu.MenuSeparator();
	menu.Add("About..", THISBACK(showdlgAbout))
	    .Help("Display the About dialog for this program");
}


// set up the toolbar buttons
void
MyUppApp::ToolBarMenu(Bar& menu)
{
	// set image; callback function; tooltip
	menu.Add(MyUppAppImg::Exit(),THISBACK(exitMyUppApp))
	    .Tip("End this program");

	menu.Separator();

	menu.Add(MyUppAppImg::Editors(), THISBACK(showdlgEditors))
	    .Tip("Explore Ultimate++ DocEdit control");

	menu.Add(MyUppAppImg::Inputs(), THISBACK(showdlgInputs))
	    .Tip("Explore Ultimate++ Input controls");

	menu.Add(MyUppAppImg::Push(), THISBACK(showdlgPush))
	    .Tip("Explore Ultimate++ Push controls");

	menu.Add(MyUppAppImg::Static(), THISBACK(showdlgStatic))
	    .Tip("Explore Ultimate++ Static controls");

	menu.Add(MyUppAppImg::Complex(), THISBACK(showdlgComplex))
	    .Tip("Explore Ultimate++ Complex controls");

	menu.Separator();

	menu.Add(MyUppAppImg::Options(), THISBACK(showdlgToolsOptions))
	    .Tip("Set MyUppApp options");

	menu.Separator();

	menu.Add(MyUppAppImg::Help(), THISBACK(showdlgHelp))
	    .Tip("Show MyUppApp help");

	menu.Add(MyUppAppImg::About(), THISBACK(showdlgAbout))
	    .Tip("Show MyUppApp About dialog");

}


// set up the pop-up menu items
void
MyUppApp::PopupMenu(Bar& menu)
{
	// set menu name; callback function
	menu.Add("Exit", THISBACK(exitMyUppApp));
	menu.MenuSeparator();
	menu.Add("Editors..", THISBACK(showdlgEditors));
	menu.Add("Input..", THISBACK(showdlgInputs));
	menu.Add("Push..", THISBACK(showdlgPush));
	menu.Add("Static..", THISBACK(showdlgStatic));
	menu.Add("Complex..", THISBACK(showdlgComplex));
	menu.MenuSeparator();
	menu.Add("Options..", THISBACK(showdlgToolsOptions));
	menu.MenuSeparator();
	menu.Add("Help..", THISBACK(showdlgHelp));
	menu.Add("About..", THISBACK(showdlgAbout));
}


// display the popup menu. Note: RightDown is a U++ function.
void
MyUppApp::RightDown(Point, dword)
{
	MenuBar::Execute(THISBACK(PopupMenu));
}


// stop this program
void
MyUppApp::exitMyUppApp()
{
	Break(IDOK);
}


// show the editors dialog
void
MyUppApp::showdlgEditors()
{
	Editors ed;
	ed.Run();
}


// not yet implemented
void
MyUppApp::showdlgInputs()
{
	Inputs inp;
	inp.Run();
}


// not yet implemented
void
MyUppApp::showdlgPush()
{
	Push p;
	p.Run();
}


// not yet implemented
void
MyUppApp::showdlgStatic()
{
	Static s;
	s.Run();
}


// not yet implemented
void
MyUppApp::showdlgComplex()
{
	Complex c;
	c.Run();
}


// show the Options dialog
void
MyUppApp::showdlgToolsOptions()
{
	Options op;
	op.Run();
}


// show the U++ help system
void
MyUppApp::showdlgHelp()
{
	HelpWindow help;
	help.GoTo("topic://MyUppApp/app/help$en-us");
	help.Execute();
}


// show the program's About dialog
void
MyUppApp::showdlgAbout()
{
	About a;
	a.Run();
}


// checks program's configuration
void
MyUppApp::checkConfig()
{
	if ( FileExists(cfgfile) ) {                                // if there is a config file
		programcfg_ = LoadIniFile(cfgfile);                     // load it
		mainwdontsplash_ = ScanInt(programcfg_.Get("Splash", Null)); // get value of Splash
    	mainwposx_ = ScanInt(programcfg_.Get("PosX", Null));         // get value of PosX
    	mainwposy_ = ScanInt(programcfg_.Get("PosY", Null));         // get value of PosY

	}
	else {                                           // if no config file
		mainwdontsplash_ = 0;                        // enable Splash screen
		String newcfg;
		newcfg <<  "Splash=" << mainwdontsplash_ << "\n"  // save intial config value
				"SaveWinPos=" << 0 << "\n"                // save intial config value
				"PosX=" << 0 << "\n"                      // save intial config value
				"PosY=" << 0 << "\n";                     // save intial config value
		if( !SaveFile(cfgfile, newcfg) )                  // if save fails
			Exclamation("Error saving configuration!");   // display error message
	}
}


// the program's constructor
MyUppApp::MyUppApp()
{
	// get the configuration values or set initial default values
	checkConfig();

	// show the splash screen if mainwdontsplash is not set to 1
    if (mainwdontsplash_ == 0) {
		Splash s;
		s.Run();
    }

	// set up the main window
	CtrlLayout(*this, "MyUppApp: Controls Explorer");
	MinimizeBox();
	Icon(MyUppAppImg::Small(),MyUppAppImg::Big());

	// get the size of this main window
	Size scp = GetSize();

	// set the position of this main window
	// only if config values are valid ie more than zero
	if (mainwposx_ > 0 && mainwposy_ > 0)
		SetRect(mainwposx_, mainwposy_, scp.cx, scp.cy);

	// add the menu and toolbar and set their callback function
	AddFrame(menu_);
	AddFrame(tb_);
	menu_.Set(THISBACK(MainMenu));
	tb_.Set(THISBACK(ToolBarMenu));

	// add the status bar and show a default text
	AddFrame(sb_);
	AddFrame(InsetFrame());
	sb_ = "Ready!";

	// set the status bar to display menu option help when needed
	menu_.WhenHelp = sb_;
}


// this is where the program starts
GUI_APP_MAIN
{
	MyUppApp().Run();
}

// end of file
