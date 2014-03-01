#include "XMLToolBar.h"

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////
// XMLToolBarItem

// constructor
XMLToolBarItem::XMLToolBarItem()
{
	commandId	= "";
	label		= "";
	icon		= Null;
	tooltip		= "";
	isSeparator	= false;
	subMenu.Clear();
}
		
// copy constructor
XMLToolBarItem::XMLToolBarItem(const XMLToolBarItem &item, int dummy)
{
	commandId	= item.commandId;
	label		= item.label;
	icon		= item.icon;
	tooltip		= item.tooltip;
	isSeparator	= item.isSeparator;
	subMenu		<<= item.subMenu;
}

// pick constructor
XMLToolBarItem::XMLToolBarItem(XMLToolBarItem pick_ &item)
{
	commandId	= item.commandId;
	label		= item.label;
	icon		= item.icon;
	tooltip		= item.tooltip;
	isSeparator	= item.isSeparator;
	subMenu		= item.subMenu;
}

#ifdef flagDEBUG
// debugging stuff -- dumps bar content
void XMLToolBarItem::Dump(int level)
{
	String spacer;
	spacer.Cat(' ', level);
	if(isSeparator)
		DLOG(spacer << "SEPARATOR");
	else
	{
		DLOG(spacer << "commandId:" << commandId);
		DLOG(spacer << "label    :" << label);
		DLOG(spacer << "tooltip  :" << tooltip);
		DLOG(spacer << "submenu  :" << FormatHex(~subMenu));
		if(subMenu)
			subMenu->Dump(level+2);
	}
}
#endif

// xml support
void XMLToolBarItem::Xmlize(XmlIO xml)
{
	xml
		("commandId"	, commandId)
		("label"		, label)
		("tooltip"		, tooltip)
		("separator"	, isSeparator)
	;
	if(xml.IsLoading())
	{
		XMLToolBar *sub = new XMLToolBar;
		xml("submenu", *sub);
		subMenu = sub;
		if(!sub->GetItems().GetCount())
			subMenu.Clear();
		
		String encoded;
		xml("icon", encoded);
		String compressed = Decode64(encoded);
		String s = GZDecompress(compressed);
		StringStream ss(s);
		ss.SetLoading();
		icon.Serialize(ss);
	}
	else
	{
		XMLToolBar sub;
		if(subMenu)
			sub <<= *subMenu;
		xml("submenu", sub);
		
		StringStream ss;
		ss.SetStoring();
		icon.Serialize(ss);
		String compressed = GZCompress(ss);
		String encoded = Encode64(compressed);
		xml("icon", encoded);
	}
}
		
////////////////////////////////////////////////////////////////////////////////////
// XMLToolBar

// constructor
XMLToolBar::XMLToolBar()
{
	name = "";
	items.Clear();
	state = TOOLBAR_TOP;
	prevState = TOOLBAR_TOP;
	position.Clear();
}

// pick constructor
XMLToolBar::XMLToolBar(XMLToolBar pick_ &tb)
{
	name = tb.name;
	items = tb.items;
	state = tb.state;
	position = tb.position;
}

// copy constructor
XMLToolBar::XMLToolBar(XMLToolBar const &tb, int dummy)
{
	name = tb.name;
	items <<= tb.items;
	state = tb.state;
	prevState = tb.prevState;
	position = tb.position;
}
		
// copy operator
XMLToolBar &XMLToolBar::operator=(XMLToolBar pick_ &tb)
{
	name = tb.name;
	items = tb.items;
	state = tb.state;
	prevState = tb.prevState;
	position = tb.position;
	return *this;
}

// add an entry, various ways
XMLToolBar &XMLToolBar::Add(String const &commandId)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = commandId;
	item->icon = Null;
	item->subMenu.Clear();
	item->tooltip = "";
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, String const &label)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = label;
	item->icon = Null;
	item->subMenu.Clear();
	item->tooltip = "";
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, Image const &icon)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = commandId;
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = "";
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, String const &label, Image const &icon)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = label;
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = "";
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, Image const &icon, String const &tooltip)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = commandId;
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = tooltip;
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, String const &label, String const &tooltip)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = label;
	item->icon = Null;
	item->subMenu.Clear();
	item->tooltip = tooltip;
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, String const &label, Image const &icon, String const &tooltip)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = label;
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = tooltip;
	items.Add(item);
	return *this;
}

