#ifndef _XMLToolBar_h_
#define _XMLToolBar_h_

#include "XMLToolBar.h"
#include "XMLDragFrame.h"
#include "XMLToolBarFloating.h"

NAMESPACE_UPP

// XMLToolBar class : a container for ToolBar and its drag handle
class XMLToolBarFrame;
class XMLMenuInterface;
class XMLToolBarCtrl : public ParentCtrl
{
	template<class T> friend class WithXMLMenu;
	friend class XMLToolBarFrame;
	friend class XMLToolBarFloating;

	private:

		// just to access BarPane::GetSize()...
		struct SizeToolBar : ToolBar
		{
			SizeToolBar() { Gap(0); ToolGap(0); pane.Margin(0, 0); }
			Size GetPaneSize(bool horz, int max = INT_MAX) { return pane.GetPaneSize(horz, max); }
		};

		// the embedded toolbar
		SizeToolBar toolBar;
		
		// drag frame
		XMLDragFrame dragFrame;
		
		// pointer containing owning frame, if any
		Ptr<XMLToolBarFrame> toolBarFrame;
		
		// container when floating, if any
		One<XMLToolBarFloating> floating;
		
		// interface of owning WithXMLMenu, if any
		XMLMenuInterface *iFace;
		
		// current or last docking status of this toolbar
		XMLToolBarState toolBarState, prevState;
		Point toolBarPos;
		
		// lays toolbar and handle inside control
		virtual void Layout(void);
		
		// gets toolbar align
		int GetAlign(void) const;
		
		typedef XMLToolBarCtrl CLASSNAME;

		XMLMenuInterface *GetInterface(void) const { return iFace; }
		
		// gets toolbar size when completely inlined horizontally
		virtual Size GetHorzSize(void);
		
		// gets toolbar size when completely inlined vertically
		virtual Size GetVertSize(void);
		
		// gets toolbar size when squared
		Size GetSquaredSize(void);
		
		// get docked state
		bool GetIsDocked() const;
		
		// get floating state
		bool GetIsFloating() const;
		
		// get opened state
		bool GetIsOpened() const;
		
		// get popup state
		bool GetIsPopUp() const;
		
		// sets toolbar state
		XMLToolBarCtrl &SetState(XMLToolBarState state);
		XMLToolBarState GetState(void) const					{ return toolBarState; }
		XMLToolBarCtrl &SetPrevState(XMLToolBarState state)		{ prevState = state; return *this; }
		XMLToolBarState GetPrevState(void) const				{ return prevState; }
		
		// gets bar position
		Point GetPosition(void) const;
		
		// floats the toolbar at a given position
		XMLToolBarFloating &Float(Point p);
		
		// popups the toolbar at a given position
		XMLToolBarCtrl &Popup(Point p);
		XMLToolBarCtrl &PopHorz(Point p);
		XMLToolBarCtrl &PopVert(Point p);
		XMLToolBarCtrl &PopSquare(Point p);
		
		// pre-Dock and pre-Undock a toolbar inside frame
		XMLToolBarCtrl &PreDock(XMLToolBarFrame &f, Point p);
		XMLToolBarCtrl &UnPreDock(XMLToolBarFrame &f);
		
		// docks the toolbar at a position inside a frame
		XMLToolBarCtrl &Dock(XMLToolBarFrame &f, Point p);
		XMLToolBarCtrl &DockAt(XMLToolBarFrame &f, Point p);
		
		// closes the toolbar
		XMLToolBarCtrl &CloseBar(void);

		// toolbar callback -- to be removed once XML is in place
		void Set(Callback1<Bar &>bar) { toolBar.Set(bar); }
		
		// right mouse click handler -- forwards to main class context menu
		virtual void ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);

		// constructor
		XMLToolBarCtrl(XMLMenuInterface *iFace);
		
	public:
		// destructor
		~XMLToolBarCtrl();
		
};

END_UPP_NAMESPACE

#endif
