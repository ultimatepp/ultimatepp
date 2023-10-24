#include "CtrlLib.h"

#ifndef CtrlCore_MenuImp_h
#define CtrlCore_MenuImp_h

namespace Upp {

class MenuItemBase : public Ctrl, public Bar::Item {
public:
	virtual Bar::Item& Text(const char *text);
	virtual Bar::Item& Key(dword key);
	virtual Bar::Item& Image(const UPP::Image& img);
	virtual Bar::Item& Enable(bool _enable);
	virtual Bar::Item& Tip(const char *tip);
	virtual Bar::Item& Help(const char *help);
	virtual Bar::Item& Topic(const char *help);
	virtual Bar::Item& Description(const char *desc);
	virtual Bar::Item& Check(bool check);
	virtual Bar::Item& Radio(bool check);
	virtual Bar::Item& Bold(bool bold);

	virtual String GetDesc() const;
	virtual dword  GetAccessKeys() const;
	virtual void   AssignAccessKeys(dword used);

	using   Ctrl::Key;

protected:
	enum {
		NOTHING, CHECK0, CHECK1, RADIO0, RADIO1
	};

	enum {
		NORMAL, HIGHLIGHT, PUSH
	};

	String  text;
	dword   accel;
	int     state;
	int     leftgap, textgap;
	Font    font;
	bool    isenabled;
	byte    type;
	int     accesskey;
	Size    maxiconsize;
	const MenuBar::Style *style;
	bool    nodarkadjust;

	MenuBar *GetMenuBar() const;

public:
	virtual void SyncState() = 0;

	void           DrawMenuText(Draw& w, int x, int y, const String& s, Font f, bool enabled, bool hl,
	                            Color color, Color hlcolor);
	void           PaintTopItem(Draw& w, int state);

	bool           IsItemEnabled() const          { return isenabled; }
	String         GetText() const                { return text; }
	MenuItemBase&  LeftGap(int cx)                { leftgap = cx; return *this; }
	MenuItemBase&  TextGap(int cx)                { textgap = cx; return *this; }
	MenuItemBase&  SetFont(Font f)                { font = f; return *this; }
	MenuItemBase&  Style(const MenuBar::Style *s) { style = s; return *this; }
	Font           GetFont() const                { return font; }
	MenuItemBase&  MaxIconSize(Size sz)           { maxiconsize = sz; return *this; }
	bool           InOpaqueBar() const;
	MenuItemBase&  NoDarkAdjust(bool b = true)    { nodarkadjust = b; return *this; }

	MenuItemBase();
};

class MenuItem : public MenuItemBase {
public:
	virtual void  Paint(Draw& w);
	virtual void  MouseEnter(Point, dword);
	virtual void  MouseLeave();
	virtual Size  GetMinSize() const;
	virtual void  LeftUp(Point, dword);
	virtual void  RightUp(Point, dword);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual bool  Key(dword key, int count);
	virtual bool  HotKey(dword key);
	virtual void  SyncState();

	virtual Bar::Item& Image(const UPP::Image& img);

private:
	UPP::Image licon, ricon;

	void  SendHelpLine();
	void  ClearHelpLine();
	using MenuItemBase::Key;

protected:
	virtual int  GetVisualState();

public:
	MenuItem& RightImage(const UPP::Image& img);
};

class SubMenuBase {
protected:
	MenuBar  menu;
	Event<Bar&> proc;
	MenuBar *parentmenu;

	void     Pull(Ctrl *item, Point p, Size sz);

public:
	virtual  void Pull() = 0;

	void SetParent(MenuBar *m)                           { parentmenu = m; menu.MaxIconSize(m->GetMaxIconSize()); }
	void Set(Event<Bar&> _submenu)                   { proc = _submenu; }
	Event<Bar&> Get()                                { return proc; }

	SubMenuBase()                                        { parentmenu = NULL; }
	virtual ~SubMenuBase() {}
};

class SubMenuItem : public MenuItem, public SubMenuBase {
public:
	virtual void MouseEnter(Point, dword);
	virtual void MouseLeave();
	virtual void GotFocus();
	virtual bool HotKey(dword key);
	virtual bool Key(dword key, int count);
	virtual int  GetVisualState();
	virtual void Pull();

protected:
	enum {
		TIMEID_PULL = BarCtrl::TIMEID_COUNT,
		TIMEID_COUNT
	};

public:
	typedef SubMenuItem CLASSNAME;

	SubMenuItem();
};

class TopSubMenuItem : public MenuItemBase, public SubMenuBase {
public:
	virtual void Paint(Draw& w);
	virtual void MouseEnter(Point, dword);
	virtual void MouseLeave();
	virtual void GotFocus();
	virtual void LostFocus();
	virtual void LeftDown(Point, dword);
	virtual void SyncState();
	virtual Size GetMinSize() const;
	virtual bool Key(dword key, int);
	virtual bool HotKey(dword key);
	virtual void Pull();

private:
	int  GetState();
	using MenuItemBase::Key;

public:
	TopSubMenuItem() { NoInitFocus(); }
};

class TopMenuItem : public MenuItemBase {
public:
	virtual void  Paint(Draw& w);
	virtual void  MouseEnter(Point, dword);
	virtual void  MouseLeave();
	virtual void  LeftUp(Point, dword);
	virtual void  LeftDown(Point, dword);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual bool  Key(dword key, int count);
	virtual Size  GetMinSize() const;
	virtual void  SyncState();

	static int GetStdHeight(Font font = StdFont());

private:
	int  GetState();
	using MenuItemBase::Key;

public:
	TopMenuItem() { NoInitFocus(); }
};

}

#endif
