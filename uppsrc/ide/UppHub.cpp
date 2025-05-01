#include "ide.h"

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

static Color s_Make(Color c) {	return Blend(SColorPaper(), c, IsDarkTheme() ? 60 : 20); }

Color StatusPaper(const String& status)
{
	static SColor broken([] { return s_Make(SLtRed()); });
	static SColor experimental([] { return s_Make(SLtYellow()); });
	static SColor stable([] { return s_Make(SLtGreen()); });
	static SColor rolling([] { return s_Make(SLtCyan()); });

	if(status == "broken")
		return broken;
	if(status == "experimental")
		return experimental;
	if(status == "stable")
		return stable;
	if(status == "rolling")
		return rolling;
	return SColorPaper();
}

void VerifyUppHubRequirements()
{
	if (HasGit())
	{
		return;
	}
	
	Loge() << UPP_FUNCTION_NAME << "(): Git is not available!";
	ErrorOK(
		"Git executable was not detected. UppHub will not work properly. Make sure git executable is present in your enviroment path. "
		"You can find more information about requirements "
		"[^https`:`/`/www`.ultimatepp`.org`/app`$ide`$UppHub`_en`-us`.html`#2^ here].&&"
		"You can still use UppHub to view available packages but other operations will not work."
	);
}

class UppHubSettingsDlg final : public WithUppHubSettingsLayout<TopWindow> {
public:
	static constexpr auto GLOBAL_CONFIG_NAME = "UppHubDlgSettings";
	
public:
	UppHubSettingsDlg();
	
	void LoadGlobalSettings();
	void SaveGlobalSettings();
	
private:
	void RefreshCtrls();
};

UppHubSettingsDlg::UppHubSettingsDlg()
{
	CtrlLayoutOKCancel(*this, "Settings");
	FileSelectOpen(url, selfile);
	seturl.WhenAction = [=] {
		RefreshCtrls();
	};
}

void UppHubSettingsDlg::LoadGlobalSettings()
{
	LoadFromGlobal(*this, GLOBAL_CONFIG_NAME);
	RefreshCtrls();
}

void UppHubSettingsDlg::SaveGlobalSettings()
{
	StoreToGlobal(*this, GLOBAL_CONFIG_NAME);
}

void UppHubSettingsDlg::RefreshCtrls()
{
	auto enable = static_cast<bool>(seturl.Get());
	url.Enable(enable);
	selfile.Enable(enable);
}

struct UppHubDlg : WithUppHubLayout<TopWindow> {
	SplitterFrame splitter;
	ArrayCtrl list;
	RichTextView info;
	
	VectorMap<String, UppHubNest> upv;
	Index<String> loaded;
	Progress pi;
	bool loading_stopped;
	String last_package;
	VectorMap<String, String> readme;

	// loading readme in background
	TimeCallback delay;
	String       readme_url;
	bool         loading = false;
	HttpRequest  http;

	UppHubSettingsDlg settings;

	Value LoadJson(const String& url);
	void  Load(int tier, const String& url);
	void  Load();
	void  Install(bool noprompt = false);
	void  Uninstall(bool noprompt = false);
	void  Reinstall();
	void  Install(const Index<String>& ii);
	void  Update();
	void  Sync();
	void  SyncList();
	void  Settings();
	bool  Installed();
	void  UrlLoading();
	void  Menu(Bar& bar);
	
	UppHubNest *Get(const String& name) { return upv.FindPtr(name); }
	UppHubNest *Current()               { return list.IsCursor() ? Get(list.Get("NAME")) : NULL; }

	UppHubDlg();

	bool Key(dword key, int count) override;
};

#define METHOD_NAME "UppHubDlg::" << UPP_FUNCTION_NAME << "(): "

