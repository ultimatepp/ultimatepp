#ifndef _RasterCtrlTest_h
#define _RasterCtrlTest_h

#include <CtrlLib/CtrlLib.h>

#include "../RasterCtrl/RasterCtrl.h"

using namespace Upp;

class RasterCtrlTestWindow : public TopWindow
{
	public:
		typedef RasterCtrlTestWindow CLASSNAME;
		
		RasterCtrlTestWindow();
		
	private:

		MenuBar menu;
		StatusBar status;
		
		RasterCtrl rasterCtrl;
		
		// file-exit submenu callback
		void Menu_File_Exit_cb(void);
		
		// file-open submenu callback
		void Menu_File_Open_cb(void);
		
		// view submenu callback
		void Menu_View_cb(int i);
		
		// main menu items
		void Menu_File(Bar& bar);
		void Menu_View( Bar& bar );

		// main menu
		void MainMenu(Bar& bar);
		
};

#endif
