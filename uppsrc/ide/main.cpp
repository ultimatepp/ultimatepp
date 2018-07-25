#include "ide.h"
#include "CommandLineHandler.h"

#define FUNCTION_NAME UPP_FUNCTION_NAME << "(): "

void DelTemps()
{
	FindFile ff(ConfigFile("*.tmp"));
	while(ff) {
		DeleteFile(ConfigFile(ff.GetName()));
		ff.Next();
	}
}

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )
#endif

extern int MemoryProbeFlags;

void Uninstall();

bool SilentMode;

#if defined(PLATFORM_WIN32)
#include <wincon.h>

void Puts(const char *s)
{
	dword dummy;
	if(!SilentMode)
		WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), s, (int)strlen(s), &dummy, NULL);
}
#endif


#ifdef PLATFORM_POSIX
void Puts(const char *s)
{
	if(!SilentMode)
		puts(s);
}
#endif

bool splash_screen;

int CommaSpace(int c)
{
	return c == ',' ? ' ' : c;
}

void ReduceCache()
{
	String cfgdir = ConfigFile("cfg");
	FindFile ff(AppendFileName(cfgdir, "*.*"));
	while(ff) {
		if(ff.IsFile()) {
			String fn = ff.GetName();
			String ext = GetFileExt(fn);
			if(ext != ".aux" && ext != ".cfg")
				if((Date)Time(ff.GetLastAccessTime()) < GetSysDate() - 14)
					DeleteFile(AppendFileName(cfgdir, fn));
		}
		ff.Next();
	}
}

bool IsAssembly(const String& s)
{
	Vector<String> varlist;
	for(FindFile ff(ConfigFile("*.var")); ff; ff.Next())
		if(ff.IsFile())
			if(GetFileTitle(ff.GetName()) == s)
				return true;
	Vector<String> l = Split(s, ',');
	for(int i = 0; i < l.GetCount(); i++)
		if(FindFile(NormalizePath(l[i])).IsFolder())
			return true;
	return false;
}

void StartEditorMode(const Vector<String>& args, Ide& ide, bool& clset)
{
	if(args.IsEmpty() || clset) {
		return;
	}
	
	Vector<String> dir = Split(LoadFile(GetHomeDirFile("usc.path")), ';');
	for(int i = 0; i < dir.GetCount(); i++)
		ide.UscProcessDirDeep(dir[i]);
	for(int i = 0; i < args.GetCount(); i++) {
		if(args[i] != "-f") {
			String file_path = NormalizePath(args[i]);
			
			Logd() << FUNCTION_NAME << "Opening file \"" << file_path << "\".";
			
			ide.EditFile(file_path);
			ide.FileSelected();
		}
	}
	
	clset = true;
	ide.EditorMode();
}

