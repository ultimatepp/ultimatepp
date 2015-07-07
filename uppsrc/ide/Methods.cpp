#include "Methods.h"

void  TextSwitch::SetData(const Value& data)
{
	String s = data;
	Switch::SetData(atoi((String)data));
}

Value TextSwitch::GetData() const
{
	return AsString(Switch::GetData());
}

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
	
	ndk_path <<= THISBACK(OnNdkPathChange);
	
	ndkDownload.SetImage(IdeImg::DownloadBlack());
	ndkDownload.Tip("Download");
	ndkDownload <<= callback1(LaunchWebBrowser, AndroidNDK::GetDownloadUrl());
	ndk_path.AddFrame(ndkDownload);
	
	ndkBrowse.SetImage(CtrlImg::right_arrow());
	ndkBrowse.Tip("Select directory");
	ndkBrowse <<= THISBACK(OnNdkPathInsert);
	ndk_path.AddFrame(ndkBrowse);
	
	jdkDownload.SetImage(IdeImg::DownloadBlack());
	jdkDownload.Tip("Download");
	jdkDownload <<= callback1(LaunchWebBrowser, Jdk::GetDownloadUrl());
	jdk_path.AddFrame(jdkDownload);
	
	jdkBrowse.SetImage(CtrlImg::right_arrow());
	jdkBrowse.Tip("Select directory");
	jdkBrowse <<= callback1(InsertPath, &jdk_path);
	jdk_path.AddFrame(jdkBrowse);
}

void AndroidBuilderSetup::InitSetupCtrlsMap(VectorMap<Id, Ctrl*>& map)
{
	map.Add("NDK_PATH",                &ndk_path);
	map.Add("JDK_PATH",                &jdk_path);
	map.Add("SDK_PLATFORM_VERSION",    &sdk_platform_version);
	map.Add("SDK_BUILD_TOOLS_RELEASE", &sdk_build_tools_release);
	map.Add("NDK_BLITZ",               &ndk_blitz);
	map.Add("NDK_ARCH_ARMEABI",        &ndk_arch_armeabi);
	map.Add("NDK_ARCH_ARMEABI_V7A",    &ndk_arch_armeabi_v7a);
	map.Add("NDK_ARCH_ARM64_V8A",      &ndk_arch_arm64_v8a);
	map.Add("NDK_ARCH_X86",            &ndk_arch_x86);
	map.Add("NDK_ARCH_X86_64",         &ndk_arch_x86_64);
	map.Add("NDK_ARCH_MIPS",           &ndk_arch_mips);
	map.Add("NDK_ARCH_MIPS64",         &ndk_arch_mips64);
	map.Add("NDK_TOOLCHAIN",           &ndk_toolchain);
	map.Add("NDK_CPP_RUNTIME",         &ndk_cpp_runtime);
	map.Add("NDK_COMMON_CPP_OPTIONS",  &ndk_common_cpp_options);
	map.Add("NDK_COMMON_C_OPTIONS",    &ndk_common_c_options);
}

AndroidBuilderSetup::~AndroidBuilderSetup()
{
	
}

void AndroidBuilderSetup::New(const String& builder)
{
	OnLoad();
	
	ndk_arch_armeabi.Set(1);
	ndk_arch_armeabi_v7a.Set(1);
	ndk_arch_arm64_v8a.Set(1);
	ndk_common_cpp_options.SetData("-fexceptions -frtti");
}

void AndroidBuilderSetup::OnLoad()
{
	String sdkPath = GetAndroidSDKPath();
	
	sdk_path.SetData(sdkPath);
	OnSdkPathChange(sdkPath);
}

void AndroidBuilderSetup::OnCtrlLoad(const String& ctrlKey, const String& value)
{
	VectorMap<Id, Ctrl*> map;
	InitSetupCtrlsMap(map);
	
	if(map.Find(ctrlKey) > -1) {
		Ctrl* ctrl = map.Get(ctrlKey);
		if(ctrl == &ndk_path)
			OnNdkPathChange0(value);
	}
}

