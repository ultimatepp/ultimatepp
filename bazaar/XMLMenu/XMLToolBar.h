#ifndef _XMLToolBar_h_
#define _XMLToolBar_h_

#include "XMLDragFrame.h"
#include "XMLToolBarFloating.h"

NAMESPACE_UPP

// XMLToolBar class : a container for ToolBar and its drag handle
class XMLToolBarFrame;
class XMLMenuInterface;
class XMLToolBar : public ParentCtrl
{
	template<class T> friend class WithXMLMenu;
	public:
		typedef enum { TOOLBAR_CLOSED, TOOLBAR_FLOATING, TOOLBAR_HORZ_POPUP, TOOLBAR_VERT_POPUP, TOOLBAR_SQUARE_POPUP, TOOLBAR_TOP, TOOLBAR_BOTTOM, TOOLBAR_LEFT, TOOLBAR_RIGHT } XMLToolBarState;
	
	friend class XMLToolBarFrame;
	private:

		// just to access BarPane::GetSize()...
		struct SizeToolBar : ToolBar
		{
			Size GetPaneSize(bool horz, int max = INT_MAX) { return pane.GetPaneSize(horz, max); }
		};

		// link to floating window, if undocked
		// @@@@ to do
	
		// the embedded toolbar
		ToolBar toolBar;
		
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
		int dockedRow, dockedCol;
		int undockedX, undockedY;
		
	protected:
	
		// lays toolbar and handle inside control
		virtual void Layout(void);
		
		// gets toolbar align
		int GetAlign(void);
		
		// mouse handler -- handles moves when ctrl is in popup state
		virtual Image MouseEvent(int event, Point p, int zdelta, dword keyflags);
		
	public:
	
		typedef XMLToolBar CLASSNAME;

		// constructor
		XMLToolBar(XMLMenuInterface *iFace);
		
		// destructor
		~XMLToolBar();
		
		XMLMenuInterface *GetInterface(void) { return iFace; }
		
		// gets toolbar size when completely inlined horizontally
		virtual Size GetHorzSize(void);
		
		// gets toolbar size when completely inlined vertically
		virtual Size GetVertSize(void);
		
		// gets toolbar size when squared
		Size GetSquaredSize(void);
		
		// get docked state
		bool GetIsDocked();
		
		// get floating state
		bool GetIsFloating();
		
		// get opened state
		bool GetIsOpened();
		
		// get popup state
		bool GetIsPopUp();
		
		// sets toolbar state
		XMLToolBar &SetState(XMLToolBarState state);
		XMLToolBarState GetState(void) { return toolBarState; }
		
		// floats the toolbar at a given position
		XMLToolBarFloating &Float(Point p);
		
		// popups the toolbar at a given position
		XMLToolBar &Popup(Point p);
		XMLToolBar &PopHorz(Point p);
		XMLToolBar &PopVert(Point p);
		XMLToolBar &PopSquare(Point p);
		
		// pre-Dock and pre-Undock a toolbar inside frame
		XMLToolBar &PreDock(XMLToolBarFrame &f, Point p);
		XMLToolBar &UnPreDock(XMLToolBarFrame &f);
		
		// docks the toolbar at a position inside a frame
		XMLToolBar &Dock(XMLToolBarFrame &f, int row, int col);
		XMLToolBar &Dock(XMLToolBarFrame &f, Point p);
		
		// closes the toolbar
		XMLToolBar &CloseBar(void);

		// toolbar callback -- to be removed once XML is in place
		void Set(Callback1<Bar &>bar) { toolBar.Set(bar); }
};

END_UPP_NAMESPACE

#endif
