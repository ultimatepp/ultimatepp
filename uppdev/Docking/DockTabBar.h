$uvs: PENDING CONFLICT
#ifndef _Docking_DockTabBar_h_
#define _Docking_DockTabBar_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <TabBar/TabBar.h>

class DockTabBar : public TabBar
{
public:
	typedef DockTabBar CLASSNAME;

	Callback1<int> 	WhenContext;
	Callback1<int>  WhenDrag;

$uvs: REPOSITORY INSERT
	virtual void 	ContextMenu(Bar& bar)			{ TabBar::ContextMenu(bar); }
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	virtual void 	ContextMenu(Bar& bar)	{ TabBar::ContextMenu(bar); }
$uvs: END REPOSITORY DELETE

	virtual void 	FrameAddSize(Size& sz);
	virtual void	FrameLayout(Rect& r);
	bool 			IsAutoHide() const				{ return GetCount() <= autohide; }

	DockTabBar &	AutoHideMin(int hidemin = 1)	{ autohide = hidemin; return *this; }
	DockTabBar &	Icons(bool b = true)			{ icons = b; }

	void			SyncRepos()						{ Repos(); }

	DockTabBar();
protected:
	int autohide;
	bool icons:1;
//	PasteClip *clip;

	virtual void DrawTabData(Draw& w, Point p, const Size &sz, const Value& q, const Font &font,
		Color ink, dword style);
	virtual Size GetStdSize(Value &q);

	virtual void RightDown(Point p, dword keyflags);
	virtual void LeftDown(Point p, dword keyflags)	{ TabBar::LeftDown(p, keyflags &= ~K_SHIFT); }
	virtual void LeftUp(Point p, dword keyflags)	{ TabBar::LeftUp(p, keyflags &= ~K_SHIFT); }
	virtual void LeftDrag(Point p, dword keyflags);
//	virtual void DragAndDrop(Point p, PasteClip& d);
//	virtual void DragLeave();
};

class DockCont;

class AutoHideBar : public DockTabBar {
public:
	typedef	AutoHideBar CLASSNAME;

	virtual void MouseEnter(Point p, dword keyflags);
	virtual void MouseLeave();

	void 	AddCtrl(DockCont &c, const String &group = Null);
	int 	FindCtrl(DockCont &c) const;
	DockCont *	GetCtrl(int ix)	const				{ return ValueTo<DockCont *>(Get(ix));  }
	void	RemoveCtrl(int ix);
	void	RemoveCtrl(DockCont &c)					{ return RemoveCtrl(c, FindCtrl(c)); }
	void	RemoveCtrl(DockCont &c, int ix);
	bool 	HasCtrl(DockCont &c) const				{ return (FindCtrl(c) >= 0); }

	static void SetTimeout(int delay_ms)	  		{ ASSERT(delay_ms > 0); autohide_timeout = delay_ms; }

	AutoHideBar();

private:
	static int autohide_timeout;

	struct HidePopup : public Ctrl
	{
		Callback WhenEnter;
		Callback WhenLeave;
		virtual void ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);
	};

	enum { TIMEID_ACTION_CHECK = Ctrl::TIMEID_COUNT,
		   TIMEID_HIDE_TIMEOUT,
           TIMEID_COUNT };
	Ctrl *	ctrl;
	HidePopup popup;

$uvs: REPOSITORY INSERT
	void 	TabDrag(int ix);
$uvs: END REPOSITORY INSERT
	void 	TabHighlight();
	void	TabClose(Value v);
	void	ShowAnimate(Ctrl *c);
	void	HideAnimate(Ctrl *c);
	void 	AdjustSize(Rect &r, const Size &sz);
};

#endif

