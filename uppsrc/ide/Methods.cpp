#include <ide/Builders/AndroidBuilder.h>

#include "Methods.h"

void DirTable::SetData(const Value& data)
{
	Vector<String> l = Split((String)data, ';');
	Clear();
	for(int i = 0; i < l.GetCount(); i++)
		Add(l[i]);
}

Value DirTable::GetData() const
{
	String s;
	for(int i = 0; i < GetCount(); i++) {
		if(i) s << ';';
		s << (String)Get(i, 0);
	}
	return s;
}

void DirTable::Init(const char *name)
{
	AutoHideSb();
	AddColumn(name).Edit(edit);
	Appending().Removing().Moving();
	edit_dir.Attach(edit);
	edit_dir.AllFilesType();
	WhenArrayAction = edit <<= callback(this, &DirTable::Modify);
}

DirTable::DirTable()
{
	Init();
	NoHeader();
}

DirTable::DirTable(const char *name)
{
	Init(name);
}

void DirMap::SetData(const Value& data)
{
	Vector<String> l = Split((String)data, ';');
	Clear();
	for(int i = 0; i < l.GetCount(); i++) {
		String li = l[i];
		int f = li.Find('>');
		if(f >= 0)
			Add(li.Left(f), li.Mid(f + 1));
	}
}

Value DirMap::GetData() const
{
	String s;
	for(int i = 0; i < GetCount(); i++) {
		if(i) s << ';';
		s << (String)Get(i, 0) << '>' << (String)Get(i, 1);
	}
	return s;
}

DirMap::DirMap()
{
	AutoHideSb();
	AddColumn("Map local path").Edit(localpath);
	AddColumn("To remote path").Edit(remotepath);
	Appending().Removing().Moving();
	edit_dir.Attach(localpath);
	WhenArrayAction = localpath <<= remotepath <<= callback(this, &DirMap::Modify);
}

void BuilderSetupInterface::InitBuilderSetup(BuilderSetup& bs)
{
	bs.setupCtrl = this;
	InitSetupCtrlsMap(bs.setupCtrlsMap);
}

AndroidBuilderSetup::AndroidBuilderSetup()
{
	CtrlLayout(*this);
	
	sdk_path << [=] { OnSdkPathChange(); };
	sdk_path_download << [=] { LaunchWebBrowser(AndroidSDK::GetDownloadUrl()); };
	sdk_path_download.SetImage(IdeImg::arrow_down());
	DirSelect(sdk_path, sdk_path_select);
	
	ndk_path << [=] { OnNdkPathChange(); };
	ndk_path_download << [=] { LaunchWebBrowser(AndroidNDK::GetDownloadUrl()); };
	ndk_path_download.SetImage(IdeImg::arrow_down());
	DirSelect(ndk_path, ndk_path_select);
	
	jdk_path_download << [=] { LaunchWebBrowser(Jdk::GetDownloadUrl()); };
	jdk_path_download.SetImage(IdeImg::arrow_down());
	DirSelect(jdk_path, jdk_path_select);
}

void AndroidBuilderSetup::InitSetupCtrlsMap(VectorMap<Id, Ctrl*>& map)
{
	map.Add("SDK_PATH",                &sdk_path);
	map.Add("NDK_PATH",                &ndk_path);
	map.Add("JDK_PATH",                &jdk_path);
	map.Add("SDK_PLATFORM_VERSION",    &sdk_platform_version);
	map.Add("SDK_BUILD_TOOLS_RELEASE", &sdk_build_tools_release);
	map.Add("NDK_BLITZ",               &ndk_blitz);
	map.Add("NDK_ARCH_ARMEABI_V7A",    &ndk_arch_armeabi_v7a);
	map.Add("NDK_ARCH_ARM64_V8A",      &ndk_arch_arm64_v8a);
	map.Add("NDK_ARCH_X86",            &ndk_arch_x86);
	map.Add("NDK_ARCH_X86_64",         &ndk_arch_x86_64);
	map.Add("NDK_TOOLCHAIN",           &ndk_toolchain);
	map.Add("NDK_CPP_RUNTIME",         &ndk_cpp_runtime);
	map.Add("NDK_COMMON_CPP_OPTIONS",  &ndk_common_cpp_options);
	map.Add("NDK_COMMON_C_OPTIONS",    &ndk_common_c_options);
}

