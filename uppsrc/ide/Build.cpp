#include "ide.h"

void Ide::ConsoleShow()
{
	ShowConsole();
	console.Sync();
}

void Ide::ConsoleSync()
{
	console.Sync();
}

Vector<String> Ide::PickErrors()
{
	return console.PickErrors();
}

void Ide::DoProcessEvents()
{
	ProcessEvents();
}

String Ide::GetMain()
{
	return main;
}

void Ide::BeginBuilding(bool clear_console)
{
	SetupDefaultMethod();
	HdependTimeDirty();
	Builder::cmdx_cache.Clear();
	Renumber();
	StopDebug();
	ShowConsole();
	SaveFile();
	SaveWorkspace();
	SetIdeState(BUILDING);
	console.Kill();
	console.ClearError();
	ClearErrorsPane();
	if(clear_console)
		console.Clear();
	build_time = msecs();
	cmdout.Clear();
	InvalidateIncludes();
}

void Ide::EndBuilding(bool ok)
{
	console.EndGroup();
	console.Wait();
	Vector<String> errors = console.PickErrors();
	for(String p : errors)
		DeleteFile(p);
	if(!errors.IsEmpty())
		ok = false;
	PutConsole("");
	PutConsole((ok ? "OK. " : "There were errors. ") + GetPrintTime(build_time));
	SetIdeState(EDITING);
	if(GetTopWindow()->IsOpen()) {
		if(ok)
			BeepMuteInformation();
		else
			BeepMuteExclamation();
	}
	ShowConsole();
}

void Ide::DoBuild()
{
	Build();
}

void Ide::PackageBuild()
{
	InitBlitz();
	BeginBuilding(true);
	const Workspace& wspc = IdeWorkspace();
	int pi = GetPackageIndex();
	if(pi >= 0 && pi <= wspc.GetCount()) {
		Vector<String> linkfile, immfile;
		String linkopt;
		bool ok = BuildPackage(wspc, pi, 0, 1, mainconfigparam, Null, linkfile, immfile, linkopt);
		EndBuilding(ok);
		SetErrorEditor();
	}
}

void Ide::StopBuild()
{
	if(idestate == BUILDING) {
		console.Kill();
		PutConsole("User break.");
		SetIdeState(EDITING);
	}
}

String Ide::GetOutputDir()
{
	return GetFileFolder(target);
}

String Ide::GetConfigDir()
{
	return GetHomeDirFile(".config/u++/" + GetFileTitle(target));
}

void Ide::PackageClean()
{
	const Workspace& wspc = IdeWorkspace();
	int pi = GetPackageIndex();
	if(pi >= 0 && pi < wspc.GetCount()) {
		console.Clear();
		CleanPackage(wspc, pi);
	}
}

void Ide::CleanUppOut()
{
	String out = GetOutputDir();
	if(!PromptYesNo(Format("Erase the whole output directory [* \1%s\1]?", out)))
		return;
	console.Clear();
	PutConsole("UPPOUT cleanup...");
	DeleteFolderDeep(out);
	PutConsole("(done)");
	HideBottom();
}

void Ide::FileCompile()
{
	if(editfile.IsEmpty())
		return;
	linking = false;
	SwitchHeader();
	BeginBuilding(true);
	const Workspace& wspc = IdeWorkspace();
	bool ok = true;
	onefile = editfile;
	if(wspc.GetCount()) {
		Vector<String> linkfile, immfile;
		String linkopt;
		MainConf(wspc);
		for(int i = 0; i < wspc.GetCount(); i++)
			BuildPackage(wspc, i, 1, wspc.GetCount(), mainconfigparam, Null, linkfile, immfile, linkopt, false);
	}
	onefile.Clear();
	EndBuilding(ok);
	SetErrorEditor();
}

void Ide::Preprocess(bool asmout) {
	if(editfile.IsEmpty())
		return;
	int pi = GetPackageIndex();
	if(pi < 0) return;
	SwitchHeader();
	String pfn = ConfigFile(GetFileTitle(editfile) + ".i.tmp");
	DeleteFile(pfn);
	const Workspace& wspc = IdeWorkspace();
	if(pi >= wspc.GetCount())
		return;
	Host host;
	CreateHost(host, darkmode, disable_uhd);
	One<Builder> b = CreateBuilder(&host);
	Vector<String> linkfile;
	String linkopt;
	b->config = PackageConfig(wspc, pi, GetMethodVars(method), mainconfigparam, host, *b);
	console.Clear();
	PutConsole((asmout ? "Compiling " : "Preprocessing ") + editfile);
	b->Preprocess(wspc[pi], editfile, pfn, asmout);
	HideBottom();
	if(FileExists(pfn)) {
		EditFile(pfn);
		if(!editor.IsReadOnly())
			ToggleReadOnly();
	}
}

void Ide::CreateMakefile()
{
	const Workspace& wspc = IdeWorkspace();
	if(wspc.GetCount() == 0) {
		PutConsole("Project is empty!");
		return;
	}
	FileSel mfout;
	mfout.AllFilesType();
	mfout <<= AppendFileName(PackageDirectory(wspc[0]), "Makefile");
	if(!mfout.ExecuteSaveAs("Save makefile as"))
		return;
	SaveMakeFile(~mfout, true);
}