UppHubDlg::UppHubDlg()
{
	CtrlLayoutCancel(*this, "UppHub");
	Sizeable().Zoomable();
	
	parent.Add(list.SizePos());
	parent.AddFrame(splitter.Right(info, 500));
	
	list.AddKey("NAME");
	list.AddColumn("Name").Sorting();
	list.AddColumn("Category").Sorting();
	list.AddColumn("Description");
	
	list.ColumnWidths("109 80 338");
	list.WhenSel = [=] {
		UppHubNest *n = Current();
		http.Abort();
		http.Timeout(0);
		http.New();
		if(n && readme.Find(n->readme) < 0) {
			readme_url = n->readme;
			http.Url(readme_url);
			loading = true;
			delay.KillPost([=] { UrlLoading(); });
		}
		Sync();
	};
	list.WhenLeftDouble = [=] {
		if(list.IsCursor()) {
			if(Installed())
				Reinstall();
			else
				Install();
		}
	};
	list.WhenBar = [=](Bar& bar) {
		if(list.IsCursor()) {
			if(Installed()) {
				bar.Add("Uninstall", [=] { Uninstall(); });
				bar.Add("Reinstall", [=] { Reinstall(); });
			}
			else
				bar.Add("Install", [=] { Install(); });
			bar.Separator();
		}
		Menu(bar);
	};
	reinstall << [=] { Reinstall(); };
	
	more << [=] {
		MenuBar bar;
		Menu(bar);
		bar.Execute();
	};
	
	update << [=] { Update(); };
	
	help << [=] { LaunchWebBrowser("https://www.ultimatepp.org/app$ide$UppHub_en-us.html"); };
	
	search.NullText("Search (Ctrl+K)");
	search.SetFilter([](int c) { return (int)ToUpper(ToAscii(c)); });
	search << [=] { SyncList(); };
	
	experimental <<= true;
	broken <<= false;
	
	category ^= experimental ^= broken ^= [=] { SyncList(); };
	
	settings.LoadGlobalSettings();
}

INITBLOCK
{
	RegisterGlobalConfig(UppHubSettingsDlg::GLOBAL_CONFIG_NAME);
}

bool UppHubDlg::Key(dword key, int count)
{
	if(key == K_CTRL_K) {
		search.SetFocus();
		return true;
	}
	return TopWindow::Key(key, count);
}

void UppHubDlg::Menu(Bar& bar)
{
	Ide *ide = (Ide *)TheIde();
	String hubdir = GetHubDir();
	bool sep = false;
	UppHubNest *n = Current();
	if(Installed()) {
		String p = hubdir + "/" + n->name;
		bar.Add("Open " + n->name + " Directory", [=] { ShellOpenFolder(p); });
		bar.Add("Copy " + n->name + " Directory Path", [=] { WriteClipboardText(p); });
		if(ide)
			bar.Add("Terminal at " + n->name + " Directory", [=] { ide->LaunchTerminal(p); });
		sep = true;
	}

	if(n && !n->website.IsEmpty()) {
		bar.Add("Open " + n->name + " in Browser..", [=] { LaunchWebBrowser(n->website); });
		sep = true;
	}

	if(sep)
		bar.Separator();
	
	bar.Add("Open UppHub Directory", [=] { ShellOpenFolder(hubdir); });
	bar.Add("Copy UppHub Directory Path", [=] { WriteClipboardText(hubdir); });
	if(ide)
		bar.Add("Terminal at UppHub Directory", [=] { ide->LaunchTerminal(hubdir); });
	bar.Separator();
	bar.Add("Install everything..", [=] {
		if(!PromptYesNo("Installing everything will take some time and will need a lot of storage space.&[/ Are you sure?"))
			return;
		Index<String> names;
		for(const UppHubNest& n : upv)
			if(!DirectoryExists(hubdir + "/" + n.name))
				names.Add(n.name);
		if(names.GetCount() == 0) {
			Exclamation("No modules folders.");
			return;
		}
		Install(names);
	});
	bar.Add("Reinstall all..", [=] {
		Index<String> names;
		for(const UppHubNest& n : upv)
			if(DirectoryExists(hubdir + "/" + n.name))
				names.Add(n.name);
		if(names.GetCount() == 0) {
			Exclamation("No modules folders.");
			return;
		}
		if(!PromptYesNo("Reinstall all installed nests?&[/ (This will delete any local changes)]"))
			return;
		for(String n : names)
			if(!DeleteFolderDeep(hubdir + "/" + n, true))
				Exclamation("Failed to delete \1" + n);
		Install(names);
	});
	bar.Add("Uninstall everything..", [=] {
		if(!PromptYesNo("This will completely delete the local UppHub content.&Continue?"))
			return;
		for(FindFile ff(hubdir + "/*"); ff; ff++) {
			String p = ff.GetPath();
			if(ff.IsFolder()) {
				if(!DeleteFolderDeep(p, true))
					Exclamation("Failed to delete \1" + p);
			}
			else
			if(ff.IsFile())
				FileDelete(p);
		}
		SyncList();
	});
	bar.Separator();
	bar.Add("Synchronize repos..", [=] {
		Vector<String> dirs;
		for(FindFile ff(hubdir + "/*"); ff; ff++) {
			if(ff.IsFolder() && DirectoryExists(ff.GetPath() + "/.git"))
				dirs.Add(ff.GetPath());
		}
		if(dirs.GetCount() == 0) {
			Exclamation("No working folders.");
			return;
		}
		RepoSyncDirs(dirs);
		SyncList();
	});
	bar.Separator();
	bar.Add("Set UppHub URL..", [=] { Settings(); });
}

