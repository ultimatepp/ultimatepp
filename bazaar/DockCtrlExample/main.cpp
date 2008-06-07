#include "DockCtrlExample.h"

void DockCtrlExample::InitDockCtrl()
{
	// This is an API change (from DEV 803b.1 on).
	// IntDockCtrl() method is called by DockCtrl at the application start.
	// Dock initialization must be done here!
	
	// Magic Stuff...
	// DockableCtrl derived widgets use 3 variable to determine their place: 
	// -----------------------------------------------------------------------------------------
	// 1) Alignment: 	As usual, it can be left, top, right, bottom.
	// 2) State:	 	It can be show, hide, autohide, tab, floating, shut.
	// 3) Position:		Determines the position of the widget, for example 0 means the widget is 
	//					on its own (usually, at floating state, or shut). So, any "docked" widget
	//					is at least in position 1. In normal docking mode (no tabbing) system
	//					adds the dock automatically to the end of the list, so this variable is
	//					optional for the user. 
	// -----------------------------------------------------------------------------------------
	//	Tabbing styles: While any type of manipulation is possible with the above variables,
	//					tabbing is a little different. (From DockCtrl version DEV803b.1 on, there
	//					are two ways to tab two (or more) dockable widgets):
	//
	//				1)	MANUAL TABBING: As the name may suggest, in "manual" tabbing method use
	//					state and position combination to "tab" dockable widgets. 
	//
	//					Let's assume that we have 3 widgets docked on the "top" pane. And let's
	//					say that we want to add two more docks on the top pane. We have three
	//					alternatives: a) We can add it in usual way, and let the framework
	//					handle the positioning. b) We can determine the position to dock.
	//					(e.g. if we want to add it to 2. position, position should be 2). 
	//					c) We can tab docks. Now, to tab a dock, all we have to do is to set the 
	//					two variables properly: state and position. "State" should be "tabbed".
	//					"position" should be the position where we want to tab (e.g. If we want
	//					to tab the first dock then position should be 1).
	// 
	//					This is a framework design issue; because, with this way it is easily possible 
	//					to tab any number of dockable widgets simultaneously. And since it can be 
	//					confusing sometimes, it is a DEPRECEATED way. 					
	//
	//				2)	TABBING USING TABIFYING METHODS: From DockCtrl version DEV803b.1, Tabify()
	//					method (has 1 overload) is added to the public API. This is the easy and
	//					reliable way to create tabbed widgets (either normal or nested).
	//					The Syntax of Tabify is as following;
	//					
	//					DockableCtrl&	Tabify(DockableCtrl& ctrl1, DockableCtrl& ctrl2);
	//					DockableCtrl&	Tabify(DockableCtrl& ctrl1, DockableCtrl& ctrl2, DockableCtrl& ctrl3, DockableCtrl& ctrl4);
	//				
	//					As you can see, Tabify method returns DockableCtrl& reference for the created
	//					tab window, so it's both easy to tab (normal OR nested) numerous widgets simultaneously 
	//					and the standard docking rules apply to the created tab window (see below).
						

	Dock(dock1.SetLabel("Dock 1").DockTop().StateShow().SetIcon(CtrlImg::new_doc()));
	Dock(dock2.SetLabel("Dock 2").DockBottom().StateShow().SetIcon(CtrlImg::new_doc()));
	Dock(dock4.SetLabel("Dock 4").DockRight().StateTabbed().Posit(1).SetIcon(CtrlImg::new_doc()));			// Manual tabbing example
	Dock(dock5.SetLabel("Dock 5").DockRight().StateTabbed().Posit(1).SetIcon(CtrlImg::new_doc()));			//
	Dock(dock6.SetLabel("Dock 6").DockLeft().StateAuto().SetIcon(CtrlImg::new_doc()));						// Manual autohide (no need to set position variable).

	

	// Tabify():
	// Here is two tabbing examples:
	// Do not use Dock() for individual widgets when tabifying.
	// Instead, pass them to the Tabify() method, and then Dock() 
	// the Tabwindow returned by Tabify().
	//
	CtrlLayout(dock7);
	dock7.SetLabel("Dock 7 - With Layout").SetIcon(CtrlImg::new_doc());										// Layout-ed dock example.
	dock3.SetLabel("Dock 3").SetIcon(CtrlImg::new_doc());													
	
	Dock(Tabify(dock3, dock7).DockFloat());																	// Floating + Tabbing example.
	
	// Here is another example. This example is mainly for demostrating
	// "nested tabbing". In order it to work correctly, you shoud
	// remove "Dock()" from the widgets passed to the method. and 
	// uncomment the 2 lines below.
	// 
	// AllowNestedTabbing()
	// Dock(Tabify(dock6, dock7, dock4, dock5).DockFloat()); 
}

