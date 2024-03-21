#include "CppCheck.h"

static String sExeFilePath;
static bool sVerboseMode = false;

String CppCheck::GetConfigFilePath() const
{
	return ConfigFile(AppendFileName("cppcheck", IdeGetCurrentMainPackage() + "-cppcheck.json"));
}

Value CppCheck::LoadConfig()
{
	String path = GetConfigFilePath();
	if(!FileExists(path))
		RealizePath(path);
	return ParseJSON(LoadFile(path));
}

void CppCheck::SaveConfig(const Value& cfg)
{
	String path = GetConfigFilePath();
	if(!FileExists(path))
		RealizePath(path);
	SaveChangedFile(path, cfg);
}

bool CppCheck::Exists() const
{
	static bool b = false;
	ONCELOCK
	{
#ifdef flagWIN32
		constexpr const char *exe = "C:\\Program Files\\CppCheck\\cppcheck.exe";
		b = FileExists(exe);
		if(b) sExeFilePath << "\"" << exe << "\"";
#else
		b = Sys("which cppcheck", sExeFilePath) == 0;
		if(b) sExeFilePath = TrimRight(sExeFilePath);
		else  sExeFilePath.Clear();
#endif
	}
	return b && TheIde();
}

void CppCheck::Settings()
{
	CppCheckConfigDlg dlg(static_cast<Linter&>(*this));
	dlg.Load();
	if(dlg.ExecuteOK())
		dlg.Save();
}

String CppCheck::MakeCmdLine(Scope sc, Vector<String>& paths)
{
	String path;
	for(const String& s : paths)
		path << "\"" << s << "\" ";
	
	Value v = LoadConfig()["CppCheck"];
	if(IsNull(v))
		return sExeFilePath
			+ " --languge=c++ --std=c++14 --platform=native"
			  " --enable=all, --xml -isrc.tpp -isrcdoc.tpp" + path;

	int depth = v["depth"];
	int jobs  = v["jobs"];
	String opts = v["cmdline_options"];
	
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
		
	for(const String& q : v["libraries"])
		if(FileExists(q))
			s << "--library=\"" << q << "\" ";

	for(const String& q : v["addons"])
		if(FileExists(q))
			s << "--plugin=\"" << q << "\" ";

	sVerboseMode = v["verbose_mode"];

	return s + opts + " " + path;
}

void CppCheck::OnResults(const String& results)
{
	DecodeXML(ParseXML(results)["results"]);
}

void CppCheck::DecodeXML(const XmlNode& results)
{
	if(results.IsTag("results")) {
		DecodeXML(results["errors"]);
	}
	else
	if(results.IsTag("errors")) {
		for(const XmlNode& node : results) {
			if(!node.IsTag("error"))
				continue;
			const XmlNode& loc = node["location"];
			String severity = node.Attr("severity");
			Ide::ListLineInfo e;
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
			const String& verbosemsg = node.Attr("verbose");
			e.message = node.Attr("msg");
			int linecy;
			AttrText txt(TheIde()->FormatErrorLine(e.message, linecy));
			txt.NormalPaper(paper);
			txt.SetImage(img);
			ArrayCtrl& error = TheIde()->error;
			error.Add(e.file, e.lineno, txt, RawToValue(e));
			ValueArray notes;
			if(sVerboseMode) {
				e.message = node.Attr("verbose");
				if(e.message.GetCount())
					notes.Add(RawToValue(e));
			}
			if(node.Attr("inconclusive") == "true"){
				e.message = "[Note that this is an inconclusive result!]";
				notes.Add(RawToValue(e));
			}
			error.Set(error.GetCount() - 1, "NOTES", notes);
			error.SetLineCy(error.GetCount() - 1, linecy);
		}
	}
}

INITIALIZER(CppCheck)
{
	RegisterLinterModule(Single<CppCheck>("CppCheck"));
}


