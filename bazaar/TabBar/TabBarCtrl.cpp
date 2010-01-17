#include "TabBarCtrl.h"

NAMESPACE_UPP;

/////////////////////////////////////////////////////////////////////////////////////////
TabBarCtrl::Item& TabBarCtrl::Item::Text(const String& _text)
{
	owner->tabBar.Set(owner->tabBar.FindKey(tabKey), _text);
	return *this;
}

TabBarCtrl::Item& TabBarCtrl::Item::SetImage(const UPP::Image& _im)
{
	owner->tabBar.SetIcon(owner->tabBar.FindKey(tabKey), _im);
	return *this;
}

TabBarCtrl::Item& TabBarCtrl::Item::Slave(Ctrl *_slave)
{
	if(slave)
		slave->Remove();
	slave = _slave;
	owner->pane.AddChild(slave);
	owner->updateDisplay();
	return *this;
}

TabBarCtrl::Item& TabBarCtrl::Item::Key(dword _key)
{
	key = _key;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////
// constructor class TabBarCtrl
TabBarCtrl::TabBarCtrl()
{
	// initializes actions
	WhenSet.Clear();

	// adds the tabBar frame and container pane
	AddFrame(tabBar);
	Ctrl::Add(pane.SizePos());
	
	// initialize items
	items.Clear();
	
	// initializes key generator
	nextKey = 0;
	
	// enables actions
	actionStop = false;
	
	// setup tab change handler
	tabBar.WhenAction = THISBACK(selectCb);
	
	// setup close handlers
	tabBar.WhenCloseSome = THISBACK(closeCb);
	tabBar.CancelCloseSome = THISBACK(cancelCloseCb);
	
	// no minimum tab opened (can close all)
	// beware, if you change this must change closeAllCb handler
	// and that's not an easy way to do...
	tabBar.MinTabCount(0);
	
	Transparent().NoWantFocus();
	accept_current = false;
	
}

// updates embedded controls display
void TabBarCtrl::updateDisplay(void)
{
	// gets the active tab
	int activeTab = Get();
	if(activeTab < 0)
		return;

	// show/hides embedded ctrls
	for(int i = 0; i < items.GetCount(); i++)
	{
		Ctrl *c = items[i].slave;
		if(c)
			c->Show(i == activeTab);
	}
	if(HasFocusDeep())
	{
		if(items[activeTab].slave)
			IterateFocusForward(items[activeTab].slave, GetTopCtrl(), false, true);
		else
			IterateFocusForward(&pane, GetTopCtrl(), false, true);
	}
	
}

TabBarCtrl::Item &TabBarCtrl::Add()
{
	// avoid sending events when not needed
	actionStop = true;
	
	Item &item = items.Add(nextKey);
	item.owner = this;
	item.tabKey = nextKey;
	int cursor = tabBar.GetCursor();
	tabBar.AddKey(nextKey, "");
	if(cursor < 0)
		Set(0);
	else
		tabBar.SetCursor(cursor);

	// restore events
	actionStop = false;

	nextKey++;
	return item;
}
		
TabBarCtrl::Item& TabBarCtrl::Add(const char *text)
{
	return Add().Text(text);
}

TabBarCtrl::Item& TabBarCtrl::Add(const Image& m, const char *text)
{
	return Add(text).SetImage(m);
}

TabBarCtrl::Item& TabBarCtrl::Add(Ctrl& slave, const char *text)
{
	return Add(text).Slave(&slave);
}

TabBarCtrl::Item& TabBarCtrl::Add(Ctrl& slave, const Image& m, const char *text)
{
	return Add(slave, text).SetImage(m);
}

TabBarCtrl::Item& TabBarCtrl::Insert(int i)
{
	// avoid sending events when not needed
	actionStop = true;
	
	Item &item = items.Add(nextKey);
	item.owner = this;
	item.tabKey = nextKey;
	int cursor = tabBar.GetCursor();
	tabBar.InsertKey(i, nextKey, "");
	if(cursor < 0)
		Set(0);
	else
		tabBar.SetCursor(cursor);
	
	// restore events
	actionStop = false;
	
	nextKey++;
	return item;
}

TabBarCtrl::Item& TabBarCtrl::Insert(int i, const char *text)
{
	return Insert(i).Text(text);
}

TabBarCtrl::Item& TabBarCtrl::Insert(int i, const Image& m, const char *text)
{
	return Insert(i, text).SetImage(m);
}

TabBarCtrl::Item& TabBarCtrl::Insert(int i, Ctrl& slave, const char *text)
{
	return Insert(i, text).Slave(&slave);
}

TabBarCtrl::Item& TabBarCtrl::Insert(int i, Ctrl& slave, const Image& m, const char *text)
{
	return Insert(i, slave, text).SetImage(m);
}

void  TabBarCtrl::Remove(int i)
{
	ASSERT(i >= 0 && i < items.GetCount());
	int tabKey = items.GetKey(i);
	if(tabKey < 0)
		return;
	int iTab = tabBar.FindKey(tabKey);
	if(iTab < 0)
		return;
	tabBar.Close(iTab);
	items.Remove(i);
}

int  TabBarCtrl::Get() const
{
	if(!tabBar.GetCount())
		return -1;
	int iCursor = tabBar.GetCursor();
	if(iCursor < 0)
		return iCursor;
	int key = tabBar.GetKey(iCursor);
	return items.Find(key);
	
}
void TabBarCtrl::Set(int i)
{
	int key = items[i].tabKey;
	int iCursor = tabBar.FindKey(key);
	if(tabBar.GetCursor() == iCursor)
		return;
	actionStop = true;
	tabBar.SetCursor(iCursor);
	actionStop = false;
	if(WhenSet)
		WhenSet();
}

void TabBarCtrl::Set(Ctrl& slave)
{
	for(int i = 0; i < items.GetCount(); i++)
		if(items[i].slave == &slave) {
			Set(i);
			return;
		}
}

void TabBarCtrl::selectCb(void)
{
	// gets selected tab
	int iTab = tabBar.GetCursor();
	
	// if null, just do nothing
	if(iTab < 0)
		return;
	
	// setup requested tab control
	updateDisplay();
	
	// if actions enabled, propagate them
	if(!actionStop && WhenAction)
		WhenAction();
	if(!actionStop && WhenSet)
		WhenSet();
}

// handle tab closing
bool TabBarCtrl::cancelCloseCb(Vector<Value> keys)
{
	// check if we want to cancel close
	if(CancelClose)
	{
		Vector<int>iv;
		for(int i = 0; i < keys.GetCount(); i++)
			iv.Add(items.Find(keys[i]));
		return CancelClose(iv);
	}
	return false;
}

void TabBarCtrl::closeCb(Vector<Value> keys)
{
	// notify of tab closing
	if(WhenClose)
	{
		Vector<int>iv;
		for(int i = 0; i < keys.GetCount(); i++)
			iv.Add(items.Find(keys[i]));
		WhenClose(iv);
	}

	// delete item corresponding to closed tab
	for(int i = 0; i < keys.GetCount(); i++)
	{
		int j = items.Find(keys[i]);
		if(j >= 0)
			items.Remove(j);
	}
	
	// updates display
	updateDisplay();
}

bool TabBarCtrl::Accept()
{
	if(items.GetCount() == 0)
		return true;
	int ii = Get();
	if(accept_current)
		return !items[ii].slave || items[ii].slave -> Accept();
	for(int i = 0; i < items.GetCount(); i++)
		if(items[i].slave) {
			Set(i);
			if(!items[i].slave->Accept())
				return false;
		}
	Set(ii);
	return true;
}

END_UPP_NAMESPACE;
