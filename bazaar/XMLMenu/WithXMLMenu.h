#ifndef _WithXMLMenu_h_
#define _WithXMLMenu_h_

#include "XMLMenuInterface.h"
#include "XMLMenuEditor.h"

NAMESPACE_UPP

// main XML menu template
template<class T> class WithXMLMenu : public T, public XMLMenuInterface
{
	private:
	
		// the available commands
		XMLCommands commands;
	
		// the configurable toolbars
		XMLToolBars toolBars;
		
		// the configurable menu bars
		// the bar named 'Main' is the main menu
		// others are context menus
		XMLToolBars menuBars;
	
		// the four corner XMLToolbarFrame frames
		XMLToolBarFrame topFrame;
		XMLToolBarFrame bottomFrame;
		XMLToolBarFrame leftFrame;
		XMLToolBarFrame rightFrame;
		
		// the menu bars
		Array<MenuBar> menuBarCtrls;
	
		// toolbar controls storage area
		Array<XMLToolBarCtrl> toolBarCtrls;
		
		// flags stating allowed dock point and main menu bar
		bool dockTop, dockBottom, dockLeft, dockRight;
		bool mainMenu;
		
		// refrash frames when menu/toolbars changed
		// or docking points changed
		void RefreshFrames(void);
		
		// dragging flag
		bool dragging;

		// toolbar being dragged		
		Ptr<XMLToolBarCtrl> dragToolBar;
		
		// pre-docking stuffs
		Ptr<XMLToolBarFrame> preDockFrame;
		
		// menu and toolbars setter callbacks
		void SetMenuBar0(Bar &bar, int mnIdx, Array<XMLToolBarItem> const *items);
		void SetMenuBar(Bar &bar, int mnIdx) { SetMenuBar0(bar, mnIdx, NULL); }

		void SetToolBar0(Bar &bar, int tbIdx, Array<XMLToolBarItem> const *items);
		void SetToolBar(Bar &bar, int tbIdx) { SetToolBar0(bar, tbIdx, NULL); }
		
		// calls user command handler, passing command ID to it
		void callUserHandler(String const &s) { UserCmdHandler(s); }
		
		// docks/undocks/hide a toolbar
		WithXMLMenu<T> &Reposition(XMLToolBarCtrl *tb, XMLToolBarState state, Point p);
		
		// query a dock frame under screen point
		XMLToolBarFrame *QueryDockFrame(Point p);

		// frame mouse event, captures right click on docking frames
		// allowing to activate 'customize' even without any toolbar displayed
		Image FrameMouseEvent(int event, Point p, int zdelta, dword keyflags);

		// child frame mouse event handler captures the click on toolbar's frames
		virtual void ChildFrameMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);
		
		// mouse events sent from floating toolbar
		virtual void FloatingDraggedEvent(XMLToolBarCtrl &tb, Point p);

		// run the menu editor
		void runEditorCb(void) { XMLMenuEditor(this).RunAppModal(); }
		
		// toggles bar visibility
		void toggleBarCb(int iBar);
		
		// right click context menu
		void ContextMenu(Bar& bar);

		// right click event sent from various controls
		virtual void RightClickEvent(Point p) { MenuBar::Execute(THISBACK(ContextMenu)); }

		// the drag loop
		void DragLoop(Point startP);
		
		// refresh menus and bars
		void RefreshBars(void);
		
		// sync bars from ctrls
		void SyncBars(void);

	public:
	
		typedef WithXMLMenu<T> CLASSNAME;
	
		WithXMLMenu();
		~WithXMLMenu();
		
		// user commands handler
		Callback1<String> UserCmdHandler;
		
		// sets builtin commands
		void SetCommands(Callback1<XMLCommands &> cmds);
		
		// sets menu entries
		void SetMenuBars(Callback1<XMLToolBars &> tb);

		// sets toolbars entries
		void SetToolBars(Callback1<XMLToolBars &> tb);
		
		// gets/sets commands, menu and toolbars
		virtual XMLCommands const &GetCommands(void)	{ return commands; }
		virtual void SetCommands(XMLCommands &cmds)		{ commands = cmds; RefreshBars(); }
		virtual XMLToolBars const &GetMenuBars(void)	{ return menuBars; }
		virtual void SetMenuBars(XMLToolBars &tb)		{ menuBars = tb; RefreshBars(); }
		virtual XMLToolBars const &GetToolBars(void)	{ SyncBars(); return toolBars; }
		virtual void SetToolBars(XMLToolBars &tb)		{ toolBars = tb;   RefreshBars(); }
		
		// controls docking and main menu behaviour
		WithXMLMenu<T> &DockTop(bool b = true)		{ dockTop = b;		RefreshFrames(); return *this; } 
		WithXMLMenu<T> &NoDockTop(void)				{ return DockTop(false); }
		WithXMLMenu<T> &DockBottom(bool b = true)	{ dockBottom = b;	RefreshFrames(); return *this; } 
		WithXMLMenu<T> &NoDockBottom(void)			{ return DockBottom(false); }
		WithXMLMenu<T> &DockLeft(bool b = true)		{ dockLeft = b;		RefreshFrames(); return *this; } 
		WithXMLMenu<T> &NoDockLeft(void)			{ return DockLeft(false); }
		WithXMLMenu<T> &DockRight(bool b = true)	{ dockRight = b;	RefreshFrames(); return *this; } 
		WithXMLMenu<T> &NoDockRight(void)			{ return DockRight(false); }
		
		WithXMLMenu<T> &MainMenu(bool b = true)		{ mainMenu = b; RefreshFrames(); return *this; }
		WithXMLMenu<T> &NoMainMenu(void)			{ return MainMenu(false); }
		
		// gets a context menu by name -- NULL if none
		MenuBar *GetContextMenu(String const &name);
		
		// xml support
		void Xmlize(XmlIO xml);
};

