#include "ide.h"

EditorTabBar::EditorTabBar()
{
	Stacking();
	AutoScrollHide(false);
	FileIcons(false);
	SetBorder(1);
}

String EditorTabBar::GetFile(int n) const
{
	return tabs[n].key;
}

bool EditorTabBar::FindSetFile(const String& fn)
{
	int n = FindKey(fn);
	if(n >= 0)
	{
		SetCursor(n);
		return true;
	}
	return false;
}

void EditorTabBar::SetAddFile(const String& fn)
{
	if(IsNull(fn))
		return;
	if(FindSetFile(fn))
		return;
	AddFile(WString(fn), true);
}

void EditorTabBar::RenameFile(const String& fn, const String& nn)
{
	FileTabs::RenameFile(WString(fn), WString(nn));
}

void EditorTabBar::SetSplitColor(const String& fn, const Color& c)
{
	int n = -1;
	
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		tabs[i].col = Null;
		
		if(n < 0 && tabs[i].key == fn)
			n = i;
	}
	
	if(n >= 0)
	{
		if(stacking)
			n = FindStackHead(tabs[n].stack);
		tabs[n].col = c;
	}
	
	Refresh();
}

void EditorTabBar::ClearSplitColor()
{
	for(int i = 0; i < tabs.GetCount(); i++)
		tabs[i].col = Null;
	Refresh();
}

void Ide::TabFile()
{
	int q = tabs.GetCursor();
	if(q >= 0)
		EditFile(tabs.GetFile(q));
}

void Ide::ClearTab()
{
	int c = tabs.GetCursor();
	if(c >= 0)
		tabs.Close(c);
}

void Ide::ClearTabs()
{
	tabs.Clear();
	FileSelected();
}

void Ide::CloseRest(EditorTabBar *tabs)
{
	Index<String> fn;
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		for(int j = 0; j < wspc.GetPackage(i).file.GetCount(); j++)
			fn.Add(SourcePath(wspc[i], wspc.GetPackage(i).file[j]));
	String cfn;
	if(tabs->GetCursor() >= 0)
		cfn = tabs->GetFile(tabs->GetCursor());
	for(int i = tabs->GetCount() - 1; i >= 0; i--)
		if(fn.Find(tabs->GetFile(i)) < 0)
			tabs->Close(i);
	tabs->FindSetFile(cfn);
}

void Ide::TabsLR(int d)
{
	int c = tabs.GetCursor();
	if(c < 0 || tabs.GetCount() <= 1)
		return;
	c = minmax(c + d, 0, tabs.GetCount() - 1);
	EditFile(tabs.GetFile(c));
}

void Ide::FileSelected()
{
	if(!IsNull(editfile))
		tabs.SetAddFile(editfile);
}
