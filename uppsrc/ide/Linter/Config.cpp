#include "Linter.h"

LinterConfigTab::LinterConfigTab()
{
	language
		.Add("c")
		.Add("c++"); // default.

	standard
		.Add("c89")
		.Add("c99")
		.Add("c11")
		.Add("c++03")
		.Add("c++11")
		.Add("c++14")  // default.
		.Add("c++17")
		.Add("c++20");

	platform
	    .Add("native") // default.
		.Add("unix32")
		.Add("unix64")
		.Add("win32A")
		.Add("win32W")
		.Add("win64");

	depth
		.Add(1)
		.Add(2) // default.
		.Add(3)
		.Add(4)
		.Add(5)
		.Add(6);

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

	SaveChangedFile(Linter::GetConfigFilePath(), Json("CppCheck", j));
}
