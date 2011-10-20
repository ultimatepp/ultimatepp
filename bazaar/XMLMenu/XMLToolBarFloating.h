#ifndef _XMLToolBarFloating_h_
#define _XMLToolBarFloating_h_

#include "DragWindow.h"

NAMESPACE_UPP

// container for floating toolbars
class XMLToolBarCtrl;
class XMLToolBarFloating : public DragWindow
{
	friend class XMLToolBarCtrl;
	
	private:
	
		Ptr<XMLToolBarCtrl> toolBar;

		// handler for window drag events
		virtual void WindowDragged(DragWindow::DragEvent e, Point p);
		
		// constructor
		XMLToolBarFloating(XMLToolBarCtrl &tb, Point p);
		
		// close handler -- if done manually (closing window)
		// sets toolbar state to TOOLBAR_CLOSED
		void Close(void);

	public:
		~XMLToolBarFloating();
};

END_UPP_NAMESPACE

#endif
