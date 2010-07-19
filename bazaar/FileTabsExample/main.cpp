#include "FileTabsExample.h"

#define FILE_LIMIT 5
#define PACKAGE_LIMIT 5

FileTabsExample::FileTabsExample()
{
	CtrlLayout(*this, "FileTabs Example");
	Sizeable().Zoomable();

	for (int i = 0; i < 4; i++)
		bar[i].SetAlign((i+1) % 4);
	AddFrame(bar[0]);

	bar[0] <<= THISBACK(OnCursor);
	
	addfile 	<<= THISBACK(OnAdd);
	insert 		<<= THISBACK(OnInsert);
	
	grouping 	<<= THISBACK(OnGrouping);
	groupsort 	<<= THISBACK(OnGroupSort);
	separators 	<<= THISBACK(OnSeparators);
	stacking 	<<= THISBACK(OnStacking);
	normalicons <<= THISBACK(OnIcons);
	stackedicons <<= THISBACK(OnIcons);
	prompt 		<<= THISBACK(OnPrompt);
	allalign 	<<= THISBACK(OnAllAlign);
	valuesort 	<<= THISBACK(OnValueSort);
	keysort 	<<= THISBACK(OnKeySort);
	
	grouping <<= true;
	inactive <<= true;
	normalicons <<= true;
	
	OnInsert();	
	OnCursor();
}

void FileTabsExample::OnCursor()
{
	if (bar[0].HasCursor())
		selection = (String)~bar[0];
	else
		selection = "No tab selected";
}

void FileTabsExample::OnInsert()
{
	Vector<String> files;
	files.Add("C:\\uppsvn\\bazaar\\TabBar\\FileTabs.h");
	files.Add("C:\\uppsvn\\bazaar\\TabBar\\TabBar.cpp");
	files.Add("C:\\uppsvn\\bazaar\\TabBar\\TabBar.h");
	files.Add("C:\\uppsvn\\bazaar\\TabBar\\TabBar.iml");
	files.Add("C:\\uppsvn\\bazaar\\TabBar\\TabBar.upp");
	files.Add("C:\\uppsvn\\bazaar\\TabBar\\FileTabs.cpp");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\Docking.lay");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\Docking.t");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockTabBar.cpp");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockTabBar.h");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockMenu.cpp");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockMenu.h");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockPane.cpp");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockWindow.cpp");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\Docking.h");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\Docking.upp");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockPane.h");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockableCtrl.cpp");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockableCtrl.h");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockConfig.cpp");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockCont.cpp");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\DockCont.h");
	files.Add("C:\\uppsvn\\bazaar\\UppDocking\\Docking.iml");
	files.Add("C:\\uppsvn\\bazaar\\FileTabsExample\\FileTabsExample.h");
	files.Add("C:\\uppsvn\\bazaar\\FileTabsExample\\FileTabsExample.lay");
	files.Add("C:\\uppsvn\\bazaar\\FileTabsExample\\FileTabsExample.upp");
	files.Add("C:\\uppsvn\\bazaar\\FileTabsExample\\main.cpp");
	for (int i = 0; i < 4; i++)
		bar[i].AddFiles(files, false);	
}

bool FileTabsExample::OnCloseConfirm(Value v)
{
	return PromptOKCancel(Format("Close '%s'?", DeQtf((String)v))) == 0;
}

bool FileTabsExample::OnCloseAllConfirm()
{
	return PromptOKCancel("Close all tabs?") == 0;
}

void FileTabsExample::OnAdd()
{
	if (!fileinput.GetLength()) return;
	for (int i = 0; i < 4; i++)
		bar[i].AddFile(~fileinput);
}

void FileTabsExample::OnGrouping()
{
	for (int i = 0; i < 4; i++)
		bar[i].Grouping(~grouping);
}

void FileTabsExample::OnGroupSort()
{
	if(~groupsort)
	for (int i = 0; i < 4; i++)
		bar[i].SortGroups(true); //or specify own TabSort
	else	
	for (int i = 0; i < 4; i++)
		bar[i].SortGroups(false);
}

void FileTabsExample::OnSeparators()
{
	for (int i = 0; i < 4; i++)
		bar[i].GroupSeparators(~separators);	
}

void FileTabsExample::OnStacking()
{
	for (int i = 0; i < 4; i++)
		bar[i].Stacking(~stacking);	
}

void FileTabsExample::OnStackSort()
{
	if(~stacksort)
	for (int i = 0; i < 4; i++)
		bar[i].SortStacks(true); //or specify own TabSort
	else	
	for (int i = 0; i < 4; i++)
		bar[i].SortStacks(false);
}

void FileTabsExample::OnIcons()
{
	for (int i = 0; i < 4; i++)
		bar[i].FileIcons(~normalicons, ~stackedicons);
}

void FileTabsExample::OnPrompt()
{
	if (prompt) {
		bar[0].CancelClose 		= THISBACK(OnCloseConfirm);	
		bar[0].CancelCloseAll 	= THISBACK(OnCloseAllConfirm);
	}
	else {
		bar[0].CancelClose.Clear();
		bar[0].CancelCloseAll.Clear();
	}
}

void FileTabsExample::OnAllAlign()
{
	if (allalign) {
		for (int i = 1; i < 4; i++)
			AddFrame(bar[i]);
	}
	else {
		for (int i = 0; i < 4; i++)
			bar[i].Remove();			
		ClearFrames();
		AddFrame(bar[0]);
	}
}

void FileTabsExample::OnInactive()
{
	for (int i = 1; i < 4; i++)
		bar[i].InactiveDisabled(~inactive);				
}

void FileTabsExample::OnValueSort()
{
	keysort = false;
	if(~valuesort)
	for (int i = 0; i < 4; i++)
		bar[i].SortTabValues(vo);
	else
	for (int i = 0; i < 4; i++)
		bar[i].SortTabs(false);
}

void FileTabsExample::OnKeySort()
{
	valuesort = false;
	if(~keysort)
	for (int i = 0; i < 4; i++)
		bar[i].SortTabKeys(vo);
	else
	for (int i = 0; i < 4; i++)
		bar[i].SortTabs(false);
}

bool FileTabsExample::Key(dword key, int count)
{
	if (key == K_RETURN && fileinput.HasFocus()) {
		addfile.PseudoPush();
		return true;
	}
	return false;
}

void FileTabsExample::DragAndDrop(Point p, PasteClip& d)
{
	if (AcceptFiles(d))
	    for (int i = 0; i < 4; i++)
	    	bar[i].AddFiles(GetFiles(d));
}



GUI_APP_MAIN
{
	FileTabsExample().Run();
}