void AndroidBuilderSetup::OnShow()
{	
	AndroidSDK sdk(GetAndroidSDKPath(), true);
	if(!sdk.Validate())
		return;
	
	if(((String)sdk_platform_version.GetValue()).IsEmpty())
		sdk_platform_version.SetData(sdk.FindDefaultPlatform());
	if(((String)sdk_build_tools_release.GetValue()).IsEmpty())
		sdk_build_tools_release.SetData(sdk.FindDefaultBuildToolsRelease());
}

void AndroidBuilderSetup::OnSdkPathChange(const String& sdkPath)
{
	AndroidSDK sdk(sdkPath, true);
	if(sdk.Validate()) {
		LoadPlatforms(sdk);
		LoadBuildTools(sdk);
	}
}

void AndroidBuilderSetup::OnNdkPathInsert()
{
	String currentPath = ndk_path.GetData();
	
	InsertPath(&ndk_path);
	if(currentPath != ndk_path.GetData())
		OnNdkPathChange();
}

void AndroidBuilderSetup::OnNdkPathChange()
{
	OnNdkPathChange0(ndk_path.GetData());
}

void AndroidBuilderSetup::OnNdkPathChange0(const String& ndkPath)
{
	AndroidNDK ndk(ndkPath);
	if(ndk.Validate()) {
		LoadToolchains(ndk);
		LoadCppRuntimes(ndk);
	}
}

void AndroidBuilderSetup::LoadPlatforms(const AndroidSDK& sdk)
{
	Vector<String> platforms = pick(sdk.FindPlatforms());
	Sort(platforms, StdGreater<String>());
	
	LoadDropList(sdk_platform_version,
	             platforms,
	             sdk.FindDefaultPlatform());
}

void AndroidBuilderSetup::LoadBuildTools(const AndroidSDK& sdk)
{
	Vector<String> releases = pick(sdk.FindBuildToolsReleases());
	Sort(releases, StdGreater<String>());
	
	LoadDropList(sdk_build_tools_release,
	             releases,
	             sdk.FindDefaultBuildToolsRelease());
}

void AndroidBuilderSetup::LoadToolchains(const AndroidNDK& ndk)
{
	Vector<String> toolchains = pick(ndk.FindToolchains());
	Sort(toolchains, StdGreater<String>());
	
	LoadDropList(ndk_toolchain, toolchains, ndk.FindDefaultToolchain());
}

void AndroidBuilderSetup::LoadCppRuntimes(const AndroidNDK& ndk)
{
	Vector<String> runtimes = pick(ndk.FindCppRuntimes());
	
	LoadDropList(ndk_cpp_runtime, runtimes, ndk.FindDefaultCppRuntime());
}

