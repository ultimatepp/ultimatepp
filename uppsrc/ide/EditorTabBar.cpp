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
	AddFile(WString(fn), IdeFileImage(fn, false, false), true);
}

void EditorTabBar::RenameFile(const String& fn, const String& nn)
{
	FileTabs::RenameFile(WString(fn), WString(nn));
}

void EditorTabBar::FixIcons()
{
	for(int i = 0; i < tabs.GetCount(); i++)
		tabs[i].img = IdeFileImage(GetFile(i), false, false);
	Repos();
	Refresh();
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

void Ide::TabsLR(int jd)
{
	TabBar::JumpStack js;
	int tc = tabs.GetCount();
	
	int n = tabs.GetTabLR(jd);
	if(n >= 0 && n < tc) {
		js = tabs.jump_stack;
		EditFile(tabs.GetFile(n));
		tabs.jump_stack = js;
	}
}

void Ide::TabsStackLR(int jd)
{
	int tc = tabs.GetCount();
  
	int n = tabs.GetTabStackLR(jd);
	if(n >= 0 && n < tc)
		EditFile(tabs.GetFile(n));
}

void Ide::FileSelected()
{
	if(IsNull(editfile))
		return;
	
	tabs.SetAddFile(editfile);
}