template<class T> WithXMLMenu<T>::WithXMLMenu() :
	topFrame		(TOOLBAR_TOP),
	bottomFrame		(TOOLBAR_BOTTOM),
	leftFrame		(TOOLBAR_LEFT),
	rightFrame		(TOOLBAR_RIGHT)
{
DLOG("XMLMenu : " << FormatHex(this));
	// allows, by default, docking on 4 corners and embeds
	// main menu too
	dockTop = dockBottom = dockLeft = dockRight = true;
	mainMenu = true;
	
	dragging = false;
	dragToolBar = NULL;
	
	preDockFrame = NULL;

	RefreshFrames();
	
}

template<class T> WithXMLMenu<T>::~WithXMLMenu()
{
}

// refrash frames when menu/toolbars changed
// or docking points changed
template<class T> void WithXMLMenu<T>::RefreshFrames(void)
{
	// removes all frames
//	T::ClearFrames();
	T::RemoveFrame(topFrame);
	T::RemoveFrame(bottomFrame);
	T::RemoveFrame(leftFrame);
	T::RemoveFrame(rightFrame);
	
	// add docking frames
	if(dockTop)
		T::InsertFrame(0, topFrame);
	if(dockBottom)
		T::InsertFrame(0, bottomFrame);
	if(dockLeft)
		T::InsertFrame(0, leftFrame);
	if(dockRight)
		T::InsertFrame(0, rightFrame);

	// adds main menu
	if(mainMenu)
	{
		int mainIdx = menuBars.Find("Main");
		if(mainIdx >= 0)
		{
			MenuBar &bar = menuBarCtrls[mainIdx];
			T::RemoveFrame(bar);
			T::InsertFrame(0, bar);
		}
	}

}
		

// docks/undocks/hide a toolbar
template<class T> WithXMLMenu<T> &WithXMLMenu<T>::Reposition(XMLToolBarCtrl *tb, XMLToolBarState state, Point p)
{
	switch(state)
	{
		case TOOLBAR_CLOSED :
			tb->Close();
			tb->toolBarPos = p;
			break;
			
		case TOOLBAR_FLOATING :
			tb->Float(p);
			break;
			
		case TOOLBAR_TOP :
			tb->Dock(topFrame, p);
			break;
			
		case TOOLBAR_BOTTOM :
			tb->Dock(bottomFrame, p);
			break;
			
		case TOOLBAR_LEFT :
			tb->Dock(leftFrame, p);
			break;
			
		case TOOLBAR_RIGHT :
			tb->Dock(rightFrame, p);
			break;
			
		default:
			NEVER();
	}
	return *this;
}

