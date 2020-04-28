#include "ide.h"

namespace Upp {
	bool IsSystemThemeDark();
};

void Ide::RunArgs() {
	WithRunLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Run options");
	dlg.Sizeable().Zoomable();
	
#ifndef PLATFORM_POSIX
	dlg.consolemode.Hide();
	dlg.console_label.Hide();
#endif

#ifndef PLATFORM_WIN32
	dlg.advanced_label.Hide();
	dlg.disable_uhd.Hide();
	dlg.darkmode.Hide();
#endif

#ifdef PLATFORM_WIN32
	dlg.darkmode.SetLabel(IsSystemThemeDark() ? "Run in light mode" : "Run in dark mode");
#endif

	SelectDirButton dir_browse("Run in folder");
	dir_browse.Attach(dlg.dir);
	dlg.dir = rundir.ToWString();

	dlg.arg <<= runarg;

	{
		StringStream ss(recent_runarg);
		dlg.arg.SerializeList(ss);
	}

	SaveFileButton stdout_browse("Save STDOUT as");
	stdout_browse.Type("Text files (*.txt)", "*.txt").AllFilesType();
	stdout_browse.Attach(dlg.stdout_file);

	{
		StringStream ss(recent_stdout_file);
		dlg.stdout_file.SerializeList(ss);
		dlg.stdout_file <<= stdout_file;
	}

	dlg.runmode <<= runmode;
	dlg.external = runexternal;
	dlg.consolemode = consolemode;
	dlg.utf8 <<= console_utf8;
	dlg.runmode <<= dlg.Breaker(222);
	dlg.disable_uhd <<= disable_uhd;
	dlg.darkmode <<= darkmode;

	auto Ins = [&](bool file) {
		int l, h;
		dlg.arg.GetSelection(l, h);
		String s = file ? SelectFileOpen("All files\t*.*") : SelectDirectory();
		dlg.arg.SetSelection(l, h);
		if(s.GetCount()) {
			if(s.Find(' ') >= 0)
				s = '\"' + s + '\"';
			dlg.arg.Insert(s);
		}
	};
	dlg.ifile.SetImage(CtrlImg::File());
	dlg.ifile << [&] { Ins(true); };
	dlg.idir.SetImage(CtrlImg::Dir());
	dlg.idir << [&] { Ins(false); };
	
	for(;;) {
		bool b = ~dlg.runmode == RUN_FILE;
		dlg.stdout_file_lbl.Enable(b);
		dlg.stdout_file.Enable(b);
		int rm = ~dlg.runmode;
		dlg.stdout_file.Enable(rm == RUN_FILE || rm == RUN_FILE_CONSOLE);
		dlg.utf8.Enable(rm != RUN_WINDOW);
		switch(dlg.Run()) {
		case IDOK:
			rundir  = ~dlg.dir;
			runarg  = ~dlg.arg;
			runmode = ~dlg.runmode;
			runexternal = dlg.external;
			consolemode = dlg.consolemode;
			console_utf8 = ~dlg.utf8;
			stdout_file = ~dlg.stdout_file;
			disable_uhd = ~dlg.disable_uhd;
			darkmode = ~dlg.darkmode;
			dlg.arg.AddHistory();
			{
				StringStream ss;
				dlg.arg.SerializeList(ss);
				recent_runarg = ss;
			}
			{
				StringStream ss;
				dlg.stdout_file.SerializeList(ss);
				recent_stdout_file = ss;
			}
			return;

		case IDCANCEL:
			return;
		}
	}
}

One<Host> Ide::CreateHostRunDir()
{
	One<Host> h = CreateHost(darkmode, disable_uhd);
	if(IsNull(rundir))
		h->ChDir(GetFileFolder(target));
	else
		h->ChDir(rundir);
	return h;
}

bool Ide::ShouldHaveConsole()
{
	return decode(consolemode, 0, FindIndex(SplitFlags(mainconfigparam, true), "GUI") < 0,
	                           1, true, false);
}

void Ide::BuildAndExecute()
{
	if(Build()) {
		String targetExt = GetFileExt(target);
		if(targetExt == ".apk")
			ExecuteApk();
		else
			ExecuteBinary();
	}
}

