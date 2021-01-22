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

	bar[0].WhenAction << [=] { OnCursor(); };
	
	addfile << [=] {
		if (!fileinput.GetLength()) return;
		for (int i = 0; i < 4; i++)
			bar[i].AddFile(~fileinput);
	};

	insert << [=] {
		for(FindFile ff(GetHomeDirFile("*")); ff; ff++)
			for(int i = 0; i < 4; i++)
				bar[i].AddFile(ff.GetPath().ToWString());
	};

	grouping << [=] {
		for (int i = 0; i < 4; i++)
			bar[i].Grouping(~grouping);
	};

	groupsort << [=] {
		if(~groupsort)
			for (int i = 0; i < 4; i++)
				bar[i].SortGroups(true); //or specify own TabSort
			else
			for (int i = 0; i < 4; i++)
				bar[i].SortGroups(false);
	};

	separators << [=] {
		for (int i = 0; i < 4; i++)
			bar[i].GroupSeparators(~separators);
	};

	stacking << [=] {
		for (int i = 0; i < 4; i++)
			bar[i].Stacking(~stacking);
	};
	
	normalicons ^= stackedicons ^= [=] {
		for (int i = 0; i < 4; i++)
			bar[i].FileIcons(~normalicons, ~stackedicons);
	};

	prompt << [=] {
		if (prompt) {
			bar[0].ConfirmClose = [=](Value v) { return PromptOKCancel(Format("Close '\1%s\1'?", ~v)); };
			bar[0].ConfirmCloseAll = [=] { return PromptOKCancel("Close all tabs?"); };
		}
		else {
			bar[0].ConfirmClose.Clear();
			bar[0].ConfirmCloseAll.Clear();
		}
	};

	allalign << [=] {
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
	};

	valuesort << [=] {
		keysort = false;
		if(~valuesort)
		for (int i = 0; i < 4; i++)
			bar[i].SortTabValues(vo);
		else
		for (int i = 0; i < 4; i++)
			bar[i].SortTabs(false);
	};

	keysort << [=] {
		valuesort = false;
		if(~keysort)
		for (int i = 0; i < 4; i++)
			bar[i].SortTabKeys(vo);
		else
		for (int i = 0; i < 4; i++)
			bar[i].SortTabs(false);
	};

	stacksort << [=] {
		if(~stacksort)
			for (int i = 0; i < 4; i++)
				bar[i].SortStacks(true);
			else
			for (int i = 0; i < 4; i++)
				bar[i].SortStacks(false);
	};
	
	inactive << [=] {
		for (int i = 1; i < 4; i++)
			bar[i].InactiveDisabled(~inactive);
	};
	
	grouping <<= true;
	inactive <<= true;
	normalicons <<= true;
	
	insert.WhenAction();

	OnCursor();
}

void FileTabsExample::OnCursor()
{
	if (bar[0].HasCursor())
		selection = (String)~bar[0];
	else
		selection = "No tab selected";
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