#ifdef flagMAIN
GUI_APP_MAIN
#else
void AppMain___()
#endif
{
	Logi() << UPP_FUNCTION_NAME << "(): " << SplashCtrl::GenerateVersionInfo(' ');
	
	Ctrl::SetUHDEnabled();

	SetLanguage(LNG_ENGLISH);
	SetDefaultCharset(CHARSET_UTF8);

	CommandLineHandler cmd_handler(CommandLine());
	if (cmd_handler.Handle())
		return;
	auto arg = cmd_handler.GetArgs();

	bool first_install = false;

	SetVppLogSizeLimit(200000000);
#ifdef _DEBUG
//	MemoryLimitKb(1000000);
#endif

#ifdef PLATFORM_POSIX
	LoadUpdaterCfg();

	String home = Environment().Get("UPP_HOME", Null);
	if(!IsNull(home))
		SetHomeDirectory(home);
	FindFile ff(ConfigFile("*.var"));
	if(!ff) {
		if(!Install())
			return;
		SaveFile(ConfigFile("version"), IDE_VERSION);
		first_install = true;
	}
#endif

#ifdef _DEBUG
//	Ctrl::ShowRepaint(20);
#endif

	// _DBG_ InstantSetup(); return;

#ifdef PLATFORM_WIN32
	if(!CheckLicense())
		return;
	AutoInstantSetup();
#endif

	if(!FileExists(BlitzBaseFile()))
		ResetBlitz();

	for(int i = 0; i < arg.GetCount(); i++) {
	#ifdef PLATFORM_WIN32
		if(arg[i] == "!") {
			String cmdline;
			for(++i; i < arg.GetCount(); i++) {
				if(!IsNull(cmdline))
					cmdline << ' ';
				cmdline << arg[i];
			}
			int n = cmdline.GetLength() + 1;
			Buffer<char> cmd(n);
			memcpy(cmd, cmdline, n);
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = TRUE;
			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			AllocConsole();
			SetConsoleTitle(cmdline);
			int time = msecs();
			if(CreateProcess(NULL, cmd, &sa, &sa, TRUE,
				             NORMAL_PRIORITY_CLASS,
			                 NULL, NULL, &si, &pi)) {
				WaitForSingleObject(pi.hProcess, INFINITE);
				dword exitcode = 255;
				GetExitCodeProcess(pi.hProcess, &exitcode);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				Puts("<--- Finished in " + GetPrintTime(time) + ", exitcode: " + AsString(exitcode) + " --->");
			}
			else
				Puts("Unable to launch " + cmdline);
			char h[1];
			dword dummy;
			ReadFile(GetStdHandle(STD_INPUT_HANDLE), h, 1, &dummy, NULL);
			return;
		}
	#endif
	}

#ifdef _DEBUG0
#ifdef PLATFORM_WIN32
	InstantSetup();
#endif
#endif

#ifndef _DEBUG
	try {
#endif
		void RegisterLayDes(); RegisterLayDes();
		void RegisterIconDes(); RegisterIconDes();

		splash_screen = true;

		Ide ide;
		ide.Maximize();
		bool clset = false;
		if(arg.GetCount() >= 2 && IsAlpha(arg[0][0]) && IsAlpha(arg[1][0]) && IsAssembly(arg[0]) && arg[0] != "-f") {
			bool build = arg.GetCount() >= 3 && IsAlpha(arg[2][0]);
		#ifdef PLATFORM_WIN32
			if(build) {
				HMODULE hDLL = LoadLibrary("kernel32");
				bool attach = false;
				if(hDLL) {
					typedef BOOL (WINAPI *AttachConsoleType)(DWORD dwProcessId);
					AttachConsoleType AttachConsole;
					AttachConsole = (AttachConsoleType) GetProcAddress(hDLL, "AttachConsole");
					if(AttachConsole)
						attach = AttachConsole((DWORD)-1);
				}
				if(!attach)
					AllocConsole();
			}
		#endif
			for(int i = 3; i < arg.GetCount(); i++)
				if(arg[i][0] == '-') {
					String x = arg[i];
					for(int i = 1; i < x.GetCount(); i++)
						if(x[i] == 'l')
							SilentMode = true;
				}
			if(!LoadVars(arg[0])) {
				if(build)
					Puts("TheIDE: Invalid assembly\n");
				else
					Exclamation("Invalid assembly!");
				SetExitCode(2);
				return;
			}
			if(!FileExists(SourcePath(arg[1], GetFileTitle(arg[1]) + ".upp"))) {
				if(build)
					Puts("TheIDE: Package does not exist\n");
				else
					Exclamation("Package does not exist!");
				SetExitCode(2);
				return;
			}
			if(build) {
				ide.SetMain(arg[1]);
				clset = true;
				bool stoponerror = false;
				const Workspace& wspc = ide.IdeWorkspace();
				if(!wspc.GetCount())
					return;
				const Array<Package::Config>& f = wspc.GetPackage(0).config;
				if(f.GetCount())
					ide.mainconfigparam = f[0].param;
				String m = arg[2];
				if(!FileExists(ConfigFile((String)m + ".bm"))) {
					SilentMode = false;
					Puts("TheIDE: Invalid build method\n");
					SetExitCode(3);
					return;
				}
				ide.method = m;
				ide.debug.def.blitz = ide.release.def.blitz = 0;
				ide.debug.def.debug = 2;
				ide.release.def.debug = 0;
				ide.debug.package.Clear();
				ide.release.package.Clear();
				ide.debug.linkmode = ide.release.linkmode = 0;
				ide.release.createmap = ide.debug.createmap = false;
				ide.targetmode = 0;
				ide.console.console = true;
				bool clean = false;
				bool makefile = false;
				bool deletedir = false;
				int  exporting = 0;
				String mkf;
				for(int i = 3; i < arg.GetCount(); i++)
					if(arg[i][0] == '>')
						ide.mainconfigparam = Filter(~arg[i] + 1, CommaSpace);
					else
					if(arg[i][0] == '-') {
						String x = arg[i];
						for(int i = 1; i < x.GetCount(); i++)
							switch(x[i]) {
							case 'a':
								clean = true;
								break;
							case 'r':
								ide.targetmode = 1;
								break;
							case 'm':
								ide.release.createmap = ide.debug.createmap = true;
								break;
							case 'b':
								ide.release.def.blitz = ide.debug.def.blitz = 1;
								break;
							case 's':
								ide.debug.linkmode = ide.release.linkmode = 1;
								break;
							case 'd':
								ide.debug.def.debug = 0;
								break;
							case 'S':
								ide.debug.linkmode = ide.release.linkmode = 2;
								break;
							case 'e':
								stoponerror = true;
								break;
							case 'M':
								makefile = true;
								break;
							case 'v':
								ide.console.verbosebuild = true;
								break;
							case 'l':
								break;
							case 'x':
								exporting = 1;
								break;
							case 'X':
								exporting = 2;
								break;
							case 'k':
								deletedir = false;
								break;
							default:
								SilentMode = false;
								Puts("Invalid build option(s)");
								SetExitCode(3);
								return;
							}
					}
					else {
						ide.debug.target_override = ide.release.target_override = true;
						ide.debug.target = ide.release.target = mkf = arg[i];
					}
				if(clean)
					ide.Clean();
				if(exporting) {
					mkf = GetFullPath(mkf);
					Cout() << mkf << '\n';
					RealizeDirectory(mkf);
					if(makefile)
						ide.ExportMakefile(mkf);
					else
						ide.ExportProject(mkf, exporting == 2, false, deletedir);
				}
				else
				if(makefile) {
					ide.SaveMakeFile(IsNull(mkf) ? "Makefile" : mkf, false);
					SetExitCode(0);
				}
				else
				if(ide.Build())
					SetExitCode(0);
				else {
					if(stoponerror)
						PromptOK("TheIDE: Error while building " + DeQtf(arg[1]) + "!");
					SetExitCode(1);
				}
				return;
			}
		}
		
		ide.LoadConfig();

		if(arg.GetCount() == 1) {
			if(arg[0].EndsWith(".upp")) {
				Vector<String> names = Split(arg[0], DIR_SEP);
				int last = names.GetCount() - 1;
				for(int i = last; i >= 0; --i) {
					if (IsAssembly(names[i])) {
						String package = names[last];
						package.Remove(package.GetCount() - 4, 4);
						LoadVars(names[i]);
						ide.SetMain(package);
						clset = true;
						break;
					}
				}
			}
		} else {
			if(arg.GetCount() == 2 && IsAssembly(arg[0])) {
				LoadVars(arg[0]);
				ide.SetMain(arg[1]);
				clset=true;
			}
		}
		
		ide.LoadAbbr();
		ide.SyncCh();

		DelTemps();
		
		StartEditorMode(arg, ide, clset);

		if(splash_screen && !ide.IsEditorMode()) {
			ShowSplash();
			Ctrl::ProcessEvents();
		}

	#ifdef PLATFORM_POSIX
		int p=UpdaterCfg().period;
		if(!IsNull(p)) {
			int next=GetUtcTime()-UpdaterCfg().last+abs(p)*60;
			if(p <= 0 || next <= 0)
				ide.PostCallback(callback1(&ide,&Ide::CheckUpdates,false),0);
			if(p != 0)
				ide.SetTimeCallback(max(0, next),callback1(&ide,&Ide::SetUpdateTimer,abs(p)));
		}
	#endif
			
		ide.editor_bottom.Zoom(0);
		ide.right_split.Zoom(0);
		if(FileExists(ConfigFile("developide"))) {
	#ifdef PLATFORM_WIN32
			InstallCrashDump();
	#endif
			Ini::user_log = true;
		}
		
		String ppdefs = ConfigFile("global.defs");
	#ifndef _DEBUG
		if(!FileExists(ppdefs))
	#endif
			SaveFile(ppdefs, GetStdDefs());

		SetPPDefs(LoadFile(ppdefs));
		
		ide.LoadLastMain();
		if(clset || ide.OpenMainPackage()) {
			ide.SaveLastMain();
			ide.isscanning++;
			ide.MakeTitle();
			if(!ide.IsEditorMode())
				SyncRefs();
			ide.FileSelected();
			ide.isscanning--;
			ide.MakeTitle();
			if(!IdeExit)
				ide.Run();
			ide.SaveConfigOnTime();
			ide.SaveLastMain();
		}
	#ifdef PLATFORM_POSIX
		StoreAsXMLFile(UpdaterCfg(),"SourceUpdater",ConfigFile("updates.xml"));
	#endif
		SaveCodeBase();
		DelTemps();
		DeletePCHFiles();
		ReduceCache();
#ifndef _DEBUG
	}
	catch(const CParser::Error& e) {
		ErrorOK("Parser error " + e);
		LOG("!!!!! Parser error " + e);
	}
	catch(const Exc& e) {
		ErrorOK("Exception " + e);
		LOG("!!!!! Exception " << e);
	}
#ifdef PLATFORM_POSIX
	catch(...) {
		ErrorOK("Unknown exception !");
		LOG("!!!!! Unknown exception");
	}
#endif
#endif
}
