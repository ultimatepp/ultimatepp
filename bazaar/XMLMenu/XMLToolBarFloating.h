#ifndef _XMLToolBarFloating_h_
#define _XMLToolBarFloating_h_

#include "DragWindow.h"

NAMESPACE_UPP

// container for floating toolbars
class XMLToolBar;
class XMLToolBarFloating : public DragWindow
{
	private:
	
		Ptr<XMLToolBar> toolBar;
		Ptr<Ctrl> focusCtrl;

	protected:
	
		// handler for window drag events
		virtual void WindowDragged(DragWindow::DragEvent e, Point p);
		
	public:
	
		// handle close event
		virtual void Close(void);
	
		XMLToolBarFloating(XMLToolBar &tb, Point p);
		~XMLToolBarFloating();
};

END_UPP_NAMESPACE

#endif