void AndroidBuilderSetup::LoadDropList(DropList& dropList,
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

DefaultBuilderSetup::DefaultBuilderSetup()
{
	CtrlLayout(*this);
	
	allow_pch.SetLabel("Allow precompiled headers");
	
	paths.Add(path.SizePos(), "PATH - executable directories");
	paths.Add(include.SizePos(), "INCLUDE directories");
	paths.Add(lib.SizePos(), "LIB directories");

	debug_info.Add("0", "None");
	debug_info.Add("1", "Minimal");
	debug_info.Add("2", "Full");
}

DefaultBuilderSetup::~DefaultBuilderSetup()
{
	
}

void DefaultBuilderSetup::InitSetupCtrlsMap(VectorMap<Id, Ctrl*>& map)
{
	map.Add("COMPILER",                  &compiler);
	map.Add("COMMON_OPTIONS",            &common_options);
	map.Add("COMMON_CPP_OPTIONS",        &common_cpp_options);
	map.Add("COMMON_C_OPTIONS",          &common_c_options);
	map.Add("COMMON_FLAGS",              &common_flags);
	map.Add("DEBUG_INFO",                &debug_info);
	map.Add("DEBUG_BLITZ",               &debug_blitz);
	map.Add("DEBUG_LINKMODE",            &debug_linkmode);
	map.Add("DEBUG_OPTIONS",             &debug_options);
	map.Add("DEBUG_FLAGS",               &debug_flags);
	map.Add("DEBUG_LINK",                &debug_link);
	map.Add("RELEASE_BLITZ",             &release_blitz);
	map.Add("RELEASE_LINKMODE",          &release_linkmode);
	map.Add("RELEASE_OPTIONS",           &speed_options);
	map.Add("RELEASE_SIZE_OPTIONS",      &size_options);
	map.Add("RELEASE_FLAGS",             &release_flags);
	map.Add("RELEASE_LINK",              &release_link);
	map.Add("DEBUGGER",                  &debugger);
	map.Add("ALLOW_PRECOMPILED_HEADERS", &allow_pch);
	map.Add("PATH",                      &path);
	map.Add("INCLUDE",                   &include);
	map.Add("LIB",                       &lib);
}

void DefaultBuilderSetup::New(const String& builder)
{
	bool gcc = builder == "GCC" || builder == "GCC32" || builder == "GCC_ARM";
	if(IsNull(speed_options)) {
		if(gcc)
			speed_options <<= "-O3 -ffunction-sections -fdata-sections";
		else
			speed_options <<= "-O2";
	}
	if(IsNull(size_options)) {
		if(gcc)
			size_options <<= "-Os -finline-limit=20 -ffunction-sections -fdata-sections";
		else
			size_options <<= "-O1";
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
			if(!PromptOKCancel(NFormat("Failed to load [* \1%s\1]. Continue?", fsel[i])))
				break;
			continue;
		}
		String nf = ConfigFile(GetFileNamePos(fsel[i]));
		if(FileExists(nf) && !PromptOKCancel(NFormat("File already exists: [* \1%s\1]. Overwrite?", nf)))
			continue;
		if(!SaveFile(nf, f))
			if(!PromptOKCancel(NFormat("Failed to save [* \1%s\1]. Continue?", nf)))
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
	FindFile ff(ConfigFile("*.bm"));
	while(ff) {
		VectorMap<String, String> map;
		String fn = ConfigFile(ff.GetName());
		if(LoadVarFile(fn, map)) {
			String builderName = map.Get("BUILDER");
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
		if(map.Get("BUILDER", "") != "SCRIPT")
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

struct BoldDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
					   Color ink, Color paper, dword style) const {
		w.DrawRect(r, paper);
		DrawSmartText(w, r.left, r.top, r.Width(), (String)q, StdFont().Bold(), ink);
	}
};

void BuildMethods::ShowDefault()
{
	String m = GetDefaultMethod();
	for(int i = 0; i < method.GetCount(); i++)
		if((String)method.Get(i, 0) == m)
			method.SetDisplay(i, 0, Single<BoldDisplay>());
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
	
	String builder = map.Get("BUILDER");
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

	String builder = map.Get("BUILDER");
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
	SyncCodeBase();
	SyncBuildMode();
	SetBar();
}

String Ide::GetIncludePath()
{
	SetupDefaultMethod();
	VectorMap<String, String> bm = GetMethodVars(method);
	String include = GetVar("UPP") + ';' + bm.Get("INCLUDE", "");
#ifdef PLATFORM_POSIX
	static String sys_includes;
	ONCELOCK {
		Index<String> r;
		for(int pass = 0; pass < 2; pass++) {
			String h = Sys(pass ? "clang -v -x c++ -E /dev/null" : "gcc -v -x c++ -E /dev/null");
			Vector<String> ln = Split(h, '\n');
			for(int i = 0; i < ln.GetCount(); i++) {
				String dir = TrimBoth(ln[i]);
				if(DirectoryExists(dir))
					r.FindAdd(NormalizePath(dir));
			}
		}
		r.FindAdd("/usr/include");
		r.FindAdd("/usr/local/include");
		sys_includes = Join(r.GetKeys(), ";");
	}
	MergeWith(include, ";", sys_includes);
#endif
	;

	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pkg = wspc.GetPackage(i);
		for(int j = 0; j < pkg.include.GetCount(); j++)
			MergeWith(include, ";", SourcePath(wspc[i], pkg.include[j].text));
	}
	
	return include;
}

String Ide::IdeGetIncludePath()
{
	return GetIncludePath();
}
