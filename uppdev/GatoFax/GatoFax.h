#ifndef _GatoFax_GatoFax_h
#define _GatoFax_GatoFax_h

#include <CtrlLib/CtrlLib.h>

#include "TiffCtrl.h"

using namespace Upp;

#define LAYOUTFILE <GatoFax/GatoFax.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <GatoFax/GatoFax.iml>
#include <Draw/iml_header.h>

class GatoMain : public WithMainLayout<TopWindow>
{
	public:
		typedef GatoMain CLASSNAME;
		
		GatoMain();
		
	private:

		MenuBar menu;
		StatusBar status;
		
		// file-exit submenu callback
		void MenuExit(void);
		
		// file-new submenu callback
		void MenuNew(void);
		
		// view submenu callback
		void MenuView(int i);
		
		// main menu items
		void FileMenu(Bar& bar);
		void ViewMenu( Bar& bar );

		// main menu
		void MainMenu(Bar& bar);
		
		// key handler
		virtual bool Key(dword key, int);
		
};

#endif