// query a dock frame under point
template<class T> XMLToolBarFrame *WithXMLMenu<T>::QueryDockFrame(Point p)
{
	// reset point to be on control rect
	if(dockTop && topFrame.Contains(p))
		return &topFrame;
	else if(dockBottom && bottomFrame.Contains(p))
		return &bottomFrame;
	else if(dockLeft && leftFrame.Contains(p))
		return &leftFrame;
	else if(dockRight && rightFrame.Contains(p))
		return &rightFrame;
	else
		return NULL;
}

// frame mouse event, captures right click on docking frames
// allowing to activate 'customize' even without any toolbar displayed
template<class T> Image WithXMLMenu<T>::FrameMouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	// handles just left down events
	if(event != Ctrl::RIGHTDOWN)
		return T::FrameMouseEvent(event, p, zdelta, keyflags);

	Ctrl::PostCallback(THISBACK1(RightClickEvent, p));
	return T::FrameMouseEvent(event, p, zdelta, keyflags);
}

// child frame mouse event handler captures the click on toolbar's frames
template<class T> void WithXMLMenu<T>::ChildFrameMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	// allow context menu even on child frames
	if(event == Ctrl::RIGHTDOWN)
	{
		Ctrl::PostCallback(THISBACK1(RightClickEvent, p));
		return;
	}

	// handles just left down events
	if(event != Ctrl::LEFTDOWN)
		return;
	
	// ignore re-entrant events
	if(dragging)
		return;

	// reacts just to events related to XMLToolBar
	dragToolBar = dynamic_cast<XMLToolBarCtrl *>(child);
	if(!dragToolBar)
		return;

	// setup drag point for dragging the toolbar
	Point dragPoint = p + dragToolBar->GetScreenRect().TopLeft();
	
	// execute the drag loop
	DragLoop(dragPoint);
}

// mouse events sent from floating toolbar
template<class T> void WithXMLMenu<T>::FloatingDraggedEvent(XMLToolBarCtrl &tb, Point p)
{
	// ignore event if toolbar is not floating
	// should not happen, but.....
	if(!tb.GetIsFloating())
		return;
	
	// ignode re-entrant events
	if(dragging)
		return;

	// setup drag toolbar
	dragToolBar = &tb;
	
	// run drag loop
	DragLoop(p);
}

template<class T> void WithXMLMenu<T>::DragLoop(Point dragPoint)
{
	// marks dragging active to avoid re-entrant events
	dragging = true;
	
	// check if dragtoolbar is valid -- interna error if not
	ASSERT(dragToolBar);
	
	// ignore mouse events on main control, when dragging
	Ctrl::IgnoreMouse();
	
	// popup-ize the toolbar
	dragToolBar->Popup(dragPoint);

#ifdef PLATFORM_POSIX
	// this is needed for X11, otherwise the GetMouseLeft() function
	// may return false even if mouse is down after un-floating
	Sleep(30);
	dragToolBar->SetCapture();
#endif

	// loop up to mouse button is released
	Point ps;
	do
	{
		Ctrl::ProcessEvents();
		ps = GetMousePos();
		Rect r = dragToolBar->GetRect();
		r = r + ps - dragPoint;
		dragPoint = ps;
		dragToolBar->SetRect(r);
		
		// query an eventual dock frame under mouse
		if(preDockFrame)
			preDockFrame->UnPreDock(*dragToolBar);
		preDockFrame = QueryDockFrame(ps);
		if(preDockFrame)
		{
			XMLToolBarState state = preDockFrame->GetToolBarState();
			if(state == TOOLBAR_TOP || state == TOOLBAR_BOTTOM)
				dragToolBar->PopHorz(ps);
			else
				dragToolBar->PopVert(ps);
			preDockFrame->PreDock(*dragToolBar, ps);
		}
		else
			dragToolBar->PopSquare(ps);
		Ctrl::RefreshLayout();
	}
	while(GetMouseLeft());
	
#ifdef PLATFORM_POSIX
	dragToolBar->ReleaseCapture();
#endif

	// if dropped on a frame, dock there
	if(preDockFrame)
	{
		dragToolBar->UnPreDock(*preDockFrame);
		dragToolBar->DockAt(*preDockFrame, ps);
	}
	else
	{
		// otherwise float it
		dragToolBar->Float(ps);
	}

	// re-accepts mouse events
	Ctrl::NoIgnoreMouse();

	// end dragging mode
	dragging = false;
}

