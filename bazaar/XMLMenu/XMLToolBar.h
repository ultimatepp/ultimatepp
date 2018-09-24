#ifndef _XMLMenuItem_h_
#define _XMLMenuItem_h_

#include "XMLCommand.h"

NAMESPACE_UPP

typedef enum
{
	TOOLBAR_CLOSED,
	TOOLBAR_FLOATING,
	TOOLBAR_HORZ_POPUP,
	TOOLBAR_VERT_POPUP,
	TOOLBAR_SQUARE_POPUP,
	TOOLBAR_TOP,
	TOOLBAR_BOTTOM,
	TOOLBAR_LEFT,
	TOOLBAR_RIGHT
	
} XMLToolBarState;

////////////////////////////////////////////////////////////////////////////////////
// a menu/bar item : combine a command with label, icon, tooltip.....
class XMLToolBar;
class XMLToolBarItem : DeepCopyOption<XMLToolBarItem>
{
	friend class XMLToolBar;
	friend class XMLBarEditor;
	private:
		// the command entry
		String commandId;
		
		// the command label, if any
		String label;
		
		// icon, if any
		Image icon;
		
		// tooltip, if any
		String tooltip;
		
		// a submenu, if any
		One<XMLToolBar> subMenu;
		
		// set if separator
		bool isSeparator;
		
		// internal command, if any
		// if set, the item is generated on the fly and not tied to a command
		Function<void()> internalCb;
		
	public:
		String const &GetId(void) const					{ return commandId; }
		String const &GetLabel(void) const				{ return label; }
		Image const &GetIcon(void) const				{ return icon; }
		String const &GetTooltip(void) const			{ return tooltip; }
		XMLToolBar const &GetSubMenu(void) const		{ return *subMenu; }
		bool IsSubMenu(void) const						{ return !subMenu.IsEmpty(); }
		bool IsSeparator(void) const					{ return isSeparator; }
		bool IsInternal(void) const						{ return internalCb; }
		Function<void()> const &GetInternal(void) const	{ return internalCb; }
		
		// xml support
		void Xmlize(XmlIO xml);
		
		// constructor
		XMLToolBarItem();
		
		// copy constructor
		XMLToolBarItem(const XMLToolBarItem &item, int dummy);
		
		// pick constructor
		XMLToolBarItem(XMLToolBarItem rval_ item);

#ifdef flagDEBUG
		// debugging stuff -- dumps bar content
		void Dump(int level = 0);
#endif
};

////////////////////////////////////////////////////////////////////////////////////
// a menu/bar : an array of menu/bar items -- builds up a toolbar or a menu
class XMLToolBar : DeepCopyOption<XMLToolBar>
{
	friend class XMLToolBarItem;
	friend class XMLBarEditor;
	private:
		// bar name
		String name;
		
		// items
		Array<XMLToolBarItem> items;
		
		// position of toolbar
		XMLToolBarState state, prevState;
		Point position;
		
	public:
		// constructor
		XMLToolBar();
		
		// pick constructor
		XMLToolBar(XMLToolBar &&tb);
		
		// copy constructor
		XMLToolBar(XMLToolBar const &tb, int);
		
		// pick operator =
		XMLToolBar &operator=(XMLToolBar &&tb);
		
		// copy operator =
		XMLToolBar const &operator=(XMLToolBar const &tb);
		
		// add an entry, various ways
		XMLToolBar &SetName(String const &_name)		{ name = _name; return *this; }
		XMLToolBar &SetState(XMLToolBarState _state)	{ state = _state; return *this; }
		XMLToolBar &SetPrevState(XMLToolBarState _state){ prevState = _state; return *this; }
		XMLToolBar &SetPosition(Point p)				{ position = p; return *this; }

		XMLToolBar &Add(String const &commandId);
		XMLToolBar &Add(const char *commandId)
			{ return Add(String(commandId)); }
			
		XMLToolBar &Add(String const &commandId, String const &label);
		XMLToolBar &Add(const char *commandId, const char *label)
			{ return Add(String(commandId), String(label)); }
		XMLToolBar &Add(String const &commandId, const char *label)
			{ return Add(commandId, String(label)); }
		XMLToolBar &Add(const char *commandId, String const &label)
			{ return Add(String(commandId), label); }
			