void AndroidBuilderSetup::New(const String& builder)
{
	OnLoad();
}

void AndroidBuilderSetup::OnLoad()
{
	OnSdkPathChange();
	OnNdkPathChange();
}

void AndroidBuilderSetup::OnCtrlLoad(const String& ctrlKey, const String& value)
{
	VectorMap<Id, Ctrl*> map;
	InitSetupCtrlsMap(map);
	
	if(map.Find(ctrlKey) > -1) {
		Ctrl* ctrl = map.Get(ctrlKey);
		if(ctrl == &sdk_path) {
			OnSdkPathChange0(value);
		}
		else
		if(ctrl == &ndk_path) {
			OnNdkPathChange0(value);
		}
	}
}

void AndroidBuilderSetup::OnShow()
{
	OnSdkShow();
	OnNdkShow();
}

void AndroidBuilderSetup::OnSdkShow()
{
	AndroidSDK sdk(sdk_path.GetData(), true);
	if(!sdk.Validate()) {
		DisableSdkCtrls();
		return;
	}
	EnableSdkCtrls();
	
	if(sdk_platform_version.GetValue().IsNull())
		sdk_platform_version.SetData(sdk.FindDefaultPlatform());
	if(sdk_build_tools_release.GetValue().IsNull())
		sdk_build_tools_release.SetData(sdk.FindDefaultBuildToolsRelease());
}

void AndroidBuilderSetup::OnSdkPathInsert()
{
	String currentPath = sdk_path.GetData();
	
	InsertPath(&sdk_path);
	
	String newPath = sdk_path.GetData();
	if(currentPath != newPath)
		OnSdkPathChange();
}

void AndroidBuilderSetup::OnSdkPathChange()
{
	OnSdkPathChange0(sdk_path.GetData());
	OnSdkShow();
}

void AndroidBuilderSetup::OnSdkPathChange0(const String& sdkPath)
{
	AndroidSDK sdk(sdkPath, true);
	if(sdk.Validate()) {
		LoadPlatforms(sdk);
		LoadBuildTools(sdk);
	}
	else
		ClearSdkCtrls();
}

void AndroidBuilderSetup::OnNdkShow()
{
	AndroidNDK ndk(ndk_path.GetData());
	if(!ndk.Validate()) {
		DisableNdkCtrls();
		return;
	}
	EnableNdkCtrls();
}

void AndroidBuilderSetup::OnNdkPathInsert()
{
	String currentPath = ndk_path.GetData();
	
	InsertPath(&ndk_path);
	
	String newPath = ndk_path.GetData();
	if(currentPath != newPath)
		OnNdkPathChange();
}

void AndroidBuilderSetup::OnNdkPathChange()
{
	OnNdkPathChange0(ndk_path.GetData());
	OnNdkShow();
}

void AndroidBuilderSetup::OnNdkPathChange0(const String& ndkPath)
{
	AndroidNDK ndk(ndkPath);
	if(ndk.Validate()) {
		LoadToolchains(ndk);
		LoadCppRuntimes(ndk);
		
		ndk_arch_arm64_v8a.Set(1);
		ndk_arch_x86_64.Set(1);
		ndk_common_cpp_options.SetData("-std=c++17 -fexceptions -frtti -Wno-logical-op-parentheses");
	}
	else
		ClearNdkCtrls();
}

void AndroidBuilderSetup::LoadPlatforms(const AndroidSDK& sdk)
{
	Vector<String> platforms = sdk.FindPlatforms();
	Sort(platforms, StdGreater<String>());
	
	LoadDropList(sdk_platform_version,
	             platforms,
	             sdk.FindDefaultPlatform());
}

void AndroidBuilderSetup::LoadBuildTools(const AndroidSDK& sdk)
{
	Vector<String> releases = sdk.FindBuildToolsReleases();
	Sort(releases, StdGreater<String>());
	
	LoadDropList(sdk_build_tools_release,
	             releases,
	             sdk.FindDefaultBuildToolsRelease());
}

