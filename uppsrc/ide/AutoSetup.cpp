#include "ide.h"

extern FileSel& sSD();

static void sSetFolder(EditField *f)
{
	if(!sSD().ExecuteSelectDir()) return;
	*f <<= ~sSD();
}

void DirSel(EditField& f, FrameRight<Button>& b)
{
	f.AddFrame(b);
	b <<= callback1(&sSetFolder, &f);
	b.SetImage(CtrlImg::smallright()).NoWantFocus();
}

String NormalizePathNN(const String& path)
{
	return IsNull(path) ? path : NormalizePath(path);
}


#ifdef PLATFORM_WIN32
bool ExistProgram(String& bin, const char *dir, const char *file)
{
	String win = NormalizePath(GetWindowsDirectory());
	if(FileExists(AppendFileName(win.Mid(0, 3) + dir, file))) {
		bin = win.Mid(0, 3) + dir;
		return true;
	}
	return false;
}

void AutoSetup()
{
	WithAutoSetupLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Ultimate++ methods auto-setup");
	String mw = GetExeDirFile("mingw");
	FindFile ff(mw);
	if(ff && ff.IsDirectory())
		dlg.mingw <<= mw;
	else
		dlg.mingw <<=
			NormalizePathNN(GetWinRegString("Inno Setup: App Path",
		       "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW_is1"));
	if (IsNull(dlg.mingw))
		dlg.mingw <<= NormalizePathNN(GetWinRegString("InstallLocation",
						"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\TDM-GCC" ));
	dlg.mingwmethod <<= "MINGW";
	dlg.domingw <<= !IsNull(dlg.mingw);


	String vs = GetWinRegString("ProductDir", "SOFTWARE\\Microsoft\\VisualStudio\\7.1\\Setup\\VC");
	dlg.visualcpp71 <<=
		NormalizePathNN(
			Nvl(
				GetWinRegString("InstallLocation",
	                            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"
	                            "{362882AE-E40A-4435-B214-6420634C401F}"),
	            vs)
	    );
	dlg.visualcppmethod71 <<= "MSC71";
	dlg.dovisualcpp71 <<= !IsNull(dlg.visualcpp71);

	String sdk8 = GetWinRegString("InstallationFolder",
	                             "Software\\Microsoft\\Microsoft SDKs\\Windows\\v6.0",
	                             HKEY_CURRENT_USER);
	String bin8;
	if(!IsNull(sdk8)) {
		sdk8 = NormalizePath(sdk8);
		dlg.sdk8 <<= sdk8;
		dlg.sdk71 <<= sdk8;
		dlg.visualcpp8 <<= bin8 = sdk8;
	}
	else {
		dlg.visualcpp8 <<= bin8 = NormalizePathNN(
			GetWinRegString("8.0", "SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VS7")
		);
		dlg.sdk8 <<= NormalizePathNN(GetWinRegString("Install Dir", "SOFTWARE\\Microsoft\\MicrosoftSDK\\InstalledSDKs\\8F9E5EF3-A9A5-491B-A889-C58EFFECE8B3"));
	}
	dlg.visualcppmethod8 <<= "MSC8";
	dlg.dovisualcpp8 <<= !IsNull(dlg.visualcpp8);
	if(bin8.GetLength() && FileExists(AppendFileName(bin8, "VC\\Bin\\x64\\cl.exe"))) {
		dlg.dovisualcpp8x64 = true;
	}
	dlg.visualcppmethod8x64 <<= "MSC8x64";
	String sdk9 = NormalizePathNN(GetWinRegString("InstallationFolder",
		                                       "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0",
		                                       HKEY_LOCAL_MACHINE));  
	if(IsNull(sdk9))
		sdk9 = NormalizePathNN(GetWinRegString("InstallationFolder",
	                                           "Software\\Microsoft\\Microsoft SDKs\\Windows\\v6.1",
	                                           HKEY_CURRENT_USER));
	if(IsNull(sdk9))
		sdk9 = NormalizePathNN(GetWinRegString("InstallationFolder",
		                                       "Software\\Microsoft\\Microsoft SDKs\\Windows\\v6.0A",
		                                       HKEY_CURRENT_USER));  
	String bin9;
	if(!IsNull(sdk9) && FileExists(AppendFileName(sdk9, "VC\\Bin\\cl.exe")))
		bin9 = sdk9;
	else
		ExistProgram(bin9, "Program Files (x86)\\Microsoft Visual Studio 9.0", "VC\\Bin\\cl.exe")
		|| ExistProgram(bin9, "Program Files\\Microsoft Visual Studio 9.0", "VC\\Bin\\cl.exe");
	dlg.sdk9 <<= sdk9;
	dlg.visualcpp9 <<= bin9;
	dlg.visualcppmethod9 <<= "MSC9";
	dlg.dovisualcpp9 <<= !IsNull(dlg.visualcpp9);
	String vc9_64 = AppendFileName(bin9, "VC\\Bin\\x64");
	if(!FileExists(AppendFileName(vc9_64, "cl.exe")))
		vc9_64 = AppendFileName(bin9, "VC\\Bin\\amd64");
	if(bin9.GetLength() && FileExists(AppendFileName(vc9_64, "cl.exe")))
		dlg.dovisualcpp9x64 = true;
	dlg.visualcppmethod9x64 <<= "MSC9x64";


	String sdk10 = NormalizePathNN(GetWinRegString("InstallationFolder",
		                                       "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.1",
		                                       HKEY_LOCAL_MACHINE));
	if (IsNull (sdk10))
		sdk10 = NormalizePathNN(GetWinRegString("InstallationFolder",
		                                        "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0A",
		                                        HKEY_LOCAL_MACHINE));
	String bin10;
	if(!IsNull(sdk10) && FileExists(AppendFileName(sdk10, "VC\\Bin\\cl.exe")))
		bin10 = sdk10;
	else
		ExistProgram(bin10, "Program Files (x86)\\Microsoft Visual Studio 10.0", "VC\\Bin\\cl.exe")
		|| ExistProgram(bin10, "Program Files\\Microsoft Visual Studio 10.0", "VC\\Bin\\cl.exe");
	dlg.sdk10 <<= sdk10;
	dlg.visualcpp10 <<= bin10;
	dlg.visualcppmethod10 <<= "MSC10";
	dlg.dovisualcpp10 <<= !IsNull(dlg.visualcpp10);
	String vc10_64 = AppendFileName(bin10, "VC\\Bin\\x64");
	if(!FileExists(AppendFileName(vc10_64, "cl.exe")))
		vc10_64 = AppendFileName(bin10, "VC\\Bin\\amd64");
	if(bin10.GetLength() && FileExists(AppendFileName(vc10_64, "cl.exe")))
		dlg.dovisualcpp10x64 = true;
	dlg.visualcppmethod10x64 <<= "MSC10x64";

	dlg.mysql <<= NormalizePathNN(GetWinRegString("Location", "SOFTWARE\\MySQL AB\\MySQL Server 4.1"));

	String sdl = NormalizePathNN(ConfigFile("sdl"));
	if(DirectoryExists(sdl))
		dlg.sdl <<= sdl;

	Array< FrameRight<Button> > bd;
	DirSel(dlg.mingw, bd.Add());
	DirSel(dlg.visualcpp71, bd.Add());
	DirSel(dlg.visualcpp8, bd.Add());
	DirSel(dlg.visualcpp9, bd.Add());
	DirSel(dlg.visualcpp10, bd.Add());
	DirSel(dlg.sdk71, bd.Add());
	DirSel(dlg.sdk8, bd.Add());
	DirSel(dlg.sdk9, bd.Add());
	DirSel(dlg.sdk10, bd.Add());
	DirSel(dlg.sdl, bd.Add());
	DirSel(dlg.mysql, bd.Add());
	if(dlg.Run() != IDOK)
		return;

	String dir = GetFileFolder(GetExeFilePath());

	String exe;
	String include;
	String lib;

	sdl = ~dlg.sdl;
	if(!IsNull(sdl)) {
		exe << ";" << AppendFileName(sdl, "lib");
		include << ";" << AppendFileName(sdl, "include");
		lib << ";" << AppendFileName(sdl, "lib");
	}

	String mysql = ~dlg.mysql;
	String mlib;
	if(!IsNull(mysql)) {
		exe << ";" << AppendFileName(mysql, "bin");
		include << ";" << AppendFileName(mysql, "include");
		mlib << ";" << AppendFileName(mysql, "bin");
	}

	String mingw = ~dlg.mingw;
	if(!IsNull(mingw) && dlg.domingw) {
		String m = ~dlg.mingwmethod;
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"GCC\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-O0 -gstabs\";\n"
			"RELEASE_BLITZ = \"1\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O3 -ffunction-sections\";\n" // -funroll-loops ?
			"RELEASE_SIZE_OPTIONS = \"-Os -finline-limit=20 -ffunction-sections\";\n"
			"DEBUGGER = \"gdb\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(AppendFileName(~dlg.mingw, "bin") + exe) + ";\n"
			"INCLUDE = " + AsCString(AppendFileName(~dlg.mingw, "include") + include) + ";\n"
			"LIB = " + AsCString(AppendFileName(~dlg.mingw, "lib") + lib + mlib) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	if(!IsNull(mysql)) {
		lib << ";" << AppendFileName(mysql, "lib");
	}

	String vs10 = ~dlg.visualcpp10;
	if(!IsNull(vs10) && dlg.dovisualcpp10x64) {
		String vc = AppendFileName(vs10, "Vc");
		String m = ~dlg.visualcppmethod10x64;
		String sdk = ~dlg.sdk10;
		if(IsNull(sdk))
			sdk = AppendFileName(vc, "PlatformSDK");
		String vc_lib = AppendFileName(vc, "Lib\\x64");
		if(!DirectoryExists(vc_lib))
			vc_lib = AppendFileName(vc, "Lib\\amd64");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC10X64\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"RELEASE_SIZE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs10, "Common7\\Ide") + ';' +
							vc10_64 + ';' +
							AppendFileName(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "Include") + ';' +
							AppendFileName(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							vc_lib + ';' +
							AppendFileName(sdk, "Lib\\x64") +
							lib
			           ) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	if(!IsNull(vs10) && dlg.dovisualcpp10) {
		String vc = AppendFileName(vs10, "Vc");
		String m = ~dlg.visualcppmethod10;
		String sdk = ~dlg.sdk10;
		if(IsNull(sdk))
			sdk = AppendFileName(vc, "PlatformSDK");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC10\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"RELEASE_SIZE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs10, "Common7\\Ide") + ';' +
							AppendFileName(vc, "Bin") + ';' +
							AppendFileName(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "Include") + ';' +
							AppendFileName(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							AppendFileName(vc, "Lib") + ';' +
							AppendFileName(sdk, "Lib") +
							lib
			           ) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	String vs9 = ~dlg.visualcpp9;
	if(!IsNull(vs9) && dlg.dovisualcpp9x64) {
		String vc = AppendFileName(vs9, "Vc");
		String m = ~dlg.visualcppmethod9x64;
		String sdk = ~dlg.sdk9;
		if(IsNull(sdk))
			sdk = AppendFileName(vc, "PlatformSDK");
		String vc_lib = AppendFileName(vc, "Lib\\x64");
		if(!DirectoryExists(vc_lib))
			vc_lib = AppendFileName(vc, "Lib\\amd64");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC9X64\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"RELEASE_SIZE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs9, "Common7\\Ide") + ';' +
							vc9_64 + ';' +
							AppendFileName(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "Include") + ';' +
							AppendFileName(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							vc_lib + ';' +
							AppendFileName(sdk, "Lib\\x64") +
							lib
			           ) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	if(!IsNull(vs9) && dlg.dovisualcpp9) {
		String vc = AppendFileName(vs9, "Vc");
		String m = ~dlg.visualcppmethod9;
		String sdk = ~dlg.sdk9;
		if(IsNull(sdk))
			sdk = AppendFileName(vc, "PlatformSDK");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC9\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"RELEASE_SIZE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs9, "Common7\\Ide") + ';' +
							AppendFileName(vc, "Bin") + ';' +
							AppendFileName(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "Include") + ';' +
							AppendFileName(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							AppendFileName(vc, "Lib") + ';' +
							AppendFileName(sdk, "Lib") +
							lib
			           ) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	String vs8 = ~dlg.visualcpp8;
	if(!IsNull(vs8) && dlg.dovisualcpp8x64) {
		String vc = AppendFileName(vs8, "Vc");
		String m = ~dlg.visualcppmethod8x64;
		String sdk = ~dlg.sdk8;
		if(IsNull(sdk))
			sdk = AppendFileName(vc, "PlatformSDK");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC8X64\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"RELEASE_SIZE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs8, "Common7\\Ide") + ';' +
							AppendFileName(vc, "Bin\\x64") + ';' +
							AppendFileName(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "Include") + ';' +
							AppendFileName(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							AppendFileName(vc, "Lib\\x64") + ';' +
							AppendFileName(sdk, "Lib\\x64") +
							lib
			           ) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	if(!IsNull(vs8) && dlg.dovisualcpp8) {
		String vc = AppendFileName(vs8, "Vc");
		String m = ~dlg.visualcppmethod8;
		String sdk = ~dlg.sdk8;
		if(IsNull(sdk8))
			sdk = AppendFileName(vc, "PlatformSDK");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC8\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"RELEASE_SIZE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs8, "Common7\\Ide") + ';' +
							AppendFileName(vc, "Bin") + ';' +
							AppendFileName(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "Include") + ';' +
							AppendFileName(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							AppendFileName(vc, "Lib") + ';' +
							AppendFileName(sdk, "Lib") +
							lib
			           ) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	String vc7 = ~dlg.visualcpp71;
	if(PathIsEqual(vc7, vs))
		vs = AppendFileName(GetFileFolder(NormalizePath(vs)), "Common7\\ide") + ";";
	else
		vs.Clear();
	if(!IsNull(vc7) && dlg.dovisualcpp71) {
		String m = ~dlg.visualcppmethod71;
		String sdk = ~dlg.sdk71;
		if(IsNull(sdk))
			sdk = AppendFileName(vc7, "PlatformSDK");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC71\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2\";\n"
			"RELEASE_SIZE_OPTIONS = \"-O1\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                vs +
							AppendFileName(vc7, "Bin") + ';' +
							AppendFileName(sdk, "Bin") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc7, "Include") + ';' +
							AppendFileName(sdk, "Include") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							AppendFileName(vc7, "Lib") + ';' +
							AppendFileName(sdk, "Lib") +
							lib
			           ) + ";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}
}
#endif

#ifdef PLATFORM_X11

void AutoSetup() {}

#endif

void Ide::AutoSetup()
{
	::AutoSetup();
	SyncBuildMode();
	SetBar();
}