		XMLToolBar &Add(String const &commandId, Image const &icon);
		XMLToolBar &Add(const char *commandId, Image const &icon)
			{ return Add(String(commandId), icon); }
		
		XMLToolBar &Add(String const &commandId, String const &label, Image const &icon);
		XMLToolBar &Add(const char *commandId, String const &label, Image const &icon)
			{ return Add(String(commandId), label, icon); }
		XMLToolBar &Add(String const &commandId, const char *label, Image const &icon)
			{ return Add(commandId, String(label), icon); }
		XMLToolBar &Add(const char *commandId, const char *label, Image const &icon)
			{ return Add(String(commandId), String(label), icon); }
		
		XMLToolBar &Add(String const &commandId, Image const &icon, String const &tooltip);
		XMLToolBar &Add(const char *commandId, Image const &icon, String const &tooltip)
			{ return Add(String(commandId), icon, tooltip); }
		XMLToolBar &Add(String const &commandId, Image const &icon, const char *tooltip)
			{ return Add(commandId, icon, String(tooltip)); }
		XMLToolBar &Add(const char *commandId, Image const &icon, const char *tooltip)
			{ return Add(String(commandId), icon, String(tooltip)); }
		
		XMLToolBar &Add(String const &commandId, String const &label, String const &tooltip);
		XMLToolBar &Add(const char *commandId, String const &label, String const &tooltip)
			{ return Add(String(commandId), label, tooltip); }
		XMLToolBar &Add(String const &commandId, const char *label, String const &tooltip)
			{ return Add(commandId, String(label), tooltip); }
		XMLToolBar &Add(String const &commandId, String const &label, const char *tooltip)
			{ return Add(commandId, label, String(tooltip)); }
		XMLToolBar &Add(const char *commandId, const char *label, String const &tooltip)
			{ return Add(String(commandId), String(label), tooltip); }
		XMLToolBar &Add(const char *commandId, String const &label, const char *tooltip)
			{ return Add(String(commandId), label, String(tooltip)); }
		XMLToolBar &Add(String const &commandId, const char *label, const char *tooltip)
			{ return Add(commandId, String(label), String(tooltip)); }
		XMLToolBar &Add(const char *commandId, const char *label, const char *tooltip)
			{ return Add(String(commandId), String(label), String(tooltip)); }
		
		XMLToolBar &Add(String const &commandId, String const &label, Image const &icon, String const &tooltip);
		XMLToolBar &Add(const char *commandId, String const &label, Image const &icon, String const &tooltip)
			{ return Add(String(commandId), label, icon, tooltip); }
		XMLToolBar &Add(String const &commandId, const char *label, Image const &icon, String const &tooltip)
			{ return Add(commandId, String(label), icon, tooltip); }
		XMLToolBar &Add(String const &commandId, String const &label, Image const &icon, const char *tooltip)
			{ return Add(commandId, label, icon, String(tooltip)); }
		XMLToolBar &Add(const char *commandId, const char *label, Image const &icon, String const &tooltip)
			{ return Add(String(commandId), String(label), icon, tooltip); }
		XMLToolBar &Add(const char *commandId, String const &label, Image const &icon, const char *tooltip)
			{ return Add(String(commandId), label, icon, String(tooltip)); }
		XMLToolBar &Add(String const &commandId, const char *label, Image const &icon, const char *tooltip)
			{ return Add(commandId, String(label), icon, String(tooltip)); }
		XMLToolBar &Add(const char *commandId, const char *label, Image const &icon, const char *tooltip)
			{ return Add(String(commandId), String(label), icon, String(tooltip)); }

		// add a submenu entry
		XMLToolBar &Add(String const &subLabel, XMLToolBar const &subMenu);
		XMLToolBar &Add(const char *subLabel, XMLToolBar const &subMenu)
			{ return Add(String(subLabel), subMenu); }
					
		XMLToolBar &Add(String const &subLabel, Image const &icon, XMLToolBar const &subMenu);
		XMLToolBar &Add(const char *subLabel, Image const &icon, XMLToolBar const &subMenu)
			{ return Add(String(subLabel), icon, subMenu); }
		
