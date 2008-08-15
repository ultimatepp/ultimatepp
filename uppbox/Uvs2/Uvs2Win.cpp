#include "Uvs2.h"
#pragma hdrstop

#define LAYOUTFILE <Uvs2/Uvs2.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS UvsImg
#define IMAGEFILE "Uvs2.iml"
#include <Draw/iml.h>

//////////////////////////////////////////////////////////////////////

class UvsProject
{
public:
	String source_path;
	String local_repository;
	String ftp_server;
	String ftp_user;
	String ftp_password;
	String ftp_project_path;
	String exclude_extensions;
	String exclude_also;
};

//////////////////////////////////////////////////////////////////////

class DlgProjectSetup : public WithProjectSetupLayout<TopWindow>
{
public:
	typedef DlgProjectSetup CLASSNAME;
	DlgProjectSetup();

	bool Run(UvsProject& project);
};

//////////////////////////////////////////////////////////////////////

DlgProjectSetup::DlgProjectSetup()
{
	CtrlLayoutOKCancel(*this, "Project setup");
}

//////////////////////////////////////////////////////////////////////

bool DlgProjectSetup::Run(UvsProject& project)
{
	::Pump pump;
	pump
	<< PumpData(project.source_path,      source_path)
	<< PumpData(project.local_repository, local_repository)
	<< PumpData(project.ftp_server,       ftp_server)
	<< PumpData(project.ftp_user,         ftp_user)
	<< PumpData(project.ftp_password,     ftp_password)
	<< PumpData(project.ftp_project_path, ftp_project_path)
	<< false;
	if(TopWindow::Run() != IDOK)
		return false;
	pump << true;
	return true;
}

//////////////////////////////////////////////////////////////////////

class DlgExtensions : public WithExtensionsLayout<TopWindow>
{
public:
	typedef DlgExtensions CLASSNAME;
	DlgExtensions();

	bool Run(UvsProject& project);
};

//////////////////////////////////////////////////////////////////////

DlgExtensions::DlgExtensions()
{
	CtrlLayoutOKCancel(*this, "File extensions");
	pair.left.AddColumn("Extensions");
	pair.right.AddColumn("Exclude");
}

//////////////////////////////////////////////////////////////////////

bool DlgExtensions::Run(UvsProject& project)
{
	exclude_also <<= project.exclude_also;
	if(TopWindow::Run() != IDOK)
		return false;
	project.exclude_also = ~exclude_also;
	return true;
}

//////////////////////////////////////////////////////////////////////

class DlgUvs2 : public TopWindow
{
public:
	typedef DlgUvs2 CLASSNAME;
	DlgUvs2();

	void Run();

	void Rescan();

	TOOL(Main)
	TOOL(Project)
		TOOL(ProjectSetup)
		TOOL(ProjectExtensions)
		TOOL(ProjectSynchronize)
	TOOL(File)

	void RefreshFiles();

private:
	OldTreeCtrl files;
	MenuBar menubar;
	ToolBar toolbar;
	InfoBar infobar;

	UvsProject project;
};

void RunDlgUvs2() { DlgUvs2().Run(); }

//////////////////////////////////////////////////////////////////////

DlgUvs2::DlgUvs2()
{
	Sizeable().MaximizeBox();
	Title("Ultimate Versioning System II");
	AddFrame(menubar);
	AddFrame(toolbar);
	Add(files.SizePos());
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::Run()
{
	Rescan();
	RefreshFiles();
	TopWindow::Run();
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::Rescan()
{
	menubar.Set(THISBACK(ToolMain));
	toolbar.Set(THISBACK(ToolMain));
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::ToolMain(Bar& bar)
{
	bar.Add("Project", THISBACK(ToolProject));
	bar.Add("File", THISBACK(ToolFile));
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::ToolProject(Bar& bar)
{
	ToolProjectSetup(bar);
	ToolProjectExtensions(bar);
	bar.Separator();
	ToolProjectSynchronize(bar);
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::ToolProjectSetup(Bar& bar)
{
	bar.Add("Setup", THISBACK(OnProjectSetup));
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::OnProjectSetup()
{
	DlgProjectSetup setupdlg;
	if(setupdlg.Run(project))
		RefreshFiles();
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::ToolProjectExtensions(Bar& bar)
{
	bar.Add("Extensions", THISBACK(OnProjectExtensions));
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::OnProjectExtensions()
{
	DlgExtensions extdlg;
	if(extdlg.Run(project))
		RefreshFiles();
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::ToolProjectSynchronize(Bar& bar)
{
	bar.Add("Synchronize", THISBACK(OnProjectSynchronize));
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::OnProjectSynchronize()
{
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::ToolFile(Bar& bar)
{
}

//////////////////////////////////////////////////////////////////////

void DlgUvs2::RefreshFiles()
{
	files.Text(project.source_path.IsEmpty() ? String("UVS file tree") :  project.source_path);
	files.Image(UvsImg::root());
}

//////////////////////////////////////////////////////////////////////
