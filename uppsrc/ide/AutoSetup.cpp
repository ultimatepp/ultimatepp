#include "ide.h"

// DEPRECATED

extern FileSel& sSD();

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

class CtrlList : public ArrayCtrl {
public:
	typedef CtrlList CLASSNAME;
	CtrlList() { AutoHideSb().NoGrid().NoHeader(); AddColumn(String()); }

	template <class T>
	CtrlList& AddLayout(T& ctrl) {
		Add();
		int index = GetCount() - 1;
		DisableLine(index);
		SetCtrl(index, 0, ctrl.SizePos());
		SetLineCy(index, ctrl.GetLayoutSize().cy);
		AddSeparator();
		return *this;
	}
};

void AutoSetup()
{
	WithAutoSetupLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Ultimate++ methods auto-setup");
	dlg.Sizeable().Zoomable().SetRect(Size(640, 480));

	WithAutoSetupMSC12<ParentCtrl> msc12; CtrlLayout(msc12);
	WithAutoSetupMSC11<ParentCtrl> msc11; CtrlLayout(msc11);
	WithAutoSetupMSC10<ParentCtrl> msc10; CtrlLayout(msc10);
	WithAutoSetupMSC9<ParentCtrl> msc9; CtrlLayout(msc9);
	WithAutoSetupMSC8<ParentCtrl> msc8; CtrlLayout(msc8);
	WithAutoSetupMSC7_1<ParentCtrl> msc7_1; CtrlLayout(msc7_1);
	WithAutoSetupMinGW<ParentCtrl> mingw; CtrlLayout(mingw);
	WithAutoSetupOWC<ParentCtrl> owc; CtrlLayout(owc);
	WithAutoSetupOther<ParentCtrl> other; CtrlLayout(other);

	CtrlList msclist, mingwlist, owclist, otherlist;

	String dir = GetFileFolder(GetExeFilePath());

	msclist.AddLayout(msc12)
		.AddLayout(msc11)
		.AddLayout(msc10)
		.AddLayout(msc9)
		.AddLayout(msc8)
		.AddLayout(msc7_1);

	mingwlist.AddLayout(mingw);
	owclist.AddLayout(owc);
	otherlist.AddLayout(other);

	dlg.tab.Add(msclist.SizePos(), "Visual C++");
	dlg.tab.Add(mingwlist.SizePos(), "MinGW GCC");
	dlg.tab.Add(owclist.SizePos(), "Open Watcom");
	dlg.tab.Add(otherlist.SizePos(), "Other");

	String mw = GetExeDirFile("mingw");
	FindFile ff(mw);
	if(ff && ff.IsDirectory())
		mingw.dir <<= mw;
	else
		mingw.dir <<=
			NormalizePathNN(GetWinRegString("Inno Setup: App Path",
		       "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MinGW_is1"));
	if (IsNull(mingw.dir))
		mingw.dir <<= NormalizePathNN(GetWinRegString("InstallLocation",
						"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\TDM-GCC" ));
	mingw.method <<= "MINGW";
	mingw.create <<= !IsNull(mingw.dir) && !FileExists(AppendFileName(dir, "MINGW.bm"));

	String owcdir = GetEnv("WATCOM");
	if(owcdir.IsEmpty())
		owcdir = "C:\\WATCOM";

	if(DirectoryExists(owcdir))
		owc.dir <<= owcdir;

	owc.method <<= "OWC";
	owc.create <<= !IsNull(owc.dir)  && !FileExists(AppendFileName(dir, "OWC.bm"));


	String vs = GetWinRegString("ProductDir", "SOFTWARE\\Microsoft\\VisualStudio\\7.1\\Setup\\VC");
	msc7_1.dir <<=
		NormalizePathNN(
			Nvl(
				GetWinRegString("InstallLocation",
	                            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"
	                            "{362882AE-E40A-4435-B214-6420634C401F}"),
	            vs)
	    );
	msc7_1.method <<= "MSC71";
	msc7_1.create <<= !IsNull(msc7_1.dir) && !FileExists(AppendFileName(dir, "MSC71.bm"));

	String sdk8 = GetWinRegString("InstallationFolder",
	                             "Software\\Microsoft\\Microsoft SDKs\\Windows\\v6.0",
	                             HKEY_CURRENT_USER);
	String bin8;
	if(!IsNull(sdk8)) {
		sdk8 = NormalizePath(sdk8);
		msc8.sdk <<= sdk8;
		msc7_1.sdk <<= sdk8;
		msc8.dir <<= bin8 = sdk8;
	}
	else {
		msc8.dir <<= bin8 = NormalizePathNN(
			GetWinRegString("8.0", "SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VS7")
		);
		msc8.sdk <<= NormalizePathNN(GetWinRegString("Install Dir", "SOFTWARE\\Microsoft\\MicrosoftSDK\\InstalledSDKs\\8F9E5EF3-A9A5-491B-A889-C58EFFECE8B3"));
	}
	msc8.method <<= "MSC8";
	msc8.create <<= !IsNull(msc8.dir) && !FileExists(AppendFileName(dir, "MSC8.bm"));
	if(bin8.GetLength() && FileExists(AppendFileName(bin8, "VC\\Bin\\x64\\cl.exe"))
	   && !FileExists(AppendFileName(dir, "MSC8x64.bm"))) {
		msc8.create64 = true;
	}
	msc8.method64 <<= "MSC8x64";
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
	msc9.sdk <<= sdk9;
	msc9.dir <<= bin9;
	msc9.method <<= "MSC9";
	msc9.create <<= !IsNull(msc9.dir) && !FileExists(AppendFileName(dir, "MSC9.bm"));
	String vc9_64 = AppendFileName(bin9, "VC\\Bin\\x64");
	if(!FileExists(AppendFileName(vc9_64, "cl.exe")))
		vc9_64 = AppendFileName(bin9, "VC\\Bin\\amd64");
	if(bin9.GetLength() && FileExists(AppendFileName(vc9_64, "cl.exe"))
	   && !FileExists(AppendFileName(dir, "MSC9x64.bm")))
		msc9.create64 = true;
	msc9.method64 <<= "MSC9x64";


	String sdk10 = NormalizePathNN(GetWinRegString("InstallationFolder",
		                                       "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.1",
		                                       HKEY_LOCAL_MACHINE));
	if (IsNull(sdk10))
		sdk10 = NormalizePathNN(GetWinRegString("InstallationFolder",
		                                        "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0A",
		                                        HKEY_LOCAL_MACHINE));
	String bin10;
	if(!IsNull(sdk10) && FileExists(AppendFileName(sdk10, "VC\\Bin\\cl.exe")))
		bin10 = sdk10;
	else
		ExistProgram(bin10, "Program Files (x86)\\Microsoft Visual Studio 10.0", "VC\\Bin\\cl.exe")
		|| ExistProgram(bin10, "Program Files\\Microsoft Visual Studio 10.0", "VC\\Bin\\cl.exe");
	msc10.sdk <<= sdk10;
	msc10.dir <<= bin10;
	msc10.method <<= "MSC10";
	msc10.create <<= !IsNull(msc10.dir) && !FileExists(AppendFileName(dir, "MSC10.bm"));
	String vc10_64 = AppendFileName(bin10, "VC\\Bin\\x64");
	if(!FileExists(AppendFileName(vc10_64, "cl.exe")))
		vc10_64 = AppendFileName(bin10, "VC\\Bin\\amd64");
	if(bin10.GetLength() && FileExists(AppendFileName(vc10_64, "cl.exe"))
	   && !FileExists(AppendFileName(dir, "MSC10x64.bm")))
		msc10.create64 = true;
	msc10.method64 <<= "MSC10x64";


	String sdk11 = NormalizePathNN(GetWinRegString("InstallationFolder",
		                                       "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.0",
		                                       HKEY_LOCAL_MACHINE));
	String bin11;
	if(!IsNull(sdk11) && FileExists(AppendFileName(sdk11, "VC\\bin\\cl.exe")))
		bin11 = sdk11;
	else
		ExistProgram(bin11, "Program Files (x86)\\Microsoft Visual Studio 11.0", "VC\\bin\\cl.exe")
		|| ExistProgram(bin11, "Program Files\\Microsoft Visual Studio 11.0", "VC\\bin\\cl.exe");
	msc11.sdk <<= sdk11;
	msc11.dir <<= bin11;
	msc11.method <<= "MSC11";
	msc11.create <<= !IsNull(msc11.dir) && !FileExists(AppendFileName(dir, "MSC11.bm"));
	String vc11_64 = AppendFileName(bin11, "VC\\bin\\x64");
	if(!FileExists(AppendFileName(vc11_64, "cl.exe")))
		vc11_64 = AppendFileName(bin11, "VC\\bin\\x86_amd64");
	if(bin11.GetLength() && FileExists(AppendFileName(vc11_64, "cl.exe"))
	   && !FileExists(AppendFileName(dir, "MSC11x64.bm")))
		msc11.create64 = true;
	msc11.method64 <<= "MSC11x64";


	String sdk12 = NormalizePathNN(GetWinRegString("InstallationFolder",
		                                       "SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.1",
		                                       HKEY_LOCAL_MACHINE));
	String bin12;
	if(!IsNull(sdk12) && FileExists(AppendFileName(sdk12, "VC\\bin\\cl.exe")))
		bin12 = sdk12;
	else
		ExistProgram(bin12, "Program Files (x86)\\Microsoft Visual Studio 12.0", "VC\\bin\\cl.exe")
		|| ExistProgram(bin12, "Program Files\\Microsoft Visual Studio 12.0", "VC\\bin\\cl.exe");
	msc12.sdk <<= sdk12;
	msc12.dir <<= bin12;
	msc12.method <<= "MSC12";
	msc12.create <<= !IsNull(msc12.dir) && !FileExists(AppendFileName(dir, "MSC12.bm"));
	String vc12_64 = AppendFileName(bin12, "VC\\bin\\x64");
	if(!FileExists(AppendFileName(vc12_64, "cl.exe")))
		vc12_64 = AppendFileName(bin12, "VC\\bin\\x86_amd64");
	if(bin12.GetLength() && FileExists(AppendFileName(vc12_64, "cl.exe"))
	   && !FileExists(AppendFileName(dir, "MSC12x64.bm")))
		msc12.create64 = true;
	msc12.method64 <<= "MSC12x64";


	other.mysql <<= NormalizePathNN(GetWinRegString("Location", "SOFTWARE\\MySQL AB\\MySQL Server 4.1"));

	String sdl = NormalizePathNN(ConfigFile("sdl"));
	if(DirectoryExists(sdl))
		other.sdl <<= sdl;

	Array< FrameRight<Button> > bd;
	DirSel(mingw.dir, bd.Add());
	DirSel(owc.dir, bd.Add());
	DirSel(msc7_1.dir, bd.Add());
	DirSel(msc8.dir, bd.Add());
	DirSel(msc9.dir, bd.Add());
	DirSel(msc10.dir, bd.Add());
	DirSel(msc11.dir, bd.Add());
	DirSel(msc12.dir, bd.Add());
	DirSel(msc7_1.sdk, bd.Add());
	DirSel(msc8.sdk, bd.Add());
	DirSel(msc9.sdk, bd.Add());
	DirSel(msc10.sdk, bd.Add());
	DirSel(msc11.sdk, bd.Add());
	DirSel(msc12.sdk, bd.Add());
	DirSel(other.sdl, bd.Add());
	DirSel(other.mysql, bd.Add());
	if(dlg.Run() != IDOK)
		return;

	String exe;
	String include;
	String lib;

	sdl = ~other.sdl;
	if(!IsNull(sdl)) {
		exe << ";" << AppendFileName(sdl, "lib");
		include << ";" << AppendFileName(sdl, "include");
		lib << ";" << AppendFileName(sdl, "lib");
	}

	String mysql = ~other.mysql;
	String mlib;
	if(!IsNull(mysql)) {
		exe << ";" << AppendFileName(mysql, "bin");
		include << ";" << AppendFileName(mysql, "include");
		mlib << ";" << AppendFileName(mysql, "bin");
	}

	String mingwdir = ~mingw.dir;
	if(!IsNull(mingwdir) && mingw.create) {
		String m = ~mingw.method;
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
			"DEBUGGER = \"gdb\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(AppendFileName(mingwdir, "bin") + exe) + ";\n"
			"INCLUDE = " + AsCString(AppendFileName(mingwdir, "include") + include) + ";\n"
			"LIB = " + AsCString(AppendFileName(mingwdir, "lib") + lib + mlib) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"-msse2\";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	owcdir = ~owc.dir;
	if(!IsNull(owcdir) && owc.create) {
		String m = ~owc.method;
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"OWC\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-d2\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-ot\";\n"
			"DEBUGGER = \"gdb\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
				AppendFileName(owcdir, "binnt") + ';' +
				AppendFileName(owcdir, "binw") +
				exe) + ";\n"
			"INCLUDE = " + AsCString(AppendFileName(owcdir, "h") + ';' +
				AppendFileName(owcdir, "h\\nt") +
				include) + ";\n"
			"LIB = " + AsCString(AppendFileName(owcdir, "lib386") + ';' +
				AppendFileName(owcdir, "lib386\\nt") +
				lib + mlib) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	if(!IsNull(mysql)) {
		lib << ";" << AppendFileName(mysql, "lib");
	}

	String vs12 = ~msc12.dir;
	if(!IsNull(vs12) && msc12.create64) {
		String vc = AppendFileName(vs12, "VC");
		String m = ~msc12.method64;
		String sdk = ~msc12.sdk;
		if(IsNull(sdk))
			sdk = AppendFileName(vc, "PlatformSDK");
		String vc_lib = AppendFileName(vc, "lib\\x64");
		if(!DirectoryExists(vc_lib))
			vc_lib = AppendFileName(vc, "lib\\amd64");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC12X64\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs12, "Common7\\IDE") + ';' +
							vc12_64 + ';' +
							AppendFileName(sdk, "bin\\x64") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "include") + ';' +
							AppendFileName(sdk, "Include\\um") + ';' +
							AppendFileName(sdk, "Include\\shared") + ';' +
							AppendFileName(sdk, "Include\\winrt") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							vc_lib + ';' +
							AppendFileName(sdk, "Lib\\winv6.3\\um\\x64") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/Zm200\";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	if(!IsNull(vs12) && msc12.create) {
		String vc = AppendFileName(vs12, "VC");
		String m = ~msc12.method;
		String sdk = ~msc12.sdk;
		if(IsNull(sdk))
			sdk = AppendFileName(vc, "PlatformSDK");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC12\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs12, "Common7\\IDE") + ';' +
							AppendFileName(vc, "bin") + ';' +
							AppendFileName(sdk, "bin\\x86") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "include") + ';' +
							AppendFileName(sdk, "Include\\um") + ';' +
							AppendFileName(sdk, "Include\\shared") + ';' +
							AppendFileName(sdk, "Include\\winrt") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							AppendFileName(vc, "lib") + ';' +
							AppendFileName(sdk, "Lib\\winv6.3\\um\\x86") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/arch:SSE2 /Zm200\";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	String vs11 = ~msc11.dir;
	if(!IsNull(vs11) && msc11.create64) {
		String vc = AppendFileName(vs11, "VC");
		String m = ~msc11.method64;
		String sdk = ~msc11.sdk;
		if(IsNull(sdk))
			sdk = AppendFileName(vc, "PlatformSDK");
		String vc_lib = AppendFileName(vc, "Lib\\x64");
		if(!DirectoryExists(vc_lib))
			vc_lib = AppendFileName(vc, "Lib\\amd64");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC11X64\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs11, "Common7\\Ide") + ';' +
							vc11_64 + ';' +
							AppendFileName(sdk, "bin\\x64") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "Include") + ';' +
							AppendFileName(sdk, "Include\\um") + ';' +
							AppendFileName(sdk, "Include\\shared") + ';' +
							AppendFileName(sdk, "Include\\winrt") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							vc_lib + ';' +
							AppendFileName(sdk, "Lib\\win8\\um\\x64") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/Zm200\";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	if(!IsNull(vs11) && msc11.create) {
		String vc = AppendFileName(vs11, "VC");
		String m = ~msc11.method;
		String sdk = ~msc11.sdk;
		if(IsNull(sdk))
			sdk = AppendFileName(vc, "PlatformSDK");
		SaveFile(
			AppendFileName(dir, m + ".bm"),
			"BUILDER = \"MSC11\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"-Od\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O2 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
			"PATH = " + AsCString(
			                AppendFileName(vs11, "Common7\\Ide") + ';' +
							AppendFileName(vc, "bin") + ';' +
							AppendFileName(sdk, "bin\\x86") +
							exe
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc, "Include") + ';' +
							AppendFileName(sdk, "Include\\um") + ';' +
							AppendFileName(sdk, "Include\\shared") + ';' +
							AppendFileName(sdk, "Include\\winrt") +
							include
						   ) + ";\n"
			"LIB = " + AsCString(
							AppendFileName(vc, "Lib") + ';' +
							AppendFileName(sdk, "Lib\\win8\\um\\x86") +
							lib
			           ) + ";\n"
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/arch:SSE2 /Zm200\";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	String vs10 = ~msc10.dir;
	if(!IsNull(vs10) && msc10.create64) {
		String vc = AppendFileName(vs10, "Vc");
		String m = ~msc10.method64;
		String sdk = ~msc10.sdk;
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
			"RELEASE_OPTIONS = \"-O1 -GS-\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
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
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/Zm200\";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	if(!IsNull(vs10) && msc10.create) {
		String vc = AppendFileName(vs10, "Vc");
		String m = ~msc10.method;
		String sdk = ~msc10.sdk;
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
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
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
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/arch:SSE2 /Zm200\";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	String vs9 = ~msc9.dir;
	if(!IsNull(vs9) && msc9.create64) {
		String vc = AppendFileName(vs9, "Vc");
		String m = ~msc9.method64;
		String sdk = ~msc9.sdk;
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
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
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
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/Zm200\";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	if(!IsNull(vs9) && msc9.create) {
		String vc = AppendFileName(vs9, "Vc");
		String m = ~msc9.method;
		String sdk = ~msc9.sdk;
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
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
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
			"ALLOW_PRECOMPILED_HEADERS = \"1\";\n"
			"COMMON_OPTIONS = \"/arch:SSE2 /Zm200\";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	String vs8 = ~msc8.dir;
	if(!IsNull(vs8) && msc8.create64) {
		String vc = AppendFileName(vs8, "Vc");
		String m = ~msc8.method64;
		String sdk = ~msc8.sdk;
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
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
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

	if(!IsNull(vs8) && msc8.create) {
		String vc = AppendFileName(vs8, "Vc");
		String m = ~msc8.method;
		String sdk = ~msc8.sdk;
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
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
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
			"COMMON_OPTIONS = \"/arch:SSE2 /Zm200\";\n"
		);
		SaveFile(AppendFileName(dir, "default_method"), m);
	}

	String vc7 = ~msc7_1.dir;
	if(PathIsEqual(vc7, vs))
		vs = AppendFileName(GetFileFolder(NormalizePath(vs)), "Common7\\ide") + ";";
	else
		vs.Clear();
	if(!IsNull(vc7) && msc7_1.create) {
		String m = ~msc7_1.method;
		String sdk = ~msc7_1.sdk;
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
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"DEBUG_LINK = \"/STACK:20000000\";\n"
			"RELEASE_LINK = \"/STACK:20000000\";\n"
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

void Ide::AutoSetup()
{
	::AutoSetup();
	SyncBuildMode();
	SetBar();
}

#endif

