#include "Linter.h"

LinterConfigTab::LinterConfigTab()
{
	for(const Value& v : { "c", "c++"})
		language.Add(v);

	for(const Value& v : { "c89", "c99", "c11", "c++03", "c++11", "c++14", "c++17", "c++20"})
		standard.Add(v);

	for(const Value& v :  { "native", "unix32", "unix64", "win32A", "win32W", "win64"})
		platform.Add(v);

	for(int i = 0; i < 6; i++)
		depth.Add(i);
	
	libpath_sel.SetImage(CtrlImg::Dir()) << [this]{ libs <<= SelectDirectory(); };
	pluginpath_sel.SetImage(CtrlImg::Dir()) << [this]{ plugins <<= SelectDirectory(); };
		
	defaults.WhenAction = [this] { Reset(); };
	Reset();
}

void LinterConfigTab::Reset()
{
	language.SetIndex(1);
	standard.SetIndex(5);
	platform.SetIndex(0);
	depth.SetIndex(1);
	jobs <<= CPU_Cores();
	warning     = false;
	style       = false;
	performance = false;
	portability = false;
	information = false;
	unusedfunction = false;
	missinginclude = false;
}

void LinterConfigTab::Load()
{
	Reset();
	
	try
	{
		Value v = Linter::LoadConfig()["CppCheck"];
		if(IsNull(v))
			return;
	
		auto LoadList = [this, &v](DropList& lst, const String& id, const Value& def) {
			int i = lst.FindValue(v[id]);
			lst.SetIndex(i >= 0 ? i : lst.FindValue(def));
		};
	
		LoadList(language, "language", "c++");
		LoadList(standard, "standard", "c++14");
		LoadList(platform, "platform", "native");
		LoadList(depth,    "depth",    2);
	
		jobs <<= clamp((int) v["jobs"], 1, INT_MAX);
		
		libs <<= v["libraries_path"];
		plugins <<= v["plugins_path"];
		options <<= v["cmdline_options"];
	
		for(const Value& q : v["severity"]) {
			if(q == "warning")
				warning = true;
			else
			if(q == "style")
				style = true;
			else
			if(q == "performance")
				performance = true;
			else
			if(q == "portability")
				portability = true;
			else
			if(q == "information")
				information = true;
			else
			if(q == "unusedFunction")
				unusedfunction = true;
			else
			if(q == "missingInglude")
				missinginclude = true;
		}
	}
	catch(...)
	{
		Reset();
	}
}

void LinterConfigTab::Save()
{
	JsonArray ja;

	if(~warning)		ja << "warning";
	if(~style)			ja << "style";
	if(~performance)	ja << "performance";
	if(~portability)	ja << "portability";
	if(~information)	ja << "information";
	if(~unusedfunction)	ja << "unusedFunction";
	if(~missinginclude) ja << "missingInclude";

	Json j;

	j("language", language.GetValue());
	j("standard", standard.GetValue());
	j("platform", platform.GetValue());
	j("depth",    depth.GetValue());
	j("jobs",     ~jobs);
	j("severity", ja);
	j("libraries_path", ~libs);
	j("plugins_path", ~plugins);
	j("cmdline_options", ~options);
	
	SaveChangedFile(Linter::GetConfigFilePath(), Json("CppCheck", j));
}