bool UppHubDlg::Installed()
{
	UppHubNest *n = Current();
	return n && DirectoryExists(GetHubDir() + "/" + n->name);
}

void UppHubDlg::UrlLoading()
{
	if(http.Do())
		delay.KillPost([=] { UrlLoading(); });
	else {
		loading = false;
		if(http.IsSuccess())
			readme.GetAdd(readme_url) = http.GetContent();
		Sync();
	}
}

void UppHubDlg::Sync()
{
	action.Disable();
	reinstall.Disable();
	if(list.IsCursor()) {
		action.Enable();
		if(Installed()) {
			action.SetLabel("Uninstall");
			reinstall.Enable();
			action ^= [=] { Uninstall(); };
		}
		else {
			action.SetLabel("Install");
			action ^= [=] { Install(); };
		}
	}
	UppHubNest *n = Current();
	last_package = n && n->packages.GetCount() ? n->packages[0] : String();
	if(!n) return;
	String qtf;
	Color c = StatusPaper(n->status);
	qtf << "{{";
	if(!IsDarkTheme())
		qtf << "@(" << (int)c.GetR() << "." << (int)c.GetG() << "." << (int)c.GetB() << ")";
	qtf << " Category: [* \1" << n->category << "\1], status: [* \1" << n->status << "\1], packages: [* \1" << Join(n->packages, " ") << "\1]";
	if(Installed())
		qtf << "&Status: [* installed]";
	if (!n->website.IsEmpty())
		qtf << "&&Website: [^" << n->website << "^ " << n->website << "]";
	qtf << "}}&&";
	String s = readme.Get(n->readme, String());
	if(s.GetCount()) {
		if(n->readme.EndsWith(".qtf"))
			qtf << s;
		else
		if(n->readme.EndsWith(".md"))
			qtf << MarkdownConverter().Tables().ToQtf(s);
		else
			qtf << "\1" << s;
	}
	else {
		qtf << "[* \1" << n->description << "\1]";
		qtf << (loading ? "&[/ Loading more information]" : "&[/ Failed to get ]\1" + n->readme);
	}
	info <<= qtf;
}

void UppHubDlg::Settings()
{
	settings.LoadGlobalSettings();
	if(settings.Execute() != IDOK) {
		return;
	}
	
	settings.SaveGlobalSettings();
	Load();
}

Value UppHubDlg::LoadJson(const String& url)
{
	String s = LoadFile(url);
	if(IsNull(s)) {
		pi.SetText(url);

		HttpRequest r(url);
		r.WhenWait = r.WhenDo = [&] {
			if(pi.StepCanceled()) {
				r.Abort();
				loading_stopped = true;
			}
		};
		
		r.Execute();
		if (!r.IsSuccess()) {
			String msg = "Failed to execute UppHub download nests request with error code " + IntStr(r.GetStatusCode()) + ".";
			Loge() << METHOD_NAME << msg;
			return ErrorValue(msg);
		}
		
		if(loading_stopped) {
			return ErrorValue();
		}
	
		s = r.GetContent();
	}
	
	Value v = ParseJSON(s);
	if(v.IsError()) {
		s.Replace("&quot;", "\"");
		s.Replace("&amp;", "&");
		v = ParseJSON(s);
		if (v.IsError()) {
			String msg = "Failed to parse Json file.";
			Loge() << METHOD_NAME << msg;
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
			String msg = "Failed to load nests file with error \"" + GetErrorText(v) + "\".";
			ErrorOK(msg);
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
			if(loading_stopped)
				break;
			Load(tier + 1, l);
		}
	}
	catch(ValueTypeError) {}
}

