#ifndef _TabBarCtrl_h_
#define _TabBarCtrl_h_

#include <CtrlLib/CtrlLib.h>
#include <TabBar/TabBar.h>

NAMESPACE_UPP;

class TabBarCtrl : public ParentCtrl
{
	public:
	
		typedef TabBarCtrl CLASSNAME;
		
		TabBarCtrl();
		
		class  Item
		{
			private:
				Ptr<TabBarCtrl> owner;
				Ptr<Ctrl> slave;
				dword key;
				int tabKey;
		
				friend class TabBarCtrl;
		
			public:
				Item& Text(const String& _text);
				Item& SetImage(const UPP::Image& _im);
				Item& Slave(Ctrl *_slave);
				Item& Key(dword _key);
		
				Ctrl *GetSlave() { return slave; }
				const Ctrl *GetSlave() const { return slave; }
				~Item() { if(slave) slave->Remove(); }
		
				Item() { owner = NULL; slave = NULL; key = -1; tabKey = -1; }
/* NOT AVAILABLE TabCtrl::Item members
				Item&          Picture(PaintRect d);
				Item&          SetCtrl(Ctrl *_ctrl);
				Item&          SetCtrl(Ctrl& c)                 { return SetCtrl(&c); }
				Item&          Enable(bool _en = true);
				Item&          Disable()                        { return Enable(false); }
				bool           IsEnabled() const                { return enabled; }
				Ctrl          *GetCtrl()                        { return ctrl; }
				const Ctrl    *GetCtrl() const                  { return ctrl; }
*/
			
		};

	private:
		// the bar
		TabBar tabBar;
		
		// tabctrl child container
		ParentCtrl pane;
		
		// key-value map, connects tabs to items
		// can't be done by indices, as tabs are relocatable
		ArrayMap<int, Item> items;
		
		// next unique key used to pair tabs with slaves
		// get incremented by one on each tab creation
		int nextKey;
		
		// stop actions propagation (TabBar sends unneeded Action events)
		bool actionStop;

		// handle tab selection
		virtual void selectCb();
		
		// handle tab closing
		bool cancelCloseCb(Vector<Value> keys);
		void closeCb(Vector<Value> keys);
		
		// updates embedded controls display
		void updateDisplay(void);
		
		bool accept_current;
		virtual bool  Accept();
		
	public:
	
		Callback				WhenSet;		// Executed when tab selected (either by Set() or mouse)
												// If you want to react only at mouse, use WhenAction()
		Gate1<Vector<int> >		CancelClose;	// Return true to cancel action. Parameter: Key of closed tab
		Callback1<Vector<int> >	WhenClose; 		// Executed before tab closing. Parameter: Key of closed tab

		Item& Add();
		Item& Add(const char *text);
		Item& Add(const Image& m, const char *text);
		Item& Add(Ctrl& slave, const char *text);
		Item& Add(Ctrl& slave, const Image& m, const char *text);
	
		Item& Insert(int i);
		Item& Insert(int i, const char *text);
		Item& Insert(int i, const Image& m, const char *text);
		Item& Insert(int i, Ctrl& slave, const char *text);
		Item& Insert(int i, Ctrl& slave, const Image& m, const char *text);
	
		void  Remove(int i);
	
//		int   GetTab(Point p) const;
	
		int   GetCount() const                       { return tabBar.GetCount(); }
		Item& GetItem(int i)                         { return items[i]; }
		const Item& GetItem(int i) const             { return items[i]; }
	
		void Set(int i);
		void Set(Ctrl& slave);
		int  Get() const;
	
		void GoNext()                                { if(Get() < GetCount() - 1) Set(Get() + 1); }
		void GoPrev()                                { if(Get() > 0) Set(Get() - 1); }
	
		Size     ComputeSize(Size pane);
		void     Add(Ctrl& c)                        { pane.Add(c.SizePos()); }
		TabBarCtrl& operator<<(Ctrl& c)              { Add(c); return *this; }

		TabBarCtrl& AcceptCurrent(bool ac = true)    { accept_current = ac; return *this; }
		TabBarCtrl& AcceptAll()                      { return AcceptCurrent(false); }
		
		TabBarCtrl& NeverEmpty(bool b = true)		 { tabBar.MinTabCount(b ? 1 : 0); }

		virtual Value GetData() const { return Get(); }
		virtual void  SetData(const Value& data) { Set(data); }
};

END_UPP_NAMESPACE;

#endif