void DockCtrlExample::InitCustomLayouts()
{
	// This method is called by DockCtrl for predefined custom layouts/groups.
	
	// Predefined Group Example
	// ------------------------
	// You need to take two simple steps to add a predefined widget group to DockCtrl based application:
	// Step 1: Add the group using WidgetGroup() method.
	// Step 2: Use "%" operator to add ctrls to group. 
	
	WidgetGroup("Predefined Group 1") % dock1 % dock2 % dock3 % dock4;
	WidgetGroup("Predefined Group 2") % dock5 % dock6 % dock7;
	
	// Predefined Layout Example
	// -------------------------
	// NOT IMPLEMENTED YET. TO BE WRITTEN (SOON)...
}


DockCtrlExample::DockCtrlExample()
{
	// DockCtrl is serializable. So, you can load and save it's configuration (layouts, settings, placements, etc.) easily.
	LoadFromFile(*this, ConfigFile("dockctrlexamplelayout.bin"));


	CtrlLayout(*this, "DockCtrl [DEV803b.2] Example - Use 'Settings' menu options to open the control panel");
	Sizeable().Zoomable().SetRect(GetRect().left, GetRect().top, 1024, 600);
	
	// Usual U++ widget preperation.
	button1.SetLabel("Hello").LeftPos(10, 100).BottomPos(10, 20);
	button2.SetLabel("Brave New").HCenterPos(90).BottomPos(10, 20);
	button3.SetLabel("World").RightPos(10, 100).BottomPos(10, 20);
	
	tabctrl.Add("Tab 1");
	tabctrl.Add("Tab 2");
	
	array1.AddColumn("Numbers");
	array2.AddColumn("Roman Numbers");
	
	for(int i = 0; i < 100; i++)
	{
		array1.Add(Format("%d", i));
		array2.Add(FormatIntRoman(i, true));
	}

	// At the moment (this will change in near future!) any default BarCtrl should be added BEFORE DockCtrl. 
	toolbar1.Set(THISBACK(Toolbar));
	menubar1.Set(THISBACK(Menubar));	

	// Here (but not necessarily) we "wrap" -or, if you prefer "dock" - the above widgets.
	dock1 << button1 << button2 << button3;
	dock2 << tabctrl.SizePos();
	dock3 << array1.SizePos();
	dock5 << array2.SizePos();
	
	// Currently all DockableCtrl derived classes (e.g. DockWindow, TabWindow) can use 3 traditional "bar" ctrls of U++. 
	// Namely, MenuBar, ToolBar and StatusBar. The aim of the support for seperate bar controls is to extend the DockableCtrl functionality.
	// So, you should see and use this "bar" support as a customization support for dockctrl framework itself;
	dock2.AddMenuBar(menubar);
	dock2.AddStatusBar(statusbar);
	dock3.AddToolBar(toolbar); 

	menubar.Set(THISBACK(Menubar));	
	toolbar.Set(THISBACK(Toolbar));
	Add(richedit.SizePos());	

	// You can select which sides could be used to dock. See DockBase class 
	// AllowLeftRight();
}

DockCtrlExample::~DockCtrlExample()
{
	StoreToFile(*this, ConfigFile("dockctrlexamplelayout.bin"));
}

void DockCtrlExample::Toolbar(Bar &bar)
{
	bar.Add(CtrlImg::open(),THISBACK(Click));
	bar.Add(CtrlImg::new_doc(),THISBACK(Click));
	bar.Add(CtrlImg::save(),THISBACK(Click));
	bar.Separator();
	bar.Add(CtrlImg::undo(),THISBACK(Click));
	bar.Add(CtrlImg::redo(),THISBACK(Click));
	bar.Add(CtrlImg::paste(),THISBACK(Click));
	bar.Add(CtrlImg::remove(),THISBACK(Click));
	bar.Separator();
	bar.Add(CtrlImg::print(),THISBACK(Click));
}

void DockCtrlExample::Menubar(Bar &bar)
{
	bar.Add("File",THISBACK(WindowsMenu));
	bar.Add("Edit",THISBACK(EditMenu));
	bar.Add("Help",THISBACK(InfoMenu));
}

void DockCtrlExample::WindowsMenu(Bar& bar)
{
	bar.Add("New", CtrlImg::new_doc(), THISBACK(Click));
	bar.Add("Open", CtrlImg::open(),THISBACK(Click));
	bar.Add("Save", CtrlImg::save(), THISBACK(Click));
	bar.Add("Save As...", CtrlImg::save_as(),THISBACK(Click));
	bar.Separator();
	bar.Add("Exit", THISBACK(Click));
}

void DockCtrlExample::InfoMenu(Bar& bar)
{
	bar.Add("Help Topics", CtrlImg::help(), THISBACK(Click));
	bar.Add("About...", THISBACK(Click));
}

void DockCtrlExample::EditMenu(Bar& bar)
{
	bar.Add("Undo", CtrlImg::undo(), THISBACK(Click));
	bar.Add("Redo", CtrlImg::redo(), THISBACK(Click));
	bar.Separator();
	bar.Add("Paste", CtrlImg::paste(), THISBACK(Click));
	bar.Add("Remove", CtrlImg::remove(), THISBACK(Click));	
}


GUI_APP_MAIN
{
	SetLanguage(::GetSystemLNG());
//	SetDefaultCharset(CHARSET_UTF8);
	DockCtrlExample().Run();
}

