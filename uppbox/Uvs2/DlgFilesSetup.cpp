#include "Uvs2.h"
#pragma hdrstop

class TextCompareCtrl;

#define LAYOUTFILE <Uvs2/DlgFilesSetup.lay>
#include <CtrlCore/lay.h>

class ExtendedButton : public FrameRight<Button>
{
public:
	ExtendedButton();
	virtual bool HotKey(dword key);
};

ExtendedButton::ExtendedButton()
{
	Width(16);
	SetImage(CtrlImg::right_arrow());
}

bool ExtendedButton::HotKey(dword key)
{
	if(key == K_ALT_ENTER) {
		for(Ctrl *p = this; p; p = p->GetParent())
			if(p->HasFocus()) {
				Action();
				return true;
			}
	}
	return Button::HotKey(key);
}

class DlgFilesSetup : public WithFilesSetupLayout<TopWindow>
{
public:
	typedef DlgFilesSetup CLASSNAME;
	DlgFilesSetup();

	bool Run(UvsJob& job);

private:
	void OnDirBrowse();
	void OnLocalBrowse();

	void OnBackupMenu(Ctrl *owner);
		void OnBackupNone(Ctrl *owner);
		void OnBackupSeparate(Ctrl *owner);
		void OnBackupChapter(Ctrl *owner);

	static void DoBrowseDir(Ctrl& owner, const char *name);

private:
	ExtendedButton dir_browse;
	ExtendedButton local_browse;
	ExtendedButton tree_backup_menu;
	ExtendedButton local_backup_menu;
};

bool UvsJob::Edit() { return DlgFilesSetup().Run(*this); }

DlgFilesSetup::DlgFilesSetup()
{
	CtrlLayoutOKCancel(*this, t_("Job properties"));
	CtrlLayout(misc);
	misc.dir.AddFrame(dir_browse);
	dir_browse <<= THISBACK(OnDirBrowse);
	misc.localrepository.AddFrame(local_browse);
	local_browse <<= THISBACK(OnLocalBrowse);
	tab.Add(misc.SizePos(), t_("Miscellaneous"));
	CtrlLayout(back);
	back.tree_backup.AddFrame(tree_backup_menu);
	tree_backup_menu.WhenPush = THISBACK1(OnBackupMenu, &back.tree_backup);
	back.local_backup.AddFrame(local_backup_menu);
	local_backup_menu.WhenPush = THISBACK1(OnBackupMenu, &back.local_backup);
	tab.Add(back.SizePos(), t_("Backups"));
	CtrlLayout(excf);
	tab.Add(excf.SizePos(), t_("Exclude files"));
	CtrlLayout(excd);
	tab.Add(excd.SizePos(), t_("Exclude folders"));
	CtrlLayout(bin);
	tab.Add(bin.SizePos(), t_("Binary files"));
}

bool DlgFilesSetup::Run(UvsJob& job)
{
	CtrlRetriever rtvr;
	rtvr
		(misc.dir, job.dir)
		(excd.excludedirs, job.excludedirs)
		(bin.binaryfiles, job.binaryfiles)
		(excf.excludefiles, job.excludefiles)
		(misc.author, job.author)
		(misc.localrepository, job.localrepository)
		(misc.host, job.host)
		(misc.user, job.user)
		(misc.password, job.password)
		(misc.hostrepository, job.hostrepository)
		(misc.usecr, job.usecr)
		(misc.passive, job.passive)
		(misc.anonymous, job.anonymous)
		(misc.fullblock, job.fullblock)
		(back.tree_backup, job.tree_backup)
		(back.local_backup, job.local_backup);
	if(TopWindow::Run() != IDOK)
		return false;
	rtvr.Retrieve();
	return true;
}

void DlgFilesSetup::DoBrowseDir(Ctrl& owner, const char *name)
{
	FileSel fsel;
	fsel <<= NativePath((String)~owner);
	if(fsel.ExecuteSelectDir(name))
		owner <<= UnixPath(~fsel);
}

void DlgFilesSetup::OnDirBrowse()
{
	DoBrowseDir(misc.dir, t_("Source file folder"));
}

void DlgFilesSetup::OnLocalBrowse()
{
	DoBrowseDir(misc.localrepository, t_("Local archive folder"));
}

void DlgFilesSetup::OnBackupMenu(Ctrl *owner)
{
	MenuBar bar;
	bar.Add(t_("No backup"), THISBACK1(OnBackupNone, owner));
	bar.Add(t_("Separate files"), THISBACK1(OnBackupSeparate, owner));
	bar.Add(t_("Chapter archive"), THISBACK1(OnBackupChapter, owner));
	bar.Execute(owner -> GetScreenRect().TopRight());
}

void DlgFilesSetup::OnBackupNone(Ctrl *owner)
{
	*owner <<= Null;
}

void DlgFilesSetup::OnBackupSeparate(Ctrl *owner)
{
	String path;
	path << "jar32 a -r " << AppendFileName(GetTempPath(), "@@.j");
	*owner <<= path;
}

void DlgFilesSetup::OnBackupChapter(Ctrl *owner)
{
	String path;
	path << "jar32 ac -r " << AppendFileName(GetTempPath(), "soubory.j");
	*owner <<= path;
}