void AndroidBuilderSetup::LoadToolchains(const AndroidNDK& ndk)
{
	Vector<String> toolchains = ndk.FindToolchains();
	Sort(toolchains, StdGreater<String>());
	
	LoadDropList(ndk_toolchain, toolchains, ndk.FindDefaultToolchain());
}

void AndroidBuilderSetup::LoadCppRuntimes(const AndroidNDK& ndk)
{
	Vector<String> runtimes = ndk.FindCppRuntimes();
	
	LoadDropList(ndk_cpp_runtime, runtimes, ndk.FindDefaultCppRuntime());
}

void AndroidBuilderSetup::LoadDropList(
	DropList& dropList,
	const Vector<String>& values,
	const String& defaultKey)
{
	dropList.Clear();
	
	for(int i = 0; i < values.GetCount(); i++)
		dropList.Add(values[i]);
	
	if(!defaultKey.IsEmpty() && dropList.GetCount()) {
		int idx = dropList.Find(defaultKey);
		if(idx >= 0)
			dropList.SetIndex(idx);
	}
}

void AndroidBuilderSetup::EnableSdkCtrls(bool enable)
{
	sdk_platform_version.Enable(enable);
	sdk_build_tools_release.Enable(enable);
}

void AndroidBuilderSetup::DisableSdkCtrls()
{
	EnableSdkCtrls(false);
}

void AndroidBuilderSetup::ClearSdkCtrls()
{
	sdk_platform_version.Clear();
	sdk_build_tools_release.Clear();
}

void AndroidBuilderSetup::EnableNdkCtrls(bool enable)
{
	ndk_blitz.Enable(enable);
	ndk_arch_armeabi_v7a.Enable(enable);
	ndk_arch_arm64_v8a.Enable(enable);
	ndk_arch_x86.Enable(enable);
	ndk_arch_x86_64.Enable(enable);
	ndk_toolchain.Enable(enable);
	ndk_cpp_runtime.Enable(enable);
	ndk_common_cpp_options.Enable(enable);
	ndk_common_c_options.Enable(enable);
}

void AndroidBuilderSetup::DisableNdkCtrls()
{
	EnableNdkCtrls(false);
}

void AndroidBuilderSetup::ClearNdkCtrls()
{
	ndk_blitz.Set(0);
	ndk_arch_armeabi_v7a.Set(0);
	ndk_arch_arm64_v8a.Set(0);
	ndk_arch_x86.Set(0);
	ndk_arch_x86_64.Set(0);
	ndk_toolchain.Clear();
	ndk_cpp_runtime.Clear();
	ndk_common_cpp_options.Clear();
	ndk_common_c_options.Clear();
}

DefaultBuilderSetup::DefaultBuilderSetup()
{
	CtrlLayout(*this);
	
	paths.Add(path.SizePos(), "PATH - executable directories");
	paths.Add(include.SizePos(), "INCLUDE directories");
	paths.Add(lib.SizePos(), "LIB directories");

	debug_info.Add("0", "None");
	debug_info.Add("1", "Minimal");
	debug_info.Add("2", "Full");
}

void DefaultBuilderSetup::InitSetupCtrlsMap(VectorMap<Id, Ctrl*>& map)
{
	map.Add("COMPILER",                  &compiler);
	map.Add("COMMON_OPTIONS",            &common_options);
	map.Add("COMMON_CPP_OPTIONS",        &common_cpp_options);
	map.Add("COMMON_C_OPTIONS",          &common_c_options);
	map.Add("COMMON_LINK",               &common_link_options);
	map.Add("COMMON_FLAGS",              &common_flags);
	map.Add("DEBUG_INFO",                &debug_info);
	map.Add("DEBUG_BLITZ",               &debug_blitz);
	map.Add("DEBUG_LINKMODE",            &debug_linkmode);
	map.Add("DEBUG_OPTIONS",             &debug_options);
	map.Add("DEBUG_FLAGS",               &debug_flags);
	map.Add("DEBUG_LINK",                &debug_link);
	map.Add("DEBUG_CUDA",                &debug_cuda);
	map.Add("RELEASE_BLITZ",             &release_blitz);
	map.Add("RELEASE_LINKMODE",          &release_linkmode);
	map.Add("RELEASE_OPTIONS",           &speed_options);
	map.Add("RELEASE_FLAGS",             &release_flags);
	map.Add("RELEASE_LINK",              &release_link);
	map.Add("RELEASE_CUDA",              &release_cuda);
	map.Add("DEBUGGER",                  &debugger);
	map.Add("ALLOW_PRECOMPILED_HEADERS", &allow_pch);
	map.Add("DISABLE_BLITZ",             &disable_blitz);
	map.Add("PATH",                      &path);
	map.Add("INCLUDE",                   &include);
	map.Add("LIB",                       &lib);
}

