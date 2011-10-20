#ifndef _XMLMenu_XMLMenuInterface_h_
#define _XMLMenu_XMLMenuInterface_h_

#include "XMLToolBarFrame.h"
#include "XMLCommand.h"
#include "XMLToolBar.h"

NAMESPACE_UPP

// interface part -- to support backlink from childs and menu editor
class XMLMenuInterface
{
	private:
	
	protected:
	
	public:
	
		// mouse events sent from floating toolbar
		virtual void FloatingDraggedEvent(XMLToolBarCtrl &tb, Point p) = 0;
		
		// right click event sent from various controls
		virtual void RightClickEvent(Point p) = 0;
		
		// gets/sets commands, menu and toolbars
		virtual XMLCommands const &GetCommands(void) = 0;
		virtual void SetCommands(XMLCommands &cmsd) = 0;
		virtual XMLToolBars const &GetMenuBars(void) = 0;
		virtual void SetMenuBars(XMLToolBars &tb) = 0;
		virtual XMLToolBars const &GetToolBars(void) = 0;
		virtual void SetToolBars(XMLToolBars &tb) = 0;
};

END_UPP_NAMESPACE

#endif
