#include <Installer/install.h>

void FileError()
{
	Panic("Input file is corrupted !");
}

String GetString(StringStream& ss)
{
	int len = ss.GetL();
	String::Buffer b(len);
	if(ss.Get(b, len) != len)
		FileError();
	return b;
}

void SaveFiles(StringStream& ss, const char *dir, String path, Gate2<int, int> progress)
{
	String p1 = AppendFileName(dir, path);
	RealizeDirectory(p1);
	progress(ss.GetPos(), ss.GetSize());
	for(;;)
		switch(ss.Get()) {
		case 0:
			SaveFiles(ss, dir, AppendFileName(path, GetString(ss)), progress);
			break;
		case 1: {
			String name = GetString(ss);
			String file = GetString(ss);
			SaveFile(AppendFileName(p1, name), file);
			break;
		}
		case 2:
			return;
		default:
			FileError();
		}
}

void SaveFiles(const String& archive, const char *dir)
{
	Progress pi;
	pi.SetText("Decompressing files");
	String data = BZ2Decompress(archive, pi);
	pi.SetText("Storing files");
	StringStream ss(data);
	SaveFiles(ss, dir, "", pi);
}

String GetRCDATA(int n)
{
	HRSRC hrsrc = FindResource(NULL, LPCTSTR(n), RT_RCDATA);
	if(!hrsrc) return Null;
	return String((const char *)LockResource(LoadResource(NULL, hrsrc)),
	              SizeofResource(NULL, hrsrc));
}

VectorMap<String, String>& DocBase()
{
	static VectorMap<String, String> s;
	return s;
}

#undef  TEXT
#define INCLUDE_NAMESPACE_AppRes
#define ITEM(a, b, c, d) DocBase().GetAdd(String(a) + '/' + String(b) + '/' + String(c)) =
#define EXTERNAL
#define TEXT(x) x
#define TXT(x) x
#define END_ITEM ;

INITBLOCK
{
#include <Core/doc.dpp/dir.dph>
}

#undef INCLUDE_NAMESPACE_AppRes
#undef ITEM
#undef EXTERNAL
#undef TEXT
#undef TXT
#undef END_ITEM


