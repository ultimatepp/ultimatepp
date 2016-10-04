////////////////////////////////////////////////////////////////////////////////////////////////
// Program   : MyUppApp.exe
// Version   : 0.0.3 Alpha
// Date      : November 2006
// Copyright : Copyright(C) 2006 by Yeoh HS
// Author    : Yeoh HS
// Note      : C++ style guide at http://geosoft.no/development/cppstyle.html
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MyUppApp_MyUppApp_h
#define _MyUppApp_MyUppApp_h

#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <MyUppApp/MyUppApp.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MyUppAppImg
#define IMAGEFILE <MyUppApp/MyUppApp.iml>
#include <Draw/iml_header.h>

class MyUppApp : public WithMyUppAppLayout<TopWindow> {
private:
	StatusBar    sb_;              // status bar object
	MenuBar      menu_;            // top menu object
	ToolBar      tb_;              // toolbar object

	VectorMap<String, String> programcfg_; // configuration value pairs
	int mainwdontsplash_;                  // configuration value for Splash screen
	int mainwposx_;                        // X position of main window
	int mainwposy_;                        // Y position of main window


	void MainMenu(Bar& menu);      // set up Main menu
	void FileMenu(Bar& menu);      // set up File menu
	void DialogsMenu(Bar& menu);   // set up Dialogs menu
	void ToolsMenu(Bar& menu);     // set up Tools menu
	void HelpMenu(Bar& menu);      // set up Help menu
	void ToolBarMenu(Bar& menu);   // set up popup menu
	void PopupMenu(Bar& menu);     // set up popup menu

	void RightDown(Point, dword);   // respond to right mouse click. Note: RightDown is a U++ function.
	void checkConfig();	            // checks program's configuration
	void exitMyUppApp();            // stop this program
	void showdlgEditors();          // display Editors dialog
	void showdlgInputs();           // not yet implemented
	void showdlgPush();             // not yet implemented
	void showdlgStatic();           // not yet implemented
	void showdlgComplex();          // not yet implemented
	void showdlgToolsOptions();     // display Tools dialog
	void showdlgHelp();             // display Help
	void showdlgAbout();            // display About dialog

public:
	typedef MyUppApp CLASSNAME;
	MyUppApp();
};

#endif