void DefaultBuilderSetup::New(const String& builder)
{
	bool gcc = findarg(builder, "GCC", "CLANG") >= 0;
	if(IsNull(speed_options)) {
		if(gcc)
			speed_options <<= "-O3 -ffunction-sections -fdata-sections";
		else
			speed_options <<= "-O2";
	}
	if(IsNull(debug_options)) {
		if(gcc)
			debug_options <<= "-O0";
		else
			debug_options <<= "-Od";
	}
	if(IsNull(debugger)) {
		if(gcc)
			debugger <<= "gdb";
		else
			debugger <<= "msdev";
	}
	if(IsNull(release_link) && gcc)
		release_link <<= "-Wl,--gc-sections";
}

int CharFilterFileName(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

BuildMethods::BuildMethods()
{
	CtrlLayoutOKCancel(*this, "Build methods");
	Sizeable().Zoomable();
	method.AddColumn("Method").Edit(name);
	name.SetFilter(CharFilterFileName);
	
	method.AddCtrl("BUILDER", builder);
	InitSetups();
	
	method.AddCtrl("SCRIPT", scriptfile);
	method.AddCtrl("LINKMODE_LOCK", linkmode_lock);
	
	open_script.Attach(scriptfile);
	open_script.Type("Build scripts (*.bsc)", "*.bsc")
		.AllFilesType();
	open_script.DefaultExt("bsc");
	method.Appending().Removing().Duplicating();
	method.WhenCursor = THISBACK(ChangeMethod);
	method.WhenBar = THISBACK(MethodMenu);

	for(int i = 0; i < BuilderMap().GetCount(); i++)
		builder.Add(BuilderMap().GetKey(i));

	builder <<= THISBACK(NewBuilder);
	setdefault <<= THISBACK(SetDefault);

	linkmode_lock.SetLabel("Lock link mode");
}

void BuildMethods::MethodMenu(Bar& bar)
{
	method.StdBar(bar);
	bar.Separator();
	bar.Add("Import", THISBACK(Import));
}

void BuildMethods::Import()
{
	if(!Save())
		return;
	FileSel fsel;
	fsel.Type("Build methods (*.bm)", "*.bm")
		.AllFilesType()
		.Multi()
		.DefaultExt("bm");
	if(!fsel.ExecuteOpen())
		return;
	for(int i = 0; i < fsel.GetCount(); i++) {
		String f = LoadFile(fsel[i]);
		if(f.IsVoid()) {
			if(!PromptOKCancel(Format("Failed to load [* \1%s\1]. Continue?", fsel[i])))
				break;
			continue;
		}
		String nf = ConfigFile(GetFileNamePos(fsel[i]));
		if(FileExists(nf) && !PromptOKCancel(Format("File already exists: [* \1%s\1]. Overwrite?", nf)))
			continue;
		if(!SaveFile(nf, f))
			if(!PromptOKCancel(Format("Failed to save [* \1%s\1]. Continue?", nf)))
				break;
	}
	Load();
}

static int sCompare(const Value& v1, const Value& v2)
{
	return (String)v1 < (String)v2;
}

void BuildMethods::NewBuilder()
{
	String builderName = ~builder;
	for(int i = 0; i < setups.GetCount(); i++) {
		Index<String> currentBuilders = StringToBuilders(setups.GetKey(i));
		if(currentBuilders.Find(builderName) > -1)
			setups[i].setupCtrl->New(builderName);
	}
	
	SwitchSetupView();
}

void BuildMethods::ChangeMethod()
{
	String b;
	if(method.IsCursor())
		b = method.Get("BUILDER");
	scriptfile.Enable(b == "SCRIPT");
	SwitchSetupView();
}

void BuildMethods::Load()
{
	method.Clear();
	
	FindFile ff(ConfigFile("*.bm"));
	while(ff) {
		VectorMap<String, String> map;
		String fn = ConfigFile(ff.GetName());
		if(LoadVarFile(fn, map)) {
			String builderName = map.Get("BUILDER", Null);
			int setupIdx = -1;
			String prefix;
			for(int i = 0; i < setups.GetCount(); i++) {
				Index<String> currentBuilders = StringToBuilders(setups.GetKey(i));
				prefix = GetSetupPrefix(currentBuilders);
				if(currentBuilders.Find(builderName) >= 0) {
					setupIdx = i;
					break;
				}
			}
			
			if(setupIdx >= 0)
				setups[setupIdx].setupCtrl->OnLoad();
			
			map = MapBuilderVars(map);
			origfile.Add(fn);
			method.Add(GetFileTitle(fn));
			for(int j = 1; j < method.GetIndexCount(); j++) {
				String key = method.GetId(j).ToString();
				String val = map.Get(key, Null);
				if(setupIdx >= 0) {
					if(key.GetCount() >= prefix.GetCount())
						key.Remove(0, prefix.GetCount());
					setups[setupIdx].setupCtrl->OnCtrlLoad(key, val);
				}
				method.Set(method.GetCount() - 1, j, val);
			}
		}
		ff.Next();
	}
	method.Sort(0, sCompare);
	method.GoBegin();
}

bool BuildMethods::Save()
{
	int i;
	Index<String> name;
	for(i = 0; i < method.GetCount(); i++) {
		String n = method.Get(i, 0);
		if(name.Find(n) >= 0) {
			Exclamation("Duplicate method [* " + DeQtf(n) + "] !");
			return false;
		}
		name.Add(n);
	}
	Index<String> saved;
	for(i = 0; i < method.GetCount(); i++) {
		VectorMap<String, String> map;
		for(int j = 1; j < method.GetIndexCount(); j++)
			map.Add(method.GetId(j).ToString(), method.Get(i, j));
		if(map.Get("BUILDER", Null) != "SCRIPT")
			map.RemoveKey("SCRIPT");
		
		map = SieveBuilderVars(map);
		
		String fn = ConfigFile(String(method.Get(i, 0)) + ".bm");
		if(!SaveVarFile(fn, map)) {
			Exclamation("Error saving [* " + fn + "] !");
			return false;
		}
		saved.Add(fn);
	}
	for(i = 0; i < origfile.GetCount(); i++)
		if(saved.Find(origfile[i]) < 0)
			DeleteFile(origfile[i]);
	return true;
}

void BuildMethods::ShowDefault()
{
	String m = GetDefaultMethod();
	for(int i = 0; i < method.GetCount(); i++)
		if((String)method.Get(i, 0) == m)
			method.SetDisplay(i, 0, BoldDisplay());
		else
			method.SetDisplay(i, 0, StdDisplay());
}

void BuildMethods::SetDefault()
{
	if(method.IsCursor()) {
		SaveFile(ConfigFile("default_method"), method.GetKey());
		ShowDefault();
	}
}

String BuildMethods::GetSetupPrefix(const String& setupKey) const
{
	return setupKey + "_";
}

String BuildMethods::GetSetupPrefix(const Index<String>& buildersGroup) const
{
	return buildersGroup.GetCount() ? GetSetupPrefix(buildersGroup[0]) : "";
}

void BuildMethods::InitSetups()
{
	Index<String> builders = GetBuilders();
	
	String androidKey = BuildersToString(AndroidBuilder::GetBuildersNames());
	androidSetup.InitBuilderSetup(setups.Add(androidKey));
	SieveBuilders(builders, AndroidBuilder::GetBuildersNames());

	String defaultKey = BuildersToString(builders);
	defaultSetup.InitBuilderSetup(setups.Add(defaultKey));
	
	for(int i = 0; i < setups.GetCount(); i++) {
		Index<String> currentBuilders = StringToBuilders(setups.GetKey(i));
		if(currentBuilders.IsEmpty())
			continue;
			
		String setupKey = currentBuilders[0];
		
		ParentCtrl *currentSetup = setups[i].setupCtrl;
		setup.Add(currentSetup->SizePos());
		currentSetup->Hide();
		
		for(int j = 0; j < setups[i].setupCtrlsMap.GetCount(); j++) {
			String ctrlKey = setups[i].setupCtrlsMap.GetKey(j);
			Ctrl*  ctrl    = setups[i].setupCtrlsMap[j];
			method.AddCtrl(GetSetupPrefix(setupKey) + ctrlKey, *ctrl);
		}
	}
}

void BuildMethods::SwitchSetupView()
{
	if(!method.IsCursor()) {
		builder.Hide();
		builderLabel.Hide();
		setup.Hide();
		return;
	}
	else {
		builder.Show();
		builderLabel.Show();
	}
	String builderName = ~builder;
	builderName.IsEmpty() ? setup.Hide() : setup.Show();
	
	if(!builderName.IsEmpty()) {
		for(int i = 0; i < setups.GetCount(); i++) {
			Index<String> currentBuilders = StringToBuilders(setups.GetKey(i));
			
			if(currentBuilders.Find(builderName) > -1) {
				setups[i].setupCtrl->Show();
				setups[i].setupCtrl->OnShow();
			}
			else
				setups[i].setupCtrl->Hide();
		}
	}
}

VectorMap<String, String> BuildMethods::SieveBuilderVars(const VectorMap<String, String>& map)
{
	VectorMap<String, String> sievedMap;
	
	String builder = map.Get("BUILDER", Null);
	if(builder.IsEmpty())
		return VectorMap<String, String>();
	
	for(int i = 0; i < map.GetCount(); i++) {
		String key = map.GetKey(i);
		String value = map[i];
		
		bool toInsert = true;
		for(int j = 0; j < setups.GetCount(); j++) {
			Index<String> currentBuilders = StringToBuilders(setups.GetKey(j));
			if(currentBuilders.IsEmpty())
				continue;
			String prefix = GetSetupPrefix(currentBuilders[0]);
			if(key.StartsWith(prefix)) {
				if(currentBuilders.Find(builder) > -1)
					key.Remove(0, prefix.GetCount());
				else
					toInsert = false;
			}
		}
		if(toInsert)
			sievedMap.Add(key, value);
	}
	
	return sievedMap;
}

VectorMap<String, String> BuildMethods::MapBuilderVars(const VectorMap<String, String>& map)
{
	VectorMap<String, String> mapedMap;
	Index<String> varsToMaped;

	String builder = map.Get("BUILDER", Null);
	if(builder.IsEmpty())
		return VectorMap<String, String>();
	
	for(int i = 0; i < setups.GetCount(); i++) {
		Index<String> currentBuilders = StringToBuilders(setups.GetKey(i));
		if(currentBuilders.IsEmpty())
			continue;
		
		if(currentBuilders.Find(builder) >= 0) {
			String setupPrefix = GetSetupPrefix(currentBuilders);
			
			for(int j = 0; j < map.GetCount(); j++) {
				String ctrlName = map.GetKey(j);
				
				if(setups[i].setupCtrlsMap.Find(ctrlName) > -1)
					mapedMap.Add(setupPrefix + ctrlName, map[j]);
				else
					mapedMap.Add(ctrlName, map[j]);
			}
		}
	}

	return mapedMap;
}

void BuildMethods::SieveBuilders(Index<String>& sievedBuilders,
                                 const Index<String>& builders) const
{
	for(int i = 0; i < builders.GetCount(); i++)
		sievedBuilders.RemoveKey(builders[i]);
}

String BuildMethods::BuildersToString(const Index<String>& builders) const
{
	String str;
	for(int i = 0; i < builders.GetCount(); i++) {
		str << builders[i];
		if(i + 1 < builders.GetCount())
			str << " ";
	}
	return str;
}

Index<String> BuildMethods::StringToBuilders(const String& str) const
{
	Vector<String> vec = Split(str, ' ');
	Index<String> builders;
	for(int i = 0; i < vec.GetCount(); i++)
		builders.Add(vec[i]);
	return builders;
}

Index<String> BuildMethods::GetBuilders() const
{
	Index<String> builders;
	for(int i = 0; i < BuilderMap().GetCount(); i++)
		builders.Add(BuilderMap().GetKey(i));
	return builders;
}

void Ide::SetupBuildMethods()
{
	BuildMethods m;
	m.Load();
	m.ShowDefault();
	m.use_target = use_target;
	m.method.FindSetCursor(method);
	for(;;) {
		int c = m.Run();
		if(c == IDCANCEL)
			break;
		if(c == IDOK && m.Save()) {
			use_target = m.use_target;
			break;
		}
	}
	InvalidateIncludes();
	SyncBuildMode();
	SetBar();
	TriggerIndexer();
	editor.TriggerSyncFile(0);
}

void ExtractIncludes(Index<String>& r, String h)
{
#ifdef PLATFORM_WIN32
	h.Replace("\r", "");
#endif
	Vector<String> ln = Split(h, '\n');
	for(int i = 0; i < ln.GetCount(); i++) {
		String dir = TrimBoth(ln[i]);
		if(DirectoryExists(dir))
			r.FindAdd(NormalizePath(dir));
	}
}

String Ide::GetExternalIncludePath()
{
	SetupDefaultMethod();
	VectorMap<String, String> bm = GetMethodVars(method);

	String include_path = bm.Get("INCLUDE", "");

#ifdef PLATFORM_POSIX
	static String sys_includes;
	ONCELOCK {
		Index<String> r;
		for(int pass = 0; pass < 2; pass++)
			ExtractIncludes(r, HostSys(pass ? "clang -v -x c++ -E /dev/null" : "gcc -v -x c++ -E /dev/null"));
		r.FindAdd("/usr/include_path");
		r.FindAdd("/usr/local/include_path");
		sys_includes = Join(r.GetKeys(), ";");
	}
	if(findarg(bm.Get("BUILDER", ""), "GCC", "CLANG") >= 0)
		MergeWith(include_path, ";", sys_includes);
#endif
#ifdef PLATFORM_WIN32
	static VectorMap<String, String> clang_include;
	int q = clang_include.Find(method);
	if(q < 0) {
		String gcc = GetFileOnPath("clang.exe", bm.Get("PATH", "")); // TODO clang
		Index<String> r;
		if(gcc.GetCount()) {
			String dummy = ConfigFile("dummy.cpp");
			Upp::SaveFile(dummy, String());
			VectorMap<String, String> env(Environment(), 1);
			env.GetAdd("PATH") = Join(SplitDirs(bm.Get("PATH", "") + ';' + env.Get("PATH", "")), ";");
			String environment;
			for(int i = 0; i < env.GetCount(); i++)
				environment << env.GetKey(i) << '=' << env[i] << '\0';
			environment.Cat(0);
			LocalProcess p;
			String out;
			if(p.Start(gcc + " -v -x c++ -E " + dummy, environment) && p.Finish(out) == 0)
				ExtractIncludes(r, out);
			DeleteFile(dummy);
		}
		q = clang_include.GetCount();
		clang_include.Add(method, Join(r.GetKeys(), ";"));
	}
	MergeWith(include_path, ";", clang_include[q]);
#endif
	if(findarg(bm.Get("BUILDER", ""), "ANDROID") >= 0) {
		AndroidNDK ndk(bm.Get("NDK_PATH", ""));
		if(ndk.Validate()) {
			MergeWith(include_path, ";", ndk.GetIncludeDir());
			
			String cppIncludeDir = ndk.GetCppIncludeDir(bm.Get("NDK_CPP_RUNTIME", ""));
			if(!cppIncludeDir.IsEmpty()) {
				MergeWith(include_path, ";", cppIncludeDir);
			}
		}
	}
	return include_path;
}
	
String Ide::GetIncludePath()
{
 // this is 'real' include path defined by current build method, for Alt+J and #include assist
	if(include_path.GetCount())
		return include_path;

	SetupDefaultMethod();
	VectorMap<String, String> bm = GetMethodVars(method);
	include_path = Join(GetUppDirs(), ";");

	MergeWith(include_path, ";", GetExternalIncludePath());

	String include_path = GetExternalIncludePath();
	IncludeAddPkgConfig(include_path, Null);

	return include_path;
}

void Ide::IncludeAddPkgConfig(String& include_path, const String& clang_method)
{
#ifdef PLATFORM_POSIX
	const Workspace& wspc = GetIdeWorkspace();
	Host host;
	if(clang_method.GetCount())
		CreateHost(host, clang_method, false, false);
	else
		CreateHost(host, false, false);
	One<Builder> b = CreateBuilder(&host);
	Index<String> pkg_config;
	Index<String> cfg = PackageConfig(wspc, max(GetPackageIndex(), 0), GetMethodVars(method), mainconfigparam, host, *b);
	String main_conf;
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pkg = wspc.GetPackage(i);
		for(int j = 0; j < pkg.include.GetCount(); j++) {
			MergeWith(include_path, ";", SourcePath(wspc[i], pkg.include[j].text));
		}
		for(String h : Split(Gather(pkg.pkg_config, cfg.GetKeys()), ' '))
			pkg_config.FindAdd(h);
	}
	
	static VectorMap<String, String> cflags;
	for(String s : pkg_config) {
		int q = cflags.Find(s);
		if(q < 0) {
			q = cflags.GetCount();
			cflags.Add(s, HostSys("pkg-config --cflags " + s));
		}
		for(String p : Split(cflags[q], CharFilterWhitespace)) {
			if(p.TrimStart("-I")) {
			#ifdef FLATPAK
				p.Replace("/usr", "/run/host/usr");
			#endif
				MergeWith(include_path, ";", p);
			}
		}
	}
#endif
}

