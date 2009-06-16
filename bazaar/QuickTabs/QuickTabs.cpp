#include "QuickTabs.h"

QuickTabs::QuickTabs()
{
	Stacking(true);
	GroupSort(true);
	GroupSeparators(true);
	FileTabs::FileIcons(false, false, true);
	
	FileTabs::WhenCloseAll = THISBACK(OnCloseAll);
}

void QuickTabs::ContextMenu(Bar& bar)
{
	bar.Add("Settings", THISBACK(SettingsMenu));
	FileTabs::ContextMenu(bar);
}

void QuickTabs::SettingsMenu(Bar &bar)
{
	bar.Add("Stacking", IsStacking() ? TabBarImg::CHK() : Image(), THISBACK(ToggleStacking));
	bar.Add("Sort by group", IsGroupSort() ? TabBarImg::CHK() : Image(), THISBACK(ToggleGroupSort));
	bar.Add("Group separators", HasGroupSeparators() ? TabBarImg::CHK() : Image(), THISBACK(ToggleGroupSeparators));
	bar.Add("Show inactive", IsShowInactive() ? TabBarImg::CHK() : Image(), THISBACK(ToggleInactive));
}

void QuickTabs::Add(const char *name, bool make_active)
{
	FileTabs::AddFile(name, make_active);
}

bool QuickTabs::FindSetFile(const String& fn)
{
	int n = FindKey(fn.ToWString());
	if (n >= 0) SetCursor(n);
	return n >= 0;
}

void QuickTabs::SetAddFile(const String& fn)
{
	if (IsNull(fn)) return;
	if (!FindSetFile(fn))
		FileTabs::AddFile(fn.ToWString());
}

void QuickTabs::RenameFile(const String& fn, const String& nn)
{
	FileTabs::RenameFile(fn.ToWString(), nn.ToWString());
}

void QuickTabs::Set(const QuickTabs& t)
{
	*this << t;
}

