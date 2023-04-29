#include "Linter.h"

#define IMAGECLASS LinterImg
#define IMAGEFILE <ide/Linter/Linter.iml>
#include <Draw/iml_source.h>

#define KEYGROUPNAME "Linter"
#define KEYNAMESPACE LinterKeys
#define KEYFILE      <ide/Linter/Linter.key>
#include             <CtrlLib/key_source.h>

using namespace LinterKeys;

static String sActiveModuleName;

static Vector<Linter*>& sLM()
{
	static Vector<Linter*> m;
	return m;
}

void RegisterLinterModule(Linter& linter_module)
{
	sLM().Add(&linter_module);
}

int GetLinterModuleCount()
{
	return sLM().GetCount();
}

Linter& GetLinterModule(int i)
{
	ASSERT(i >= 0 && i < GetLinterModuleCount());
	return *sLM()[i];
}

Linter* GetActiveLinterModulePtr()
{
	for(Linter *p : sLM())
		if(p->Exists() && p->GetName() == sActiveModuleName)
			return p;
	
	for(Linter *p : sLM())
		if(p->Exists()) {
			sActiveModuleName = p->GetName();
			return p;
		}

	return nullptr;
}

bool Linter::CanCheck() const
{
	return TheIde()
		&& TheIde()->idestate == Ide::EDITING
		&& !IdeIsDebugLock();
}

String Linter::GetFileName() const
{
	return Nvl(TheIde()->GetActiveFileName(), Upp::GetFileName(TheIde()->editfile));
}

String Linter::GetFilePath() const
{
	return Nvl(TheIde()->GetActiveFilePath(), TheIde()->editfile);
}

String Linter::GetPackageName() const
{
	return TheIde()->GetActivePackage();
}

String Linter::GetPackagePath() const
{
	return TheIde()->GetActivePackagePath();
}

void Linter::CheckFile()
{
	if(!Exists())
		return;
	Vector<String> paths = { GetFilePath() };
	DoCheck(Scope::File, paths);
}

void Linter::CheckPackage()
{
	if(!Exists())
		return;
	Vector<String> paths = { GetFileFolder(GetPackagePath()) };
	DoCheck(Scope::Package, paths);
}

void Linter::CheckProject()
{
	if(!Exists())
		return;
	Vector<String> paths;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		paths.Add() = GetFileFolder(PackagePath(wspc[i]));
	DoCheck(Scope::Project, paths);
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

void Linter::DoCheck(Scope sc, Vector<String>& paths)
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
		SysCmd(MakeCmdLine(sc, paths), text, fo);
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

void sListMenu(Linter& l, Bar& menu)
{
	auto list = [&l](Bar& menu) {
		Vector<int> ndx = FindAll(sLM(), [](const Linter *p) { return p->Exists(); });
		for(int i : ndx) {
			const Linter& q = GetLinterModule(i);
			menu.Add(q.GetName(), [&q]() { sActiveModuleName = q.GetName(); })
				.Radio(q.GetName() == l.GetName());
		}
		menu.Separator();
		menu.Add("Configure " + l.GetName(), [&l]() { l.Settings(); })
			.Key(AK_CONFIGURE);
	};
	
	menu.Sub("Static analyzers", list);
}

void sFileMenu(Linter& l, String name, Bar& menu)
{
	menu.Add(l.CanCheck(), "Analyze " + name, [&l]() { l.CheckFile(); })
		.Key(AK_CHECKFILE);
}

void sPackageMenu(Linter& l, String name, Bar& menu)
{
	menu.Add(l.CanCheck(), "Analyze package " +  name, [&l]() { l.CheckPackage(); })
		.Key(AK_CHECKPACKAGE);
}

void Linter::StdMenu(Bar& menu)
{
	Linter *p = GetActiveLinterModulePtr();
	if(!p)
		return;
	sListMenu(*p, menu);
	sFileMenu(*p, p->GetFileName(), menu);
	sPackageMenu(*p, p->GetPackageName(), menu);
	menu.Add(p->CanCheck(), "Analyze all..", [p]() { p->CheckProject(); })
		.Key(AK_CHECKALL);
	menu.Separator();
}

void Linter::FileMenu(Bar& menu)
{
	Linter *p = GetActiveLinterModulePtr();
	if(p) sFileMenu(*p, p->GetFileName(), menu);
}

void Linter::PackageMenu(Bar& menu)
{
	Linter *p = GetActiveLinterModulePtr();
	if(p) sPackageMenu(*p, p->GetFileName(), menu);
}

INITIALIZER(Linter)
{
	RegisterGlobalSerialize("Linters", [](Stream& s) {
		int version = 0;
		s / version;
		s % sActiveModuleName;
	});
}