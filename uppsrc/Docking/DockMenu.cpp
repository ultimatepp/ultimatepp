#include "Docking.h"

namespace Upp {

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

void DockMenu::LayoutListMenu(Bar& bar)
{
	int cnt = dock->LayoutCount();
	for (int i = 0; i < cnt; i++)
		bar.Add(dock->GetLayoutName(i), THISBACK1(MenuLoadLayout, i)); 
}

void DockMenu::GroupListMenu(Bar& bar)
{
	const Vector<DockableCtrl *>& dcs = dock->GetDockableCtrls();
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
	if (dcs.GetCount())
		bar.Add(t_("All"), THISBACK1(GroupWindowsMenu, String(Null)));
}

void DockMenu::WindowListMenu(Bar& bar, String group)
{
	const Vector<DockableCtrl *>& dcs = dock->GetDockableCtrls();
	if (group == t_("All")) group = Null;
	
	for (int i = 0; i < dcs.GetCount(); i++) {
		if (IsNull(group) || group == dcs[i]->GetGroup())
			bar.Add(AsString(dcs[i]->GetTitle()), dcs[i]->GetIcon(), THISBACK1(WindowMenu, dcs[i]));
	}
}

void DockMenu::GroupMenu(Bar& bar, String group)
{
	if (group.IsEmpty()) group = t_("All");
	String text = group;
	text.Insert(0, ' ');

	bar.Add(true, t_("Dock") + text, 	THISBACK1(GroupDockMenu, group));
	bar.Add(true, t_("Float") + text, 	DOCKBACK1(FloatGroup, group));
	if (dock->IsAutoHide()) bar.Add(true, t_("Auto-Hide") + text, THISBACK1(GroupHideMenu, group));
	bar.Separator();
	bar.Add(true, t_("Tabify and Dock") + text, THISBACK1(GroupTabDockMenu, group));
	bar.Add(true, t_("Tabify and Float") + text, DOCKBACK1(TabFloatGroup, group));
	if(dock->HasCloseButtons())
	{
		bar.Separator();
		bar.Add(true, t_("Close") + text, CtrlImg::Remove(), DOCKBACK1(CloseGroup, group));		
	}
}

void DockMenu::GroupWindowsMenu(Bar& bar, String group)
{
	GroupMenu(bar, group);
	bar.Separator();
	WindowListMenu(bar, group);
}

void DockMenu::WindowMenu(Bar& bar, DockableCtrl *dc)
{
	WindowMenuNoClose(bar, dc);
	if(dock->HasCloseButtons())
	{
		bar.Separator();
		bar.Add(true, t_("Close"), CtrlImg::Remove(), THISBACK1(MenuClose, dc));
	}
}

void DockMenu::WindowMenuNoClose(Bar& bar, DockableCtrl *dc)
{
	bar.Add(true, t_("Dock"), 		THISBACK1(WindowDockMenu, dc)).Check(dc->IsDocked() || dc->IsTabbed());
	bar.Add(true, t_("Float"), 		THISBACK1(MenuFloat, dc)).Check(dc->IsFloating());
	if (dock->IsAutoHide()) 
		bar.Add(true, t_("Auto-Hide"), 	THISBACK1(WindowHideMenu, dc)).Check(dc->IsAutoHide());
}

void DockMenu::GroupAlignMenu(Bar& bar, String group, int mode)
{
	for (int i = 0; i < 4; i++)
		bar.Add(AlignText(i), GROUPMACRO(i, group, mode));
}

void DockMenu::WindowAlignMenu(Bar& bar, DockableCtrl *dc, bool dodock)
{
	for (int i = 0; i < 4; i++)
 		bar.Add(!dodock || dock->IsDockAllowed(i, *dc), AlignText(i), 	
 			dodock ? THISBACK2(MenuDock, i, dc) : THISBACK2(MenuAutoHide, i, dc));
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

}
