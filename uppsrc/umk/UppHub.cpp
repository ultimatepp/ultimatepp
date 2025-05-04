#include "umake.h"

struct UppHubNest : Moveable<UppHubNest> {
	int              tier = -1;
	String           name;
	Vector<String>   packages;
	String           description;
	String           repo;
	String           website;
	String           status = "unknown";
	String           category;
	String           list_name;
	String           readme;
	String           branch;
};

struct UppHubDlg {
	VectorMap<String, UppHubNest> upv;
	Index<String> loaded;

	Value LoadJson(const String& url);
	void  Load(int tier, const String& url);
	void  Load();
	void  Install(const Index<String>& ii, bool update = false);
};

Value UppHubDlg::LoadJson(const String& url)
{
	String s = LoadFile(url);
	if(IsNull(s)) {
		PutVerbose("Fetching UppHub metadata from " << url);

#ifdef PLATFORM_POSIX
		s = Sys("curl --silent " + url);
		if (IsNull(s)) {
			String msg = "Failed to execute UppHub download nests request.";
			return ErrorValue(msg);
		}
#else
		HttpRequest r(url);
		r.Execute();
		if (!r.IsSuccess()) {
			String msg = "Failed to execute UppHub download nests request with error code " + IntStr(r.GetStatusCode()) + ".";
			return ErrorValue(msg);
		}

		s = r.GetContent();
#endif
	}
	
	Value v = ParseJSON(s);
	if(v.IsError()) {
		s.Replace("&quot;", "\"");
		s.Replace("&amp;", "&");
		v = ParseJSON(s);
		if (v.IsError()) {
			String msg = "Failed to parse Json file.";
			return ErrorValue(msg);
		}
	}
	return v;
}

void UppHubDlg::Load(int tier, const String& url)
{
	if(loaded.Find(url) >= 0)
		return;
	loaded.Add(url);
	
	Value v = LoadJson(url);
	if (v.IsError()) {
		auto error_text = GetErrorText(v);
		if (!error_text.IsEmpty()) {
			PutConsole("Failed to load nests file with error \"" + GetErrorText(v) + "\".");
		}
		return;
	}

	try {
		String list_name = v["name"];
		for(Value ns : v["nests"]) {
			String url = ns["url"];
			if(url.GetCount())
				ns = LoadJson(url);
			String name = ns["name"];
			UppHubNest& n = upv.GetAdd(name);
			n.name = name;
			bool tt = tier > n.tier;
			if(tt || n.packages.GetCount() == 0)
				for(Value p : ns["packages"])
					n.packages.Add(p);
			auto Attr = [&](String& a, const char *id) {
				if(tt || IsNull(a))
					a = ns[id];
			};
			Attr(n.description, "description");
			Attr(n.repo, "repository");
			Attr(n.website, "website");
			if(IsNull(n.website))
				n.website = TrimRight(".git", n.repo);
			Attr(n.category, "category");
			Attr(n.status, "status");
			Attr(n.readme, "readme");
			Attr(n.branch, "branch");

			n.list_name = list_name;
		}
		for(Value l : v["links"]) {
			Load(tier + 1, l);
		}
	}
	catch(ValueTypeError) {}
}

void UppHubDlg::Load()
{
	String url = Nvl(LoadFile(ConfigFile("upphub_root")),
	                 (String)"https://raw.githubusercontent.com/ultimatepp/UppHub/main/nests.json");

	Load(0, url);
}

void UppHubDlg::Install(const Index<String>& ii_, bool update)
{
	Index<String> ii = clone(ii_);
	if(ii.GetCount()) {
		int i = 0;
		while(i < ii.GetCount()) {
			String ns = ii[i++];
			UppHubNest *n = upv.FindPtr(ns);
			if(n) {
				String dir = GetHubDir() + '/' + n->name;
				if(!DirectoryExists(dir)) {
					String cmd = GetGitPath() + " clone ";
					if(n->branch.GetCount())
						cmd << "-b " + n->branch << ' ';
					cmd << n->repo;
					cmd << ' ' << dir;
					PutConsole(cmd);
					system(cmd);
					for(String p : FindAllPaths(dir, "*.upp")) {
						Package pkg;
						pkg.Load(p);
						for(const auto& u : pkg.uses)
							for(const UppHubNest& n : upv)
								for(const String& p : n.packages)
									if(u.text == p) {
										ii.FindAdd(n.name);
										break;
									}
					}
				} else if (update) {
					String cmd = GetGitPath() + " -C ";
					cmd << dir << " clean -fxd";
					PutConsole(cmd);
					system(cmd);
					cmd = GetGitPath() + " -C ";
					cmd << dir << " pull";
					PutConsole(cmd);
					system(cmd);
				}
			}
		}
		InvalidatePackageCache();
	}
	ResetBlitz();
}

bool UppHubAuto(const String& main)
{
	Index<String> pmissing;
	for(;;) {
		Workspace wspc;
		wspc.Scan(main);
		Index<String> missing;
		for(int i = 0; i < wspc.GetCount(); i++) {
			String p = wspc[i];
			if(!FileExists(PackageFile(p)))
				missing.FindAdd(p);
		}
		
		if(missing.GetCount() == 0)
			return true;

		UppHubDlg dlg;
		dlg.Load();
		Index<String> found;
		for(const UppHubNest& n : dlg.upv)
			for(const String& p : n.packages)
				if(missing.Find(p) >= 0)
					found.FindAdd(n.name);
		
		if(missing != pmissing) {
			dlg.Install(found);
			pmissing = clone(missing);
			continue;
		}

		return false;
	}
	return true;
}

void UppHubUpdate(const String& main)
{
	UppHubDlg dlg;
	dlg.Load();
	Workspace wspc;
	wspc.Scan(main);
	Index<String> packages;
	for(int i = 0; i < wspc.GetCount(); i++) {
		String pkg = wspc[i];
		for(const UppHubNest& n : dlg.upv)
			for(const String& p : n.packages) {
				if (pkg == p && pkg != main)
					packages.FindAdd(n.name);
			}
	}
	dlg.Install(packages, true);
}