void Ide::ExecuteBinary()
{
	int time = msecs();
	One<Host> h = CreateHostRunDir();
	h->ChDir(Nvl(rundir, GetFileFolder(target)));
	String cmdline;
	if(!runexternal)
		cmdline << '\"' << h->GetHostPath(target) << "\" ";
	cmdline << ToSystemCharset(runarg);
		
	int exitcode;
	switch(runmode) {
	case RUN_WINDOW:
		HideBottom();
		h->Launch(cmdline, ShouldHaveConsole());
		break;
	case RUN_CONSOLE:
		ShowConsole();
		PutConsole(String().Cat() << "Executing: " << cmdline);
		console.Sync();
		exitcode = h->ExecuteWithInput(cmdline, console_utf8);
		PutConsole("Finished in " + GetPrintTime(time) + ", exit code: " + AsString(exitcode));
		break;
	case RUN_FILE: {
			HideBottom();
			String fn;
			if(IsNull(stdout_file))
				fn = ForceExt(target, ".ol");
			else
				fn = stdout_file;
			FileOut out(fn);
			if(!out) {
				PromptOK("Unable to open output file [* " + DeQtf(stdout_file) + "] !");
				return;
			}
			if(h->Execute(cmdline, out, console_utf8) >= 0) {
				out.Close();
				EditFile(fn);
			}
		}
	}
}

class SelectAndroidDeviceDlg : public WithSelectAndroidDeviceLayout<TopWindow> {
	typedef SelectAndroidDeviceDlg CLASSNAME;
	
public:
	SelectAndroidDeviceDlg(AndroidSDK* sdk);
	
	int    GetDeviceCount() const    { return devicesArray.GetCount(); }
	String GetSelectedSerial() const;
	
private:
	void LoadDevices();
	
	void OnRefresh();
	
private:
	AndroidSDK* sdk;
};

SelectAndroidDeviceDlg::SelectAndroidDeviceDlg(AndroidSDK* sdk) :
	sdk(sdk)
{
	CtrlLayoutOKCancel(*this, "Android device selection");
	
	devicesArray.AddColumn("Serial Number");
	devicesArray.AddColumn("State");
	
	refresh <<= THISBACK(OnRefresh);
	
	LoadDevices();
}

String SelectAndroidDeviceDlg::GetSelectedSerial() const
{
	int row = devicesArray.IsCursor() ? devicesArray.GetCursor() : 0;
	return devicesArray.GetCount() ? devicesArray.Get(row, 0) : "";
}

void SelectAndroidDeviceDlg::LoadDevices()
{
	Vector<AndroidDevice> devices = sdk->FindDevices();
	for(int i = 0; i < devices.GetCount(); i++) {
		devicesArray.Add(devices[i].GetSerial(), devices[i].GetState());
	}
	
	if(devicesArray.GetCount()) {
		devicesArray.GoBegin();
		ok.Enable();
	}
	else
		ok.Disable();
}

void SelectAndroidDeviceDlg::OnRefresh()
{
	devicesArray.Clear();
	LoadDevices();
}

void Ide::ExecuteApk()
{
	AndroidSDK sdk(GetAndroidSdkPath(), true);
	if(!sdk.Validate())
		return;
	
	SelectAndroidDeviceDlg select(&sdk);
	if(select.GetDeviceCount() != 1 && select.Run() != IDOK)
		return;
	if(!select.GetDeviceCount())
		return;
	
	One<Host> host = CreateHost(darkmode, disable_uhd);
	Apk apk(target, sdk);
	String packageName = apk.FindPackageName();
	String activityName = apk.FindLaunchableActivity();
	
	Adb adb = sdk.MakeAdb();
	adb.SetSerial(select.GetSelectedSerial());
	host->Execute(adb.MakeInstallCmd(target));
	
	if(!packageName.IsEmpty() && !activityName.IsEmpty())
		host->Execute(adb.MakeLaunchOnDeviceCmd(packageName, activityName));
}

void Ide::BuildAndDebug0(const String& srcfile)
{
	if(Build()) {
		One<Host> h = CreateHostRunDir();
		h->ChDir(GetFileFolder(target));
		VectorMap<String, String> bm = GetMethodVars(method);
		String dbg = Nvl(bm.Get("DEBUGGER", Null), "gdb");
		h->Launch('\"' + dbg + "\" \"" + h->GetHostPath(target) + "\"", true);
	}
}

void Ide::BuildAndExtDebug()
{
	BuildAndDebug0(Null);
}

void Ide::BuildAndExtDebugFile()
{
	BuildAndDebug0(editfile);
}

One<Debugger> GdbCreate(One<Host>&& host, const String& exefile, const String& cmdline, bool console);

#ifdef PLATFORM_WIN32
One<Debugger> PdbCreate(One<Host>&& host, const String& exefile, const String& cmdline, bool clang);
#endif

