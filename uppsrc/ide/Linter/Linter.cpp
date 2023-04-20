#include "Linter.h"

#define IMAGECLASS LinterImg
#define IMAGEFILE <ide/Linter/Linter.iml>
#include <Draw/iml_source.h>

#define KEYGROUPNAME "Linter"
#define KEYNAMESPACE LinterKeys
#define KEYFILE      <ide/Linter/Linter.key>
#include             <CtrlLib/key_source.h>

using namespace LinterKeys;

Linter& GetLinter()
{
	return Single<CppCheck>();
}

bool HasLinter()
{
	return GetLinter().Exists();
}

bool Linter::CanCheck()
{
	return TheIde()
		&& TheIde()->idestate == Ide::EDITING
		&& !IdeIsDebugLock();
}

String Linter::GetFileName()
{
	return Nvl(TheIde()->GetActiveFileName(), Upp::GetFileName(TheIde()->editfile));
}

String Linter::GetFilePath()
{
	return Nvl(TheIde()->GetActiveFilePath(), TheIde()->editfile);
}

String Linter::GetPackageName()
{
	return TheIde()->GetActivePackage();
}

String Linter::GetPackagePath()
{
	return TheIde()->GetActivePackagePath();
}

void Linter::CheckFile()
{
	if(!Exists())
		return;
	Vector<String> paths = { "\"" + GetFilePath() + "\"" };
	DoCheck(paths);
}

void Linter::CheckPackage()
{
	if(!Exists())
		return;
	Vector<String> paths = { "\"" + GetFileFolder(GetPackagePath()) + "\"" };
	DoCheck(paths);
}

void Linter::CheckProject()
{
	if(!Exists())
		return;
	Vector<String> paths;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		paths.Add() = "\"" + GetFileFolder(PackagePath(wspc[i])) + "\"";
	DoCheck(paths);
}

void Linter::SysCmd(const String& cmd, const String& text, Stream& fs)
{
	MakeBuild *mb = dynamic_cast<MakeBuild *>(TheIdeContext());
	if(!mb)
		throw Exc("Cannot get TheIDE context");
	Host host;
	mb->CreateHost(host, false, false);
	LocalProcess p;
	if(!host.StartProcess(p, ~cmd))
		throw Exc("Cannot start linter process");
	Progress pi;
	pi.Title("Linter");
	pi.SetText(text);
	for(;;) {
		String out = p.Get();
		if(p.IsRunning()) {
			if(!IsNull(out))
				fs.Put(out);
		}
		else {
			if(out.IsVoid())
				break;
			else
				fs.Put(out);
		}
		if(pi.StepCanceled()) {
			pi.Close();
			p.Kill();
			throw Exc("User break.");
		}
		IdeProcessEvents();
	}
}

void Linter::DoCheck(Vector<String>& paths)
{
	Ide   *ide  = TheIde();
	String tmp  = GetTempFileName();

	try {
		FileOut fo(tmp);
		if(!fo)
			throw Exc("Unable to open temporary file");
		ide->ConsoleClear();
		ide->ShowConsole();
		ide->PutConsole("Running linter..");
		String text = "Analyzing " + (paths.GetCount() == 1 ? Upp::GetFileName(paths[0]) : "all packages");
		SysCmd(MakeCmdLine(paths), text, fo);
		fo.Close();
		ide->Sync();
		ide->PutConsole("Parsing linter output..");
		String rawresults = LoadFile(tmp);
		DeleteFile(tmp);
		ide->ClearErrorsPane();
		OnResults(rawresults);
		ide->PutConsole("done");
		if(ide->error.GetCount()) {
			ide->BeepMuteExclamation();
			ide->SetBottom(Ide::BERRORS);
		}
		else
			ide->BeepMuteInformation();
	}
	catch(const Exc& e) {
		ide->PutConsole(e);
		ide->BeepMuteExclamation();
		if(FileExists(tmp))
			DeleteFile(tmp);
	}
}

void Linter::StdMenu(Bar& menu)
{
	FileMenu(menu);
	PackageMenu(menu);
	menu.Add(CanCheck(), "Analyze all..", [this]() { CheckProject(); })
		.Key(AK_CHECKALL);
	menu.Add(CanCheck(), "Configure linter..", [this]() { Settings(); })
		.Key(AK_CONFIGURE);
	menu.Separator();
}

void Linter::FileMenu(Bar& menu)
{
	menu.Add(CanCheck(), "Analyze " + GetFileName(), [this]() { CheckFile(); })
		.Key(AK_CHECKFILE);
}

void Linter::PackageMenu(Bar& menu)
{
	menu.Add(CanCheck(), "Analyze package " +  GetPackageName(), [this]() { CheckPackage(); })
		.Key(AK_CHECKPACKAGE);
}