void UppHubDlg::SyncList()
{
	int sc = list.GetScroll();
	Value k = list.GetKey();
	list.Clear();
	for(const UppHubNest& n : upv) {
		String pkgs = Join(n.packages, " ");
		auto AT = [&](const String& s) {
			return AttrText(s).Bold(DirectoryExists(GetHubDir() + "/" + n.name)).NormalPaper(StatusPaper(n.status));
		};
		if(ToUpperAscii(n.name + n.category + n.description + pkgs).Find(~~search) >= 0 &&
		   (IsNull(category) || ~category == n.category) &&
		   (experimental || n.status != "experimental") &&
		   (broken || n.status != "broken"))
			list.Add(n.name, AT(n.name), AT(n.category), AT(n.description), n.name);
	}
		         
	list.DoColumnSort();
	list.ScrollTo(sc);
	if(!list.FindSetCursor(k))
		list.GoBegin();
}

void UppHubDlg::Load()
{
	loading_stopped = false;
	loaded.Clear();
	upv.Clear();

	String url = Nvl(LoadFile(ConfigFile("upphub_root")),
	                 (String)"https://raw.githubusercontent.com/ultimatepp/UppHub/main/nests.json");

	if(settings.seturl)
		url = ~settings.url;

	Load(0, url);
	
	category.ClearList();
	Index<String> cat;
	for(const UppHubNest& n : upv)
		cat.FindAdd(n.category);
	SortIndex(cat);
	category.Add(Null, AttrText("All categories").Italic());
	for(String s : cat)
		category.Add(s);
	category.GoBegin();

	SyncList();

	pi.Close();
}

void UppHubDlg::Update()
{
	if(!PromptYesNo("Pull updates for all modules?"))
		return;
	UrepoConsole console;
	for(const UppHubNest& n : upv) {
		String dir = GetHubDir() + "/" + n.name;
		if(DirectoryExists(dir))
			console.Git(dir, "pull --rebase");
	}
}

void UppHubDlg::Install(const Index<String>& ii_)
{
	Index<String> ii = clone(ii_);
	UrepoConsole console;
	if(ii.GetCount()) {
		int i = 0;
		while(i < ii.GetCount()) {
			String ns = ii[i++];
			UppHubNest *n = Get(ns);
			if(n) {
				String dir = GetHubDir() + '/' + n->name;
				if(!DirectoryExists(dir)) {
					String cmd = "git clone --progress ";
					if(n->branch.GetCount())
						cmd << "-b " + n->branch << ' ';
					cmd << n->repo;
					cmd << " \"" << dir << "\"";
					console.System(cmd);
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
				}
			}
		}
		console.Log("Done", Gray());
		console.Perform();
		InvalidatePackageCache();
	}
	ResetBlitz();
	SyncList();
}

void UppHubDlg::Install(bool noprompt)
{
	if(list.IsCursor() && (noprompt || PromptYesNo("Install " + ~list.GetKey() + "?")))
		Install(Index<String>{ ~list.GetKey() });
}

void UppHubDlg::Uninstall(bool noprompt)
{
	if(list.IsCursor() && (noprompt || PromptYesNo("Uninstall " + ~list.GetKey() + "?"))) {
		if(!DeleteFolderDeep(GetHubDir() + "/" + ~list.GetKey(), true))
			Exclamation("Failed to delete " + ~list.GetKey());
		SyncList();
	}
}

void UppHubDlg::Reinstall()
{
	if(list.IsCursor() && PromptYesNo("Reinstall " + ~list.GetKey() + "?")) {
		Uninstall(true);
		Install(true);
	}
}

String UppHub()
{
	VerifyUppHubRequirements();
	
	UppHubDlg dlg;
	dlg.Load();
	dlg.Run();
	return dlg.last_package;
}

void UppHubAuto(const String& main)
{
	if(IsExternalMode())
		return;

	bool noprompt = false;
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
			break;

		UppHubDlg dlg;
		dlg.Load();
		Index<String> found;
		for(const UppHubNest& n : dlg.upv)
			for(const String& p : n.packages)
				if(missing.Find(p) >= 0)
					found.FindAdd(n.name);

		if(found.GetCount() == missing.GetCount() && missing != pmissing &&
		   (noprompt || PromptYesNo("Missing packages were found in UppHub. Install?"))) {
			dlg.Install(found);
			noprompt = true;
			pmissing = clone(missing);
			continue;
		}

		PromptOK("Some packages are missing:&&[* \1" + Join(missing.GetKeys(), "\n"));
		break;
	}
}
