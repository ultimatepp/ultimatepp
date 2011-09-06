#ifndef _DockCtrlExample_DockCtrlExample_h
#define _DockCtrlExample_DockCtrlExample_h

#include <DockCtrl/DockCtrl.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

#define LAYOUTFILE <DockCtrlExample/DockCtrlExample.lay>
#include <CtrlCore/lay.h>

// DockCtrlExample class declaration.
// Derived from DockCtrl.

class DockCtrlExample : public WithDockCtrlExampleLayout<DockCtrl> {
public:
	typedef DockCtrlExample CLASSNAME;
	DockCtrlExample();
	~DockCtrlExample();
	
	DockWindow						dock1, dock2, dock3, dock4, dock5, dock6;					// Actual Widgets 	-	these are the dockable "client" widgets
	WithDockLayout<DockWindow>		dock7;														// DockWindow with layout.
	//------------------------------------------------------------------------------------------
	RichEdit	richedit;																		// Stuff to "wrap"	
	TabCtrl		tabctrl;																		//
	Button		button1, button2, button3;														//
	ArrayCtrl	array1, array2;																	//
	MenuBar 	menubar, menubar1;																//
	ToolBar 	toolbar, toolbar1;																//
	StatusBar 	statusbar;																		//
	//------------------------------------------------------------------------------------------
	virtual void InitDockCtrl();																// DockCtrl initialization functions
	virtual void InitCustomLayouts();															// 
	
	// Other methods (not necessary; only for demo purpose...

	void Toolbar	(Bar &bar);
	void Menubar	(Bar &bar);
	void WindowsMenu(Bar& bar); 
	void InfoMenu	(Bar& bar);
	void EditMenu	(Bar& bar);
	void Click()	{}

	
};

#endif