		// creates a submenu entry
		XMLToolBar SubMenu(void);
		
		// add a submenu entry by callback
		XMLToolBar &Add(Function<void(XMLToolBar &)> bar);
		
		// add a fixed, unnamed internal command
		// used for on-the-fly built menus
		XMLToolBar &Add(String const &label, Function<void()> cb);
		XMLToolBar &Add(const char *label, Function<void()> cb)
			{ return Add(String(label), cb); }
		
		XMLToolBar &Add(Image const &icon, Function<void()> cb);
		
		XMLToolBar &Add(String const &label, Image const &icon, Function<void()> cb);
		XMLToolBar &Add(const char *label, Image const &icon, Function<void()> cb)
			{ return Add(String(label), icon, cb); }
		
		XMLToolBar &Add(String const &label, String const &tooltip, Function<void()> cb);
		XMLToolBar &Add(const char *label, String const &tooltip, Function<void()> cb)
			{ return Add(String(label), tooltip, cb); }
		XMLToolBar &Add(String const &label, const char *tooltip, Function<void()> cb)
			{ return Add(label, String(tooltip), cb); }
		XMLToolBar &Add(const char *label, const char *tooltip, Function<void()> cb)
			{ return Add(String(label), String(tooltip), cb); }
		
		XMLToolBar &Add(Image const &icon, String const &tooltip, Function<void()> cb);
		XMLToolBar &Add(Image const &icon, const char *tooltip, Function<void()> cb)
			{ return Add(icon, String(tooltip), cb); }
		
		XMLToolBar &Add(String const &label, Image const &icon, String const &tooltip, Function<void()> cb);
		XMLToolBar &Add(const char *label, Image const &icon, String const &tooltip, Function<void()> cb)
			{ return Add(String(label), icon, tooltip, cb); }
		XMLToolBar &Add(String const &label, Image const &icon, const char *tooltip, Function<void()> cb)
			{ return Add(label, icon, String(tooltip), cb); }
		XMLToolBar &Add(const char *label, Image const &icon, const char *tooltip, Function<void()> cb)
			{ return Add(String(label), icon, String(tooltip), cb); }
		
		// add a separator
		XMLToolBar &Separator(void);

		// gets toolbar name
		String const &GetName(void) const				{ return name; }
		
		// get toolbar state and position
		XMLToolBarState GetState(void)					{ return state; }
		XMLToolBarState GetPrevState(void)				{ return prevState; }
		Point GetPosition(void)							{ return position; }
		
		// gets toolbar items
		Array<XMLToolBarItem> const &GetItems(void) const	{ return items; }

		// xml support
		void Xmlize(XmlIO xml);

#ifdef flagDEBUG
		// debugging stuff -- dumps bar content
		void Dump(int level = 0);
#endif
//		rval_default(XMLToolBar);
};

////////////////////////////////////////////////////////////////////////////////////
class XMLToolBars : public ArrayMap<String, XMLToolBar>
{
	private:
	
	protected:
	
	public:
	
		// adds a new toolbar
		XMLToolBars &Add(String const &name, XMLToolBar pick_ &tb);
		
		// returns an empty toolbar at a given pos and state
		XMLToolBar ToolBar(XMLToolBarState state, Point p);
		XMLToolBar ToolBar(XMLToolBarState state, int col, int row) { return ToolBar(state, Point(col, row)); }
		XMLToolBar MenuBar(void) { return ToolBar(TOOLBAR_TOP, Point(0, 0)); }

		// creates a submenu entry
		XMLToolBar SubMenu(void);

#ifdef flagDEBUG
		// debugging stuff -- dumps bar content
		void Dump(int level = 0);
#endif

		// xml support
		void Xmlize(XmlIO xml)
		{
			ArrayMap<String, XMLToolBar> &tb = dynamic_cast<ArrayMap<String, XMLToolBar> &>(*this);
			XmlizeMap<String, XMLToolBar>(xml, "toolbarname", "toolbar", tb);
		}
};

END_UPP_NAMESPACE

#endif
