#include "ide.h"

void Ide::RunArgs() {
	WithRunLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Run options");
	
#ifndef PLATFORM_POSIX
	dlg.consolemode.Hide();
	dlg.console_lable.Hide();
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
	

	for(;;) {
		bool b = ~dlg.runmode == RUN_FILE;
		dlg.stdout_file_lbl.Enable(b);
		dlg.stdout_file.Enable(b);
		int rm = ~dlg.runmode;
		dlg.stdout_file.Enable(rm == RUN_FILE || rm == RUN_FILE_CONSOLE);
		dlg.utf8.Enable(rm != RUN_WINDOW);
		switch(dlg.Run()) {
		case IDOK:
			rundir  = dlg.dir;
			runarg  = ~dlg.arg;
			runmode = ~dlg.runmode;
			runexternal = dlg.external;
			consolemode = dlg.consolemode;
			console_utf8 = ~dlg.utf8;
			stdout_file = ~dlg.stdout_file;
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
	One<Host> h = CreateHost(false);
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
	void LoadPhysicalDevices();
	
private:
	AndroidSDK* sdk;
};

SelectAndroidDeviceDlg::SelectAndroidDeviceDlg(AndroidSDK* sdk) :
	sdk(sdk)
{
	CtrlLayoutOKCancel(*this, "Android device selection");
	
	devicesArray.AddColumn("Device/Model");
	devicesArray.AddColumn("Serial");
	
	LoadPhysicalDevices();
}

String SelectAndroidDeviceDlg::GetSelectedSerial() const
{
	int row = devicesArray.IsCursor() ? devicesArray.GetCursor() : 0;
	return devicesArray.GetCount() ? devicesArray.Get(row, 1) : "";
}

void SelectAndroidDeviceDlg::LoadPhysicalDevices()
{
	Vector<AndroidDevice> devices = sdk->FindDevices();
	for(int i = 0; i < devices.GetCount(); i++) {
		if(devices[i].IsPhysicalDevice()) {
			devicesArray.Add(devices[i].GetModel(), devices[i].GetSerial());
		}
	}
	
	if(devicesArray.GetCount())
		devicesArray.Select(0);
	else
		ok.Disable();
}

void Ide::ExecuteApk()
{
	AndroidSDK sdk(androidSDKPath, true);
	if(!sdk.Validate())
		return;
	
	SelectAndroidDeviceDlg select(&sdk);
	if(select.GetDeviceCount() != 1 && select.Run() != IDOK)
		return;
	if(!select.GetDeviceCount())
		return;
	
	One<Host> host = CreateHost(false);
	Apk apk(target, sdk);
	String packageName = apk.FindPackageName();
	String activityName = apk.FindLaunchableActivity();
	
	Adb adb = sdk.MakeAdb();
	adb.SetSerial(select.GetSelectedSerial());
	host->Execute(adb.MakeInstallCmd(target));
	
	if(!packageName.IsEmpty() && !activityName.IsEmpty())
		host->Execute(adb.MakeLaunchOnDeviceCmd(packageName, activityName));
	
	Cout() << "PID: " << adb.GetPid(packageName) << "\n";
}

void Ide::BuildAndDebug0(const String& srcfile)
{
	if(Build()) {
		One<Host> h = CreateHostRunDir();
		h->ChDir(GetFileFolder(target));
		VectorMap<String, String> bm = GetMethodVars(method);
		String dbg = bm.Get("DEBUGGER", Null);
		if(IsNull(dbg)) {
			if(bm.Get("BUILDER", Null) == "MSC71") {
				String sln = ForceExt(target, ".sln");
				if(GetFileLength(sln) > 0)
					h->Launch("devenv \"" + h->GetHostPath(sln) + "\" "
					// + "\"" + h->GetHostPath(srcfile) + "\"" //TRC, 2011/09/26: wrong devenv argument
					);
				else
					h->Launch("devenv \"" + h->GetHostPath(target)
					//+ "\" \"" + h->GetHostPath(srcfile) //TRC, 2011/09/26: wrong devenv argument
					+ "\" /debugexe "
					);
				return;
			}
			dbg = "gdb";
		}
		else
			h->Launch('\"' + dbg + "\" \""
//			          + h->GetHostPath(srcfile) + ' '
			          + h->GetHostPath(target) + "\"", true);
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

One<Debugger> GdbCreate(One<Host> rval_ host, const String& exefile, const String& cmdline, bool console);
One<Debugger> Gdb_MI2Create(One<Host> rval_ host, const String& exefile, const String& cmdline, bool console);
#ifdef PLATFORM_WIN32
One<Debugger> CdbCreate(One<Host> rval_ host, const String& exefile, const String& cmdline);
One<Debugger> PdbCreate(One<Host> rval_ host, const String& exefile, const String& cmdline);
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
	if(designer)
		EditAsText();
	One<Host> host = CreateHostRunDir();
	host->ChDir(Nvl(rundir, GetFileFolder(target)));
	HideBottom();
	editor.Disable();

	bool console = ShouldHaveConsole();
#ifdef COMPILER_MSC
	if(builder == "GCC")
		if(gdbSelector)
			debugger = Gdb_MI2Create(host, target, runarg, console);
		else
			debugger = GdbCreate(host, target, runarg, console);
	else
		debugger = PdbCreate(host, target, runarg);
#else
	if(gdbSelector)
		debugger = Gdb_MI2Create(pick(host), target, runarg, console);
	else
		debugger = GdbCreate(pick(host), target, runarg, console);
#endif
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
		if(q[0] != 0xe && !debugger->SetBreakpoint(editfile, i, q))
			if(!q.IsEmpty())
				editor.SetBreakpoint(i, "\xe");
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
	                                  *CreateHost(true), *CreateBuilder(~CreateHostRunDir()));
	if(cfg.Find("MSC") >= 0) {
		if(EditPDBExpression("Conditional breakpoint", brk))
			editor.SetBreakpoint(ln, brk);
	}
	else
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

String Ide::GetLogPath()
{
	if(target.GetCount() == 0)
		return Null;
#ifdef PLATFORM_WIN32
	return ForceExt(target, ".log");
#else
	String p = GetFileTitle(target);
	return GetHomeDirFile(".upp/" + p + "/" + p + ".log");
#endif
}

void Ide::OpenLog()
{
	String p = GetLogPath();
	if(FileExists(p))
		EditFile(p);
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