void Ide::BuildAndDebug(bool runto)
{
	VectorMap<String, String> bm = GetMethodVars(method);
	String builder = bm.Get("BUILDER", "");
	
	// TODO: implement debugging on android
	if(builder == "ANDROID") {
		BuildAndExecute();
		return;
	}
	
	if(!Build())
		return;
	if(!FileExists(target))
		return;
	if(designer && !editfile_isfolder)
		EditAsText();
	One<Host> host = CreateHostRunDir();
	host->ChDir(Nvl(rundir, GetFileFolder(target)));
	HideBottom();
	editor.Disable();

	bool console = ShouldHaveConsole();

#ifdef PLATFORM_WIN32
	if(findarg(builder, "GCC") < 0) // llvm-mingw can generate pdb symbolic info
		debugger = PdbCreate(pick(host), target, runarg, builder == "CLANG");
	else
#endif
		debugger = GdbCreate(pick(host), target, runarg, console);

	if(!debugger) {
		IdeEndDebug();
		SetBar();
		editor.Enable();
		return;
	}
	debuglock = 0;
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		for(int i = 0; i < pk.file.GetCount(); i++) {
			String file = SourcePath(n, pk.file[i]);
			LineInfo& ln = Filedata(file).lineinfo;
			for(int i = 0; i < ln.GetCount(); i++) {
				LineInfoRecord& lr = ln[i];
				if(!lr.breakpoint.IsEmpty())
					if(!debugger->SetBreakpoint(file, lr.lineno, lr.breakpoint)) {
						lr.breakpoint = "\xe";
						if(PathIsEqual(file, editfile))
							editor.SetBreakpoint(lr.lineno, "\xe");
					}
			}
		}
	}
	SetBar();
	editor.Enable();
	if(runto) {
		if(!debugger->RunTo())
			IdeEndDebug();
	}
	else
		debugger->Run();
}

void Ide::DebugClearBreakpoints()
{
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		for(int i = 0; i < pk.file.GetCount(); i++) {
			String file = SourcePath(n, pk.file[i]);
			LineInfo& ln = Filedata(file).lineinfo;
			if(debugger)
				for(int i = 0; i < ln.GetCount(); i++) {
					const LineInfoRecord& lr = ln[i];
					if(!lr.breakpoint.IsEmpty())
						debugger->SetBreakpoint(file, lr.lineno, "");
				}
			ClearBreakpoints(ln);
		}
	}
	editor.ClearBreakpoints();
}

void Ide::OnBreakpoint(int i)
{
	if(!editfile.IsEmpty() && !designer && debugger) {
		String q = editor.GetBreakpoint(i);
		if(q[0] != 0xe && !debugger->SetBreakpoint(editfile, i, q)) {
			auto event = editor.WhenBreakpoint;
			editor.WhenBreakpoint = {};
			
			if(!q.IsEmpty())
				editor.SetBreakpoint(i, Null);
			else
				editor.SetBreakpoint(i, "1");
			
			editor.WhenBreakpoint = event;
		}
	}
}

void Ide::DebugToggleBreak()
{
	if(editfile.IsEmpty() || designer)
		return;
	int ln = editor.GetCursorLine();
	String brk = editor.GetBreakpoint(ln);
	if(!brk.IsEmpty())
		editor.SetBreakpoint(ln, Null);
	else
		editor.SetBreakpoint(ln, "1");
	editor.RefreshFrame();
}

void Ide::ConditionalBreak()
{
	if(editfile.IsEmpty() || designer)
		return;
	int ln = editor.GetCursorLine();
	String brk = editor.GetBreakpoint(ln);
	if(brk == "\xe")
		brk = "1";

	Index<String> cfg = PackageConfig(IdeWorkspace(), 0, GetMethodVars(method), mainconfigparam,
	                                  *CreateHost(darkmode, disable_uhd), *CreateBuilder(~CreateHostRunDir()));
#ifdef PLATFORM_WIN32
	if(cfg.Find("MSC") >= 0) {
		if(EditPDBExpression("Conditional breakpoint", brk, NULL))
			editor.SetBreakpoint(ln, brk);
	}
	else
#endif
	if(EditText(brk, "Conditional breakpoint", "Condition"))
		editor.SetBreakpoint(ln, brk);
	editor.RefreshFrame();
}

void Ide::StopDebug()
{
	if(debugger)
		debugger->Stop();
	console.Kill();
	PosSync();
}

bool Ide::EditorTip(CodeEditor::MouseTip& mt)
{
	if(!debugger)
		return false;
	DR_LOG("EditorTip");
	int pos = mt.pos;
	String e;
	String sep;
	while(pos >= 0) {
		String b = editor.ReadIdBackPos(pos, false);
		if(b.GetCount() == 0)
			break;
		e = b + sep + e;
		sep = ".";
		while(pos > 0 && editor.GetChar(pos - 1) == ' ')
			pos--;
		if(pos > 0 && editor.GetChar(pos - 1) == '.')
			--pos;
		else
		if(pos >= 2 && editor.GetChar(pos - 1) == ':' && editor.GetChar(pos - 2) == ':') {
			pos -= 2;
			sep = "::";
		}
		else
		if(pos >= 2 && editor.GetChar(pos - 1) == '>' && editor.GetChar(pos - 2) == '-')
			pos -= 2;
		else
			break;
		while(pos > 0 && editor.GetChar(pos - 1) == ' ')
			pos--;
	}
	DR_LOG("debugger->Tip");
	return debugger->Tip(e, mt);
}
