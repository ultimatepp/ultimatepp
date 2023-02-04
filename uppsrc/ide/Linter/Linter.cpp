#include "Linter.h"

#define IMAGECLASS LinterImg
#define IMAGEFILE <ide/Linter/Linter.iml>
#include <Draw/iml_source.h>

#define KEYGROUPNAME "Linter"
#define KEYNAMESPACE LinterKeys
#define KEYFILE      <ide/Linter/Linter.key>
#include             <CtrlLib/key_source.h>

using namespace LinterKeys;

static String sExeFilePath;

Linter& GetLinter()
{
	return Single<Linter>();
}

bool HasLinter()
{
	return GetLinter().Exists();
}

String Linter::GetConfigFilePath()
{
	return ConfigFile("cppcheck.json");
}

Value Linter::LoadConfig()
{
	return ParseJSON(LoadFile(GetConfigFilePath()));
}

bool Linter::Exists()
{
	static bool b = false;
	ONCELOCK
	{
		String out;
		if(!(b = Sys("which cppcheck", sExeFilePath) == 0))
			sExeFilePath.Clear();
	}
	return b && TheIde();
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
	Vector<String> paths = { GetFilePath() };
	DoCheck(paths);
}

void Linter::CheckPackage()
{
	if(!Exists())
		return;
	Vector<String> paths = { GetFileFolder(GetPackagePath()) };
	DoCheck(paths);
}

void Linter::CheckAll()
{
	if(!Exists())
		return;
	Vector<String> paths;
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		paths.Add() = GetFileFolder(PackagePath(wspc[i]));
}

String Linter::GetCmdLine()
{
	Value v = LoadConfig()["CppCheck"];
	if(IsNull(v))
		return sExeFilePath
			+ " --languge=c++ --std=c++14 --platform=native --enable=all, --xml ";

	int depth = v["depth"];
	int jobs  = v["jobs"];

	Vector<String> severity;
	for(const Value& q : v["severity"])
		if(!IsNull(q))
			severity.Add() = q;

	String s;
	s << sExeFilePath << " "
	  << "--force "
	  << "--xml "
	  << "--language="      << Nvl(v["language"], "c++")	  << " "
	  << "--platform="      << Nvl(v["platform"], "native")   << " "
	  << "--std="           << Nvl(v["standard"], "c++14")    << " "
	  << "--max-ctu-depth=" << AsString(clamp(depth, 1, 6))   << " "
	  << "-j "              << AsString(clamp(jobs, 1, 1024)) << " ";
	if(severity.GetCount())
		s << "--enable="    << Join(severity, ",", true) << " ";
	return s;
}

void Linter::SysCmd(const String& cmd, Event<const String&> cb)
{
	MakeBuild *mb = dynamic_cast<MakeBuild *>(TheIdeContext());
	if(!mb)
		throw Exc("Cannot get TheIde context");
	Host host;
	mb->CreateHost(host, false, false);
	LocalProcess p;
	host.canlog = false;
	if(!host.StartProcess(p, ~cmd))
		throw Exc("Cannot start cppcheck process");
	for(;;) {
		String out = p.Get();
		if(p.IsRunning()) {
			if(!IsNull(out))
				cb(out);
		}
		else {
			if(out.IsVoid())
				break;
			cb(out);
		}
	}
}

void Linter::DoCheck(Vector<String>& paths)
{
	Ide   *ide  = TheIde();
	String tmp  = GetTempFileName();
	String path;
	path << Join(paths, " ", true);

	try {
		FileOut fo(tmp);
		if(!fo)
			throw Exc("Unable to open temporary file");
		ide->ConsoleClear();
		ide->ShowConsole();
		ide->PutConsole("Running cppcheck..");
		Progress pi;
		pi.Title("Cpcheck");
		pi.SetText("Analyzing " + (paths.GetCount() == 1 ? Upp::GetFileName(paths[0]) : "all packages"));
		auto progress = [&fo, &pi](const String& out) {
			if(pi.Canceled())
				throw Exc("User break.");
			fo.Put(out);
			pi.Step();
		};
		SysCmd(GetCmdLine() + path, progress);
		fo.Close();
		ide->Sync();
		ide->PutConsole("Parsing cppcheck output..");
		String results = LoadFile(tmp);
		DeleteFile(tmp);
		ide->ClearErrorsPane();
		ParseResults(ParseXML(results)["results"]);
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

void Linter::ParseResults(const XmlNode& results)
{
	Ide *ide = TheIde();
	
	if(results.IsTag("results")) {
		ParseResults(results["errors"]);
	}
	else
	if(results.IsTag("errors")) {
		for(const XmlNode& node : results) {
			if(!node.IsTag("error"))
				continue;
			const XmlNode& loc = node["location"];
			String severity = node.Attr("severity");
			Ide::ErrorInfo e;
			e.file    = loc.Attr("file");
			e.lineno  = StrInt(loc.Attr("line"));
			e.linepos = StrInt(loc.Attr("column"));
			e.kind    = severity == "error" ? 0 : 1;
			Image img = decode(
				severity,
				"warning",     LinterImg::warning(),
				"style",	   LinterImg::style(),
				"performance", LinterImg::performance(),
				"portability", LinterImg::portability(),
				"information", LinterImg::information(),
				"unusedFunction", LinterImg::unusedfunction(),
				"missingInclude", LinterImg::missinginclude(),
				LinterImg::error());

			Color paper = HighlightSetup::GetHlStyle(e.kind == 0 ? HighlightSetup::PAPER_ERROR
                                                        : HighlightSetup::PAPER_WARNING).color;
			e.message << node.Attr("msg");
			AttrText txt(e.message);
			txt.NormalPaper(paper);
			txt.SetImage(img);
			TheIde()->error.Add(e.file, e.lineno, txt, RawToValue(e));
		}
	}
}

void Linter::StdMenu(Bar& menu)
{
	FileMenu(menu);
	PackageMenu(menu);
	Ide *ide = TheIde();
	menu.Add(CanCheck(), "Analyze all..", [this]() { CheckAll(); })
		.Key(AK_CHECKALL)
		.Help(t_("Analyze project using cppcheck"));
	menu.Separator();
}

void Linter::FileMenu(Bar& menu)
{
	Ide *ide = TheIde();
	menu.Add(CanCheck(), "Analyze " + GetFileName(), [this]() { CheckFile(); })
		.Key(AK_CHECKFILE)
		.Help(t_("Analyze file using cppcheck"));
}

void Linter::PackageMenu(Bar& menu)
{
	Ide *ide = TheIde();
	menu.Add(CanCheck(), "Analyze package " +  GetPackageName(), [this]() { CheckPackage(); })
		.Key(AK_CHECKPACKAGE)
		.Help(t_("Analyze package using cppcheck"));
}