// menu and toolbars setter callbacks
template<class T> void WithXMLMenu<T>::SetMenuBar0(Bar &bar, int mnIdx, Array<XMLToolBarItem> const *items)
{
	// get menu bar items
	if(!items)
		items = &menuBars[mnIdx].GetItems();
	
	for(int i = 0; i < items->GetCount(); i++)
	{
		// get current item
		XMLToolBarItem const &item = (*items)[i];
		
		// submenu handling
		if(item.IsSubMenu())
		{
			bar.Add(item.GetLabel(), item.GetIcon(), THISBACK2(SetMenuBar0, mnIdx, &item.GetSubMenu().GetItems()));
			continue;
		}

		// get command id and find command
		// if not found, just skip it
		String id = item.GetId();
		if(!commands.Has(id))
			continue;
		
		// get command from id
		XMLCommand const &cmd = commands.Get(id);
		
		// get callback from command
		bool isCustom = cmd.GetIsCustom();
		Callback handler;
		if(isCustom)
			handler = THISBACK1(callUserHandler, id);
		else
			handler = cmd.GetCallback();
		
		// get remaining item data
		String label = item.GetLabel();
		if(label == "")
			label = id;
		Image const &img = item.GetIcon();
		
		Ctrl *ctrl = cmd.GetCtrl();
		bool enabled = cmd.GetIsEnabled();
		
		// adds to menu
		if(ctrl)
		{
			if(cmd.GetCtrlSize() == Size(-1, -1))
				bar.Add(enabled, *ctrl);
			else
				bar.Add(enabled, *ctrl, cmd.GetCtrlSize());
		}
		else
			bar.Add(enabled, label, img, handler);
	}
}

template<class T> void WithXMLMenu<T>::SetToolBar0(Bar &bar, int tbIdx, Array<XMLToolBarItem> const *items)
{
	// get menu bar items
	if(!items)
		items = &toolBars[tbIdx].GetItems();
	
	for(int i = 0; i < items->GetCount(); i++)
	{
		// get current item
		XMLToolBarItem const &item = (*items)[i];
		
		// submenu handling
		if(item.IsSubMenu())
		{
			bar.Add(item.GetLabel(), item.GetIcon(), THISBACK2(SetToolBar0, tbIdx, &item.GetSubMenu().GetItems()));
			continue;
		}

		// get command id and find command
		// if not found, just skip it
		String id = item.GetId();
		if(!commands.Has(id))
			continue;
		
		// get command from id
		XMLCommand const &cmd = commands.Get(id);
		
		// get callback from command
		bool isCustom = cmd.GetIsCustom();
		Callback handler;
		if(isCustom)
			handler = THISBACK1(callUserHandler, id);
		else
			handler = cmd.GetCallback();
		
		// get remaining item data
		String label = item.GetLabel();
		if(label == "")
			label = id;
		Image const &img = item.GetIcon();
		
		Ctrl *ctrl = cmd.GetCtrl();
		bool enabled = cmd.GetIsEnabled();
		
		// adds to menu
		if(ctrl)
		{
			if(cmd.GetCtrlSize() == Size(-1, -1))
				bar.Add(enabled, *ctrl);
			else
				bar.Add(enabled, *ctrl, cmd.GetCtrlSize());
		}
		else
			bar.Add(enabled, label, img, handler);
	}
}

// refresh menus and bars
template<class T> void WithXMLMenu<T>::RefreshBars(void)
{
//	T::ClearFrames();
	T::RemoveFrame(topFrame);
	T::RemoveFrame(bottomFrame);
	T::RemoveFrame(leftFrame);
	T::RemoveFrame(rightFrame);
	for(int iBar = 0; iBar < menuBarCtrls.GetCount(); iBar++)
		T::RemoveFrame(menuBarCtrls[iBar]);

	// refresh menus
	menuBarCtrls.Clear();
	for(int iBar = 0; iBar < menuBars.GetCount(); iBar++)
	{
		menuBarCtrls.Add(new MenuBar);

		// workaround for main menu... without this one
		// it appears as a popup menu
		if(menuBars[iBar].GetName() == "Main")
			T::AddFrame(menuBarCtrls[iBar]);

		menuBarCtrls[iBar].Set(THISBACK1(SetMenuBar, iBar));
		T::RemoveFrame(menuBarCtrls[iBar]);
	}
	
	// refresh toolbars
	toolBarCtrls.Clear();
	for(int iBar = 0; iBar < toolBars.GetCount(); iBar++)
	{
		XMLToolBar &toolBar = toolBars[iBar];
		toolBarCtrls.Add(new XMLToolBarCtrl(this));
		XMLToolBarCtrl &toolBarCtrl = toolBarCtrls.Top();
		toolBarCtrls[iBar].Set(THISBACK1(SetToolBar, iBar));
		Reposition(&toolBarCtrl, toolBar.GetState(), toolBar.GetPosition());
		toolBarCtrls[iBar].SetPrevState(toolBar.GetPrevState());
	}
	
	// refresh frames
	RefreshFrames();
}