GUI_APP_MAIN
{
	WithLicenseLayout<TopWindow> w;
	CtrlLayoutOKCancel(w, "License agreement");
	w.license = DocBase().Get("AppRes/AppRes/GPL");
	if(w.Execute() != IDOK)
		return;

	WithInstallLayout<TopWindow> d;
	CtrlLayoutOKCancel(d, "Ultimate++ / mingw 0.1.8 installer");
	d.path = "C:\\upp";
	d.myapps = "C:\\MyApps";
	d.icon = true;
	String path, exe;
	for(;;) {
		if(d.Execute() != IDOK)
			return;
		path = ~d.path;
		exe = AppendFileName(path, "theide.exe");
		if(!FileExists(exe))
			break;
		Exclamation("Previous version found in [* " + DeQtf((String)~d.path) + "].&" +
		            "Please uninstall it or choose another directory.");
	}
	SaveFiles(GetRCDATA(1112), path);
	if(d.icon)
		InstallDesktopIcon(exe, "TheIde.lnk", "Ultimate++ IDE");
	InstallProgramGroup(exe, NULL, "Ultimate++ IDE.lnk");
	InstallUninstall(exe, "Ultimate++");
	SaveFile(AppendFileName(path, "uppsrc.var"),
		"UPP = " + AsCString(AppendFileName(path, "uppsrc")) + ";\r\n"
		"COMMON = " + AsCString(AppendFileName(path, "Common")) + ";\r\n"
		"OUTPUT = " + AsCString(AppendFileName(path, "out")) + ";\r\n"
	);
	SaveFile(AppendFileName(path, "examples.var"),
		"UPP = " + AsCString(AppendFileName(path, "examples") + ';'
		                   + AppendFileName(path, "uppsrc")) + ";\r\n"
		"COMMON = " + AsCString(AppendFileName(path, "Common")) + ";\r\n"
		"OUTPUT = " + AsCString(AppendFileName(path, "out")) + ";\r\n"
	);
	SaveFile(AppendFileName(path, "reference.var"),
		"UPP = " + AsCString(AppendFileName(path, "reference") + ';'
		                   + AppendFileName(path, "uppsrc")) + ";\r\n"
		"COMMON = " + AsCString(AppendFileName(path, "Common")) + ";\r\n"
		"OUTPUT = " + AsCString(AppendFileName(path, "out")) + ";\r\n"
	);
	SaveFile(AppendFileName(path, "MyApps.var"),
		"UPP = " + AsCString(String(~d.myapps) + ';'
		                   + AppendFileName(path, "uppsrc")) + ";\r\n"
		"COMMON = " + AsCString(AppendFileName(path, "Common")) + ";\r\n"
		"OUTPUT = " + AsCString(AppendFileName(path, "out")) + ";\r\n"
	);
	SaveFile(
		AppendFileName(path, "MINGW.bm"),
		"BUILDER = \"GCC\";\n"
		"DEBUG_INFO = \"2\";\n"
		"DEBUG_BLITZ = \"1\";\n"
		"DEBUG_LINKMODE = \"2\";\n"
		"DEBUG_OPTIONS = \"-O0\";\n"
		"RELEASE_BLITZ = \"1\";\n"
		"RELEASE_LINKMODE = \"0\";\n"
		"RELEASE_OPTIONS = \"-O1 -ffunction-sections\";\n"
		"DEBUGGER = \"gdb\";\n"
		"REMOTE_HOST = \"\";\n"
		"REMOTE_OS = \"\";\n"
		"REMOTE_TRANSFER = \"\";\n"
		"REMOTE_MAP = \"\";\n"
		"PATH = " + AsCString(AppendFileName(path, "mingw/bin")) + ";\n"
		"INCLUDE = " + AsCString(AppendFileName(path, "mingw/include")) + ";\n"
		"LIB = " + AsCString(AppendFileName(path, "mingw/lib")) + ";\n"
	);
	SaveFile(AppendFileName(path, "default_method"), "MINGW");
	String vc7 = GetWinRegString("ProductDir",
	                             "SOFTWARE\\Microsoft\\VisualStudio\\7.1\\Setup\\VC");
	if(!IsNull(vc7)) {
		String pdir = GetWinRegString("VS7CommonDir",
		                              "SOFTWARE\\Microsoft\\VisualStudio\\7.1\\Setup\\VS");
		SaveFile(
			AppendFileName(path, "MSC71.bm"),
			"BUILDER = \"MSC71\";\n"
			"DEBUG_INFO = \"2\";\n"
			"DEBUG_BLITZ = \"1\";\n"
			"DEBUG_LINKMODE = \"0\";\n"
			"DEBUG_OPTIONS = \"\";\n"
			"RELEASE_BLITZ = \"0\";\n"
			"RELEASE_LINKMODE = \"0\";\n"
			"RELEASE_OPTIONS = \"-O1\";\n"
			"DEBUGGER = \"\";\n"
			"REMOTE_HOST = \"\";\n"
			"REMOTE_OS = \"\";\n"
			"REMOTE_TRANSFER = \"\";\n"
			"REMOTE_MAP = \"\";\n"
			"PATH = " + AsCString(
							AppendFileName(pdir, "Ide") + ';' +
							AppendFileName(vc7, "Bin") + ';' +
							AppendFileName(pdir, "Tools") + ';' +
							AppendFileName(pdir, "Tools\\bin")
						) + ";\n"
			"INCLUDE = " + AsCString(
							AppendFileName(vc7, "Include") + ';' +
							AppendFileName(vc7, "PlatformSDK\\Include")
						   ) + ";\n"
			"LIB = " + AsCString(
							AppendFileName(vc7, "Lib") + ';' +
							AppendFileName(vc7, "PlatformSDK\\Lib")
			           ) + ";\n"
		);
	}
	PromptOK("Installation successful.");
}
