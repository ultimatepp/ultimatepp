$uvs: PENDING CONFLICT
#include "Docking.h"

#define DOCKBACK(x)               callback(dock, &DockWindow::x)
#define DOCKBACK1(x, arg)         callback1(dock, &DockWindow::x, arg)
#define DOCKBACK2(m, a, b)        callback2(dock, &DockWindow::m, a, b)
#define DOCKBACK3(m, a, b, c)     callback3(dock, &DockWindow::m, a, b, c)
#define DOCKBACK4(m, a, b, c, d)  callback4(dock, &DockWindow::m, a, b, c, d)

#define GROUPMACRO(a, g, m) (m == 0) ? DOCKBACK3(DockGroup, a, g, -1) : ((m == 1) ? DOCKBACK2(AutoHideGroup, a, g) : DOCKBACK3(TabDockGroup, a, g, -1))

#define DOCK_LEFT 	(int)DockWindow::DOCK_LEFT
#define DOCK_TOP 	(int)DockWindow::DOCK_TOP
#define DOCK_RIGHT 	(int)DockWindow::DOCK_RIGHT
#define DOCK_BOTTOM (int)DockWindow::DOCK_BOTTOM

void DockMenu::LayoutListMenu(Bar &bar)
{
	int cnt = dock->LayoutCount();
	for (int i = 0; i < cnt; i++)
		bar.Add(dock->GetLayoutName(i), THISBACK1(MenuLoadLayout, i));
}

void DockMenu::GroupListMenu(Bar &bar)
{
	const Vector<DockableCtrl *> &dcs = dock->GetDockableCtrls();
	Array<String> groups;
	for (int i = 0; i < dcs.GetCount(); i++) {
		String s = dcs[i]->GetGroup();
		if (!s.IsEmpty()) {
			bool found = false;
			for (int j = 0; j < groups.GetCount(); j++)
				if (groups[j] == s) {
					found = true;
					break;
				}
			if (!found)
				groups.Add(s);
		}
	}
	Sort(groups);
	for (int i = 0; i < groups.GetCount(); i++)
		bar.Add(groups[i], THISBACK1(GroupWindowsMenu, groups[i]));
$uvs: REPOSITORY INSERT
	if (dcs.GetCount())
		bar.Add(t_("All"), THISBACK1(GroupWindowsMenu, String(Null)));
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	bar.Add("All", THISBACK1(GroupWindowsMenu, String(Null)));
$uvs: END REPOSITORY DELETE
}

void DockMenu::WindowListMenu(Bar &bar, String group)
{
	const Vector<DockableCtrl *> &dcs = dock->GetDockableCtrls();
$uvs: REPOSITORY INSERT
	if (group == t_("All")) group = Null;
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	if (group == "All") group = Null;
$uvs: END REPOSITORY DELETE

	for (int i = 0; i < dcs.GetCount(); i++) {
		if (IsNull(group) || group == dcs[i]->GetGroup())
			bar.Add(AsString(dcs[i]->GetTitle()), dcs[i]->GetIcon(), THISBACK1(WindowMenu, dcs[i]));
	}
}

void DockMenu::GroupMenu(Bar &bar, String group)
{
$uvs: REPOSITORY INSERT
	if (group.IsEmpty()) group = t_("All");
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	if (group.IsEmpty()) group = "All";
$uvs: END REPOSITORY DELETE
	String text = group;
	text.Insert(0, ' ');

$uvs: REPOSITORY INSERT
	bar.Add(true, t_("Dock") + text, 	THISBACK1(GroupDockMenu, group));
	bar.Add(true, t_("Float") + text, 	DOCKBACK1(FloatGroup, group));
	if (dock->IsAutoHide()) bar.Add(true, t_("Auto-Hide") + text, THISBACK1(GroupHideMenu, group));
	bar.Separator();
	bar.Add(true, t_("Tabify and Dock") + text, THISBACK1(GroupTabDockMenu, group));
	bar.Add(true, t_("Tabify and Float") + text, DOCKBACK1(TabFloatGroup, group));
	bar.Separator();
	bar.Add(true, t_("Close") + text, CtrlImg::Remove(), DOCKBACK1(CloseGroup, group));
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	bar.Add(true, "Dock" + text, 	THISBACK1(GroupDockMenu, group));
	bar.Add(true, "Float" + text, 	DOCKBACK1(FloatGroup, group));
	if (dock->IsAutoHide()) bar.Add(true, "Auto-Hide" + text, THISBACK1(GroupHideMenu, group));
	bar.Separator();
	bar.Add(true, "Tabify and Dock" + text, THISBACK1(GroupTabDockMenu, group));
	bar.Add(true, "Tabify and Float" + text, DOCKBACK1(TabFloatGroup, group));
	bar.Separator();
	bar.Add(true, "Close" + text, CtrlImg::Remove(), DOCKBACK1(CloseGroup, group));
$uvs: END REPOSITORY DELETE
}