// sync bars from ctrls
template<class T> void WithXMLMenu<T>::SyncBars(void)
{
	for(int i = 0; i < toolBars.GetCount(); i++)
	{
		XMLToolBarCtrl const &c = toolBarCtrls[i];
		Point p = c.GetPosition();
		XMLToolBarState state = c.GetState();
		XMLToolBarState prevState = c.GetPrevState();
		toolBars[i].SetPosition(p).SetState(state).SetPrevState(prevState);
	}
}

// sets builtin commands
template<class T> void WithXMLMenu<T>::SetCommands(Callback1<XMLCommands &> cmds)
{
	// setup commands
	cmds(commands);
	
	// refresh bars and frames
	RefreshBars();
}

// sets menu entries
template<class T> void WithXMLMenu<T>::SetMenuBars(Callback1<XMLToolBars &> tb)
{
	tb(menuBars);

	// refresh toolbars
	RefreshBars();
}

// sets toolbars entries
template<class T> void WithXMLMenu<T>::SetToolBars(Callback1<XMLToolBars &> tb)
{
	tb(toolBars);

	// refresh toolbars
	RefreshBars();
}

// gets a context menu by name -- NULL if none
template<class T> MenuBar *WithXMLMenu<T>::GetContextMenu(String const &name)
{
	int idx = menuBars.Find(name);
	if(idx < 0)
		return NULL;
	return &menuBarCtrls[idx];
}

// xml support
template<class T> void WithXMLMenu<T>::Xmlize(XmlIO xml)
{
	if(xml.IsStoring())
		SyncBars();
	xml
		("commands"		, commands)
		("menubars"		, menuBars)
		("toolbars"		, toolBars)
	;
	// here too, we use postcallback because we must
	// wait that top window is opened before refreshing toolbars
	// don't know if it's the right way, but.....
	if(xml.IsLoading())
		T::PostCallback(THISBACK(RefreshBars));
}

// toggles bar visibility
template<class T> void WithXMLMenu<T>::toggleBarCb(int iBar)
{
	XMLToolBarCtrl &tb = toolBarCtrls[iBar];
	if(tb.GetState() == TOOLBAR_CLOSED)
	{
		switch(tb.prevState)
		{
			case TOOLBAR_LEFT :
			case TOOLBAR_RIGHT :
			case TOOLBAR_TOP :
			case TOOLBAR_BOTTOM :
			case TOOLBAR_FLOATING :
				Reposition(&tb, tb.prevState, tb.toolBarPos);
				break;
			default:
				Reposition(&tb, TOOLBAR_FLOATING, tb.toolBarPos);
		}
	}
	else
		tb.CloseBar();
	T::RefreshLayout();
}
		
// right click context menu
template<class T> void WithXMLMenu<T>::ContextMenu(Bar& bar)
{
	// add customize command
	bar.Add(t_("Customize"), THISBACK(runEditorCb));
	
	// allow open/close bars
	if(!toolBars.GetCount())
		return;
	bar.Separator();
	for(int iBar = 0; iBar < toolBars.GetCount(); iBar++)
	{
		XMLToolBarCtrl &toolBarCtrl = toolBarCtrls[iBar];
		String const &barName = toolBars.GetKey(iBar);
		Bar::Item &item = bar.Add(barName, THISBACK1(toggleBarCb, iBar));
		item.Check(toolBarCtrl.GetState() != TOOLBAR_CLOSED);
	}
}

END_UPP_NAMESPACE

#endif