// add a submenu entry
XMLToolBar &XMLToolBar::Add(String const &subLabel, XMLToolBar pick_ &subMenu)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->label = subLabel;
	item->subMenu = new XMLToolBar(subMenu);
	item->subMenu->name = subLabel;
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &subLabel, Image const &icon, XMLToolBar pick_ &subMenu)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->label = subLabel;
	item->icon = icon;
	item->subMenu = new XMLToolBar(subMenu);
	item->subMenu->name = subLabel;
	items.Add(item);
	return *this;
}

// creates a submenu entry
XMLToolBar XMLToolBar::SubMenu(void)
{
	return XMLToolBar();
}

// add a submenu entry by callback
XMLToolBar &XMLToolBar::Add(Callback1<XMLToolBar &> bar)
{
	return *this;
}

// add a fixed, unnamed internal command
// used for on-the-fly built menus
XMLToolBar &XMLToolBar::Add(String const &label, Callback cb)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->internalCb = cb;
	item->commandId = "";
	item->label = label;
	item->subMenu.Clear();
	item->tooltip = "";
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(Image const &icon, Callback cb)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->internalCb = cb;
	item->commandId = "";
	item->label = "";
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = "";
	items.Add(item);
	return *this;
}


XMLToolBar &XMLToolBar::Add(String const &label, Image const &icon, Callback cb)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->internalCb = cb;
	item->commandId = "";
	item->label = label;
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = "";
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &label, String const &tooltip, Callback cb)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->internalCb = cb;
	item->commandId = "";
	item->label = label;
	item->subMenu.Clear();
	item->tooltip = tooltip;
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(Image const &icon, String const &tooltip, Callback cb)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->internalCb = cb;
	item->commandId = "";
	item->label = "";
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = tooltip;
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &label, Image const &icon, String const &tooltip, Callback cb)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->internalCb = cb;
	item->commandId = "";
	item->label = label;
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = tooltip;
	items.Add(item);
	return *this;
}

// add a separator
XMLToolBar &XMLToolBar::Separator(void)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->isSeparator = true;
	items.Add(item);
	return *this;
}

#ifdef flagDEBUG
// debugging stuff -- dumps bar content
void XMLToolBar::Dump(int level)
{
	String spacer;
	spacer.Cat(' ', level);
	DLOG(spacer << "Name:" << name << "  ITEMS:" << items.GetCount());
	for(int iItem = 0; iItem < items.GetCount(); iItem++)
	{
		DLOG(spacer << "ITEM #" << iItem);
		items[iItem].Dump(level + 2);
	}
}
#endif

// xml support
void XMLToolBar::Xmlize(XmlIO xml)
{
	xml
		("name"		, name)
		("state"	, (int &)state)
		("prevstate", (int &)prevState)
		("position"	, position)
		("items"	, items)
	;
}

////////////////////////////////////////////////////////////////////////////////////

// adds a new toolbar
XMLToolBars &XMLToolBars::Add(String const &name, XMLToolBar pick_ &tb)
{
	ArrayMap<String, XMLToolBar>::AddPick(name, tb);
	Top().SetName(name);
	return *this;
}

// returns an empty toolbar at a given pos and state
XMLToolBar XMLToolBars::ToolBar(XMLToolBarState state, Point p)
{
	XMLToolBar res;
	res.SetState(state);
	res.SetPosition(p);
	return res;
}

// creates a submenu entry
XMLToolBar XMLToolBars::SubMenu(void)
{
	return XMLToolBar();
}

#ifdef flagDEBUG
// debugging stuff -- dumps bar content
void XMLToolBars::Dump(int level)
{
	String spacer;
	spacer.Cat(' ', level);
	for(int iBar = 0; iBar < GetCount(); iBar++)
	{
		DLOG(spacer << "TOOLBAR #" << iBar);
		operator[](iBar).Dump(level + 2);
	}
}
#endif

END_UPP_NAMESPACE