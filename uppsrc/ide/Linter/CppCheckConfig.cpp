#include "CppCheck.h"

CppCheckConfigDlg::CppCheckConfigDlg(Linter& l)
: Linter::Config(l)
{
	CtrlLayoutOKCancel(*this, "CppCheck Settings");
	
	for(const Value& v : { "c", "c++"})
		language.Add(v);

	for(const Value& v : { "c89", "c99", "c11", "c++03", "c++11", "c++14", "c++17", "c++20"})
		standard.Add(v);

	for(const Value& v : { "native", "unix32", "unix64", "win32A", "win32W", "win64"})
		platform.Add(v);

	for(int i = 0; i < 6; i++)
		depth.Add(i);
	
	CtrlLayout(libs);
	CtrlLayout(addons);
	
	libs.dirpath.WhenAction   = [this] { libs.Load(SelectDirectory(), "*.cfg");  };
	addons.dirpath.WhenAction = [this] { addons.Load(SelectDirectory(), "*.py"); };

	tabs.Add(libs.SizePos(), "Libraries");
	tabs.Add(addons.SizePos(), "Addons");
	
	defaults.WhenAction = [this] { Reset(); };

	Reset();
}

void CppCheckConfigDlg::Reset()
{
	language.SetIndex(1);
	standard.SetIndex(5);
	platform.SetIndex(0);
	depth.SetIndex(1);
	jobs.MinMax(1, CPU_Cores()) <<= CPU_Cores();
	warning     = false;
	style       = false;
	performance = false;
	portability = false;
	information = false;
	unusedfunction = false;
	missinginclude = false;
	verbose = false;
	options <<= "-isrc.tpp -isrcdoc.tpp";
	
#ifdef flagWIN32
	constexpr const char *deflibrarypath = "C:\\Program Files\\CppCheck\\cfg";
	constexpr const char *defpluginspath = "C:\\Program Files\\CppCheck\\addons";
#else
	constexpr const char *deflibrarypath = "/usr/share/cppcheck/cfg";
	constexpr const char *defpluginspath = "/usr/share/cppcheck/addons";
#endif		

	libs.Load(deflibrarypath, "*.cfg");
	addons.Load(defpluginspath, "*.py");
	
	Title("CppCheck Configuration [" + IdeGetCurrentMainPackage() + "]");
}

void CppCheckConfigDlg::Load()
{
	Reset();
	
	try
	{
		Value v = linter.LoadConfig()["CppCheck"];
		if(IsNull(v))
			return;
		
		auto LoadList = [this, &v](DropList& lst, const String& id, const Value& def)
		{
			int i = lst.FindValue(v[id]);
			lst.SetIndex(i >= 0 ? i : lst.FindValue(def));
		};
	
		LoadList(language, "language", "c++");
		LoadList(standard, "standard", "c++14");
		LoadList(platform, "platform", "native");
		LoadList(depth,    "depth",    2);
	
		jobs <<= clamp((int) v["jobs"], 1, INT_MAX);

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

		libs   <<= v["libraries"];
		addons <<= v["addons"];
		
		verbose <<= v["verbose_mode"];
	}
	catch(...)
	{
		Reset();
	}
}

void CppCheckConfigDlg::Save()
{
	JsonArray jl, jq;
	for(const String& s : ~libs) jl << s;
	for(const String& s : ~addons) jq << s;

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
	j("libraries", jl);
	j("addons", jq);
	j("cmdline_options", ~options);
	j("verbose_mode", ~verbose);

	linter.SaveConfig(Json("CppCheck", j).ToString());
}

CppCheckConfigDlg::Pane::Pane()
{
	struct NameDisplay : Display
	{
		void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const override
		{
			StdDisplay().Paint(w, r, GetFileTitle(q.To<String>()), ink, paper, style);
		};
	};
	
	list.AddColumn("Enable").Ctrls<Option>();
	list.AddColumn("Name").SetDisplay(Single<NameDisplay>());;
	list.ColumnWidths("20 300");
	dirpath.NullText("Select a valid library path");
}

void CppCheckConfigDlg::Pane::SetData(const Value& data)
{
	if(IsValueArray(data)) {
		for(const Value& q : data) {
			int i = list.Find(q, 1);
			if(i >= 0)
				list.Set(i, 0, true);
		}
	}
}

Value CppCheckConfigDlg::Pane::GetData() const
{
	ValueArray va;
	for(int i = 0; i < list.GetCount(); i++)
		if(list.Get(i, 0) == true)
			va << list.Get(i, 1);
	return va;
}

void CppCheckConfigDlg::Pane::Load(const String& path, const String& ext)
{
	if(path.IsEmpty())
		return;
	if(!DirectoryExists(path)) {
		dirpath <<= Null;
		return;
	}
	list.Clear();
	for(const FindFile& f : FindFile(AppendFileName(path, ext)))
		list.Add(false, f.GetPath());
	dirpath <<= path;
	list.Enable(list.GetCount());
}


