#ifndef _XMLMenuEditor_h_
#define _XMLMenuEditor_h_

#include <CtrlLib/CtrlLib.h>

#include "XMLMenuInterface.h"

NAMESPACE_UPP

#define LAYOUTFILE <XMLMenu/XMLMenuEditor.lay>
#include <CtrlCore/lay.h>

////////////////////////////////////////////////////////////////////////////////
// bar editor class -- allow editing of a single menu/toolbar
class XMLBarEditor : public ParentCtrl
{
	friend class XMLBarsEditor;
	private:
		// toolbar being edited
		XMLToolBar *bar;
		
		// current icon, if any
		Image curIcon;
		
		// size of item editor
		Size itemSize;
	
		// item editor pane
		WithItemEditorLayout<ParentCtrl> itemPane;
		
		// tree ctrl containing bar structure
		TreeCtrl barTree;
		
		// vertical splitter dividing tree from item editor
		Splitter vertSplitter;
		
		// layouts control
		void Layout(void);
		
		// builds bar tree
		void buildTree(int root, XMLToolBar const *bar);
		
		// drag and drop support
				
		// dragging element
		void dragCb(void);
		
		// dropping between elements (inserts between)
		void dropInsertCb(int parent, int ii, PasteClip& d);
		
		// item selection callback
		void itemSelCb(void);
		
		// fields modified callback
		void fieldsModCb(void);
		
		// image selection callback
		void imageSelCb(void);
	
		// refresh current bar
		void RefreshBar(int treeRoot = 0, XMLToolBar *subBar = NULL);

	protected:
	
	public:
		typedef XMLBarEditor CLASSNAME;
		
		// constructor
		XMLBarEditor();
		
		// gets minimum size of bar editor
		Size GetMinSize(void);
		
		// sets bar being edited
		void SetBar(XMLToolBar *bar);
		
		// sets command id into currently selected item
		void SetCommandId(String const &cmdId) { itemPane.cmdId = cmdId; fieldsModCb(); }
		
		// barTree context menu
		void treeContextCb(Bar &bar);
		void treeContextAddCb(int mode);
		void treeContextRemoveCb(void);
};

////////////////////////////////////////////////////////////////////////////////
class XMLBarAdd : public WithBarAddLayout<TopWindow>
{
	private :
		XMLToolBars &bars;
	
	protected :
		void okCb(void)
		{
			if(~barName == "")
			{
				Exclamation(t_("Invalid empty bar name"));
				return;
			}
			if(bars.Find(barName) >= 0)
			{
				Exclamation(Format(t_("Bar '%s' already present"), ~barName));
				return;
			}
			Break(IDOK);
		}
		void cancelCb(void) { Break(IDCANCEL);}
	
	public :
		typedef XMLBarAdd CLASSNAME;
		
		XMLBarAdd(XMLToolBars &t) : bars(t)
		{
			CtrlLayout(*this);
			okBtn.Ok() << THISBACK(okCb);
			cancelBtn.Cancel() << THISBACK(cancelCb);
		}
};

////////////////////////////////////////////////////////////////////////////////
class XMLBarsEditor : public ParentCtrl
{
	private:
	
		// local copy of toolbars being edited
		XMLToolBars toolBars;
	
		// MenuBars and ToolBars lists
		WithBarListLayout<ParentCtrl> barListPane;
		
		// horizontal splitter
		Splitter horzSplitter;
		
		// the bar editor
		XMLBarEditor barEditor;
		
		// size of selector
		Size selectorSize;

		// bar selection callback
		void barSelCb(void);
		
		// bar list context menu
		void barContextCb(Bar &bar);
		void barContextAddCb(void);
		void barContextRemoveCb(void);
		
	protected:
	
	public:
	
		typedef XMLBarsEditor CLASSNAME;
	
		// constructor
		XMLBarsEditor();

		// gets minimum size of bar editor
		Size GetMinSize(void);

		// adjust layout on win changes
		void Layout(void);

		// set title
		void SetTitle(const char *s);
		
		// sets the local copy of toolbars
		void SetToolBars(XMLToolBars const &tb);
		
		// gets the local copy of toolbars
		XMLToolBars &GetToolBars(void);
		
		// query if a command is in use by a toolbar
		bool IsUsingCommand(String const &cmdId) const;
		
		// sets command id into currently selected item of currently selected toolbar
		void SetCommandId(String const &cmdId) { barEditor.SetCommandId(cmdId); }
};

////////////////////////////////////////////////////////////////////////////////
class XMLCmdAdd : public WithCmdAddLayout<TopWindow>
{
	private :
		XMLCommands &cmds;
	
	protected :
		void okCb(void)
		{
			if(~cmdId == "")
			{
				Exclamation(t_("Invalid empty command Id"));
				return;
			}
			if(cmds.Has(cmdId))
			{
				Exclamation(Format(t_("Command '%s' already present"), ~cmdId));
				return;
			}
			Break(IDOK);
		}
		void cancelCb(void) { Break(IDCANCEL);}
	
	public :
		typedef XMLCmdAdd CLASSNAME;
		
		XMLCmdAdd(XMLCommands &c) : cmds(c)
		{
			CtrlLayout(*this);
			okBtn.Ok() << THISBACK(okCb);
			cancelBtn.Cancel() << THISBACK(cancelCb);
		}
};

////////////////////////////////////////////////////////////////////////////////
class XMLMenuEditor : public WithMenuEditorLayout<TopWindow>
{
	private :
		// interface to access menu system data
		XMLMenuInterface *iFace;
		
		// left pane
		WithCmdBoxLayout<ParentCtrl> cmdPane;
		
		// the menus and bars editors
		XMLBarsEditor menusEditor, barsEditor;
		
		// local copy of commands
		XMLCommands commands;

		// right pane - contains tab ctrl
		TabCtrl tabCtrl;
		
		// vertical size of button area
		int buttonVertSize;
		
		// original sizes of cmd area and item editor area
		Size cmdSize, editorSize, tabCtrlSize;

		// minimum window width and height
		int minWidth, minHeight;
		
		// fills (or updates) command list
		void FillCmdList(void);

		// check wether a command is used in a menu
		bool cmdInUse(String const &cmdId) const;
		
		// commandlist context menu
		void cmdContextCb(Bar &bar);
		void cmdContextAddCb(void);
		void cmdContextRemoveCb(void);
		
		// commandlist double-click handler
		void cmdDblClickCb(void);
	
	protected :
	
		void cancelCb(void);
		void okCb(void);
		
		// adjust layout on win changes
		void Layout(void);
		
	public :
	
		typedef XMLMenuEditor CLASSNAME;
		
		XMLMenuEditor(XMLMenuInterface *iFace);
		
		~XMLMenuEditor();
		
};

END_UPP_NAMESPACE

#endif
