$uvs: PENDING CONFLICT
#ifndef _Docking_DockMenu_h_
#define _Docking_DockMenu_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class DockWindow;
$uvs: REPOSITORY INSERT
class DockableCtrl;
$uvs: END REPOSITORY INSERT

class DockMenu
{
public:
	typedef DockMenu CLASSNAME;

	DockMenu(DockWindow *dockwindow = NULL) : dock(dockwindow)	{  }

	void Set(DockWindow &dockwindow)						{ dock = &dockwindow; }

	void LayoutListMenu(Bar &bar);
	void GroupListMenu(Bar &bar);
	void WindowListMenu(Bar &bar, String group);

	void GroupMenu(Bar &bar, String group);
	void GroupWindowsMenu(Bar &bar, String group);
	void GroupDockMenu(Bar &bar, String group) 				{ GroupAlignMenu(bar, group, 0); }
	void GroupHideMenu(Bar &bar, String group) 				{ GroupAlignMenu(bar, group, 1); }
	void GroupTabDockMenu(Bar &bar, String group)			{ GroupAlignMenu(bar, group, 2); }

$uvs: REPOSITORY INSERT
	void WindowMenuNoClose(Bar &bar, DockableCtrl *dc);
$uvs: END REPOSITORY INSERT
	void WindowMenu(Bar &bar, DockableCtrl *dc);
	void WindowDockMenu(Bar &bar, DockableCtrl *dc)			{ WindowAlignMenu(bar, dc, true); }
	void WindowHideMenu(Bar &bar, DockableCtrl *dc)			{ WindowAlignMenu(bar, dc, false); }
$uvs: REPOSITORY INSERT

protected:
	DockWindow *dock;

	void WindowAlignMenu(Bar &bar, DockableCtrl *dc, bool dodock);
	void GroupAlignMenu(Bar &bar, String group, int mode);

	virtual void MenuDock(int align, DockableCtrl *dc);
	virtual void MenuFloat(DockableCtrl *dc);
	virtual void MenuAutoHide(int align, DockableCtrl *dc);
	virtual void MenuClose(DockableCtrl *dc);

	void MenuLoadLayout(int ix);

	const char *AlignText(int align);
};

#endif

$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
private:
	DockWindow *dock;

	void WindowAlignMenu(Bar &bar, DockableCtrl *dc, bool dock);
	void GroupAlignMenu(Bar &bar, String group, int mode);

	void MenuDock(int align, DockableCtrl *dc);
	void MenuFloat(DockableCtrl *dc);
	void MenuAutoHide(int align, DockableCtrl *dc);
	void MenuClose(DockableCtrl *dc);

	void MenuLoadLayout(int ix);
};
#endif

$uvs: END REPOSITORY DELETE