void AddDirs(String& include_path, const String& dir)
{
	MergeWith(include_path, ";", dir);
	for(FindFile ff(dir + "/*.*"); ff; ff.Next())
		if(ff.IsFolder())
			AddDirs(include_path, ff.GetPath());
}

String Ide::GetCurrentIncludePath()
{ // this is include path for libclang / assist
	String include_path;
	
	static String clang_method;

	VectorMap<String, String> bm;
	if(clang_method.GetCount())
		bm = GetMethodVars(clang_method);
	if(bm.GetCount() == 0) {
		String exact, x64, clang;
		for(FindFile ff(ConfigFile(GetMethodName("*"))); ff; ff.Next()) {
			String n = ToLower(GetFileTitle(ff.GetName()));
			if(n == "clangx64")
				exact = n;
			if(n.StartsWith("clangx64"))
				x64 = n;
			if(n.StartsWith("clang"))
				clang = n;
		}
		clang_method = Nvl(exact, x64, clang);
		if(clang_method.GetCount())
			bm = GetMethodVars(clang_method);
	}
	
	include_path = Join(GetUppDirs(), ";") + ';';
	MergeWith(include_path, ";", GetVarsIncludes());
	String inc1 = bm.Get("INCLUDE", "");
	MergeWith(include_path, ";", inc1);
	
	VectorMap<String, String> bm2 = GetMethodVars(method); // add real method include paths at the end
	String inc2 = bm2.Get("INCLUDE", "");
	if(inc1 != inc2)
		MergeWith(include_path, ";", inc2);
	
	IncludeAddPkgConfig(include_path, clang_method);
	
	String main_conf;
	const Workspace& wspc = AssistWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pkg = wspc.GetPackage(i);
		for(int j = 0; j < pkg.GetCount(); j++)
			if(pkg[j] == "import.ext")
				AddDirs(include_path, PackageDirectory(wspc[i]));
	}

	::MainConf(wspc, include_path);

	for(int i = 0; i < wspc.GetCount(); i++) { // internal includes
		const Package& pkg = wspc.GetPackage(i);
		for(int j = 0; j < pkg.include.GetCount(); j++)
			MergeWith(include_path, ";", SourcePath(wspc[i], pkg.include[j].text));
		if(IsExternalMode()) // just add everything..
			MergeWith(include_path, ";", PackageDirectory(wspc[i]));
	}

	return include_path;
}

String Ide::GetCurrentDefines()
{
	Index<String> flags;
	flags = SplitFlags(mainconfigparam, false);
	AddHostFlags(flags);
	String r;
	for(String s : flags)
		MergeWith(r, ";", "flag" + s);

	const Workspace& wspc = AssistWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pkg = wspc.GetPackage(i);
		for(int j = 0; j < pkg.GetCount(); j++) {
			if(pkg[j] == "main.conf") {
				MergeWith(r, ";", "MAIN_CONF");
				return r;
			}
		}
	}
	return r;
}

String Ide::IdeGetIncludePath()
{
	return GetIncludePath();
}