void DockMenu::GroupWindowsMenu(Bar &bar, String group)
{
	GroupMenu(bar, group);
	bar.Separator();
	WindowListMenu(bar, group);
}

void DockMenu::WindowMenu(Bar &bar, DockableCtrl *dc)
{
$uvs: REPOSITORY INSERT
	WindowMenuNoClose(bar, dc);
	bar.Separator();
	bar.Add(true, t_("Close"), CtrlImg::Remove(), THISBACK1(MenuClose, dc));
}

void DockMenu::WindowMenuNoClose(Bar &bar, DockableCtrl *dc)
{
	bar.Add(true, t_("Dock"), 		THISBACK1(WindowDockMenu, dc)).Check(dc->IsDocked() || dc->IsTabbed());
	bar.Add(true, t_("Float"), 		THISBACK1(MenuFloat, dc)).Check(dc->IsFloating());
	if (dock->IsAutoHide())
		bar.Add(true, t_("Auto-Hide"), 	THISBACK1(WindowHideMenu, dc)).Check(dc->IsAutoHide());
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	bar.Add(true, "Dock", 		THISBACK1(WindowDockMenu, dc));
	bar.Add(true, "Float", 		THISBACK1(MenuFloat, dc));
	if (dock->IsAutoHide()) bar.Add(true, "Auto-Hide", 	THISBACK1(WindowHideMenu, dc));
	bar.Separator();
	bar.Add(true, "Close", CtrlImg::Remove(), THISBACK1(MenuClose, dc));
$uvs: END REPOSITORY DELETE
}

void DockMenu::GroupAlignMenu(Bar &bar, String group, int mode)
{
$uvs: REPOSITORY INSERT
	for (int i = 0; i < 4; i++)
		bar.Add(AlignText(i), 	GROUPMACRO(i, group, mode));
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	bar.Add("Left", 	GROUPMACRO(DOCK_LEFT, group, mode));
	bar.Add("Top", 		GROUPMACRO(DOCK_TOP, group, mode));
	bar.Add("Right", 	GROUPMACRO(DOCK_RIGHT, group, mode));
	bar.Add("Bottom", 	GROUPMACRO(DOCK_BOTTOM, group, mode));
$uvs: END REPOSITORY DELETE
}

void DockMenu::WindowAlignMenu(Bar &bar, DockableCtrl *dc, bool dodock)
{
$uvs: REPOSITORY INSERT
	for (int i = 0; i < 4; i++)
 		bar.Add(!dodock || dock->IsDockAllowed(i, *dc), AlignText(i),
 			dodock ? THISBACK2(MenuDock, i, dc) : THISBACK2(MenuAutoHide, i, dc));
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
 	bar.Add(dock->IsDockAllowed(DOCK_LEFT, *dc), 	"Left", 	dodock ? THISBACK2(MenuDock, DOCK_LEFT, dc) 	: THISBACK2(MenuAutoHide, DOCK_LEFT, dc));
	bar.Add(dock->IsDockAllowed(DOCK_TOP, *dc), 	"Top", 		dodock ? THISBACK2(MenuDock, DOCK_TOP, dc) 		: THISBACK2(MenuAutoHide, DOCK_TOP, dc));
	bar.Add(dock->IsDockAllowed(DOCK_RIGHT, *dc), 	"Right",	dodock ? THISBACK2(MenuDock, DOCK_RIGHT, dc) 	: THISBACK2(MenuAutoHide, DOCK_RIGHT, dc));
	bar.Add(dock->IsDockAllowed(DOCK_BOTTOM, *dc), 	"Bottom", 	dodock ? THISBACK2(MenuDock, DOCK_BOTTOM, dc) 	: THISBACK2(MenuAutoHide, DOCK_BOTTOM, dc));
$uvs: END REPOSITORY DELETE
}

void DockMenu::MenuDock(int align, DockableCtrl *dc)
{
	dock->Dock(align, *dc);
}

void DockMenu::MenuFloat(DockableCtrl *dc)
{
	dock->Float(*dc);
}

void DockMenu::MenuAutoHide(int align, DockableCtrl *dc)
{
	dock->AutoHide(align, *dc);
}

void DockMenu::MenuClose(DockableCtrl *dc)
{
	dock->Close(*dc);
}

void DockMenu::MenuLoadLayout(int ix)
{
	dock->LoadLayout(ix);
}

$uvs: REPOSITORY INSERT
const char * DockMenu::AlignText(int align)
{
	switch (align) {
	case DOCK_LEFT:
		return t_("Left");
	case DOCK_TOP:
		return t_("Top");
	case DOCK_RIGHT:
		return t_("Right");
	case DOCK_BOTTOM:
		return t_("Bottom");
	}
	return 0;
}

$uvs: END REPOSITORY INSERT
