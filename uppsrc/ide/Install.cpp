#include "ide.h"

bool CheckLicense()
{
#ifdef PLATFORM_MACOS
	String chkfile = GetFileFolder(GetAppFolder()) + "/license.chk";
#else
	String chkfile = GetExeDirFile("license.chk");
#endif
	if(!FileExists(chkfile))
		return true;
	ShowSplash();
	Ctrl::ProcessEvents();
	Sleep(2000);
	HideSplash();
	Ctrl::ProcessEvents();
	WithLicenseLayout<TopWindow> d;
	CtrlLayoutOKCancel(d, "License agreement");
	d.license = GetTopic("ide/app/BSD_en-us").text;
	d.license.Margins(4);
	d.license.SetZoom(Zoom(Zy(18), 100));
	d.ActiveFocus(d.license);
	if(d.Run() != IDOK)
		return false;
	DeleteFile(chkfile);
	return true;
}

#ifdef PLATFORM_POSIX

bool Install(bool& hasvars)
{
	String ass = GetConfigFolder();

	String uppsrc, bazaar;
	
	String out = GetDefaultUppOut();
	
	int pass = 0;

	auto MakeAssembly = [&](String b) {
		String name = GetFileTitle(b);
		String a = ass + '/' + name + ".var";
		if(pass == 0) {
			if(name == "uppsrc" && IsNull(uppsrc))
				uppsrc = b;
		}
		else {
			if(name != "uppsrc")
				b << ';' << uppsrc;
			if(!FileExists(a))
				SaveFile(a,
					"UPP = " + AsCString(b) + ";\r\n"
					"OUTPUT = " + AsCString(out) + ";\r\n"
				);
		}
	};
	
	auto Scan = [&](String p) {
		for(FindFile ff(p); ff; ff.Next())
			if(ff.IsFolder()) {
				String path = ff.GetPath();
				for(FindFile ff2(path + "/*"); ff2; ff2.Next()) {
					String p = ff2.GetPath();
					String upp = LoadFile(p + '/' + GetFileTitle(p) + ".upp");
					if(upp.Find("mainconfig") >= 0) {
						MakeAssembly(path);
						hasvars = true;
						break;
					}
				}
			}
	};

#ifdef PLATFORM_COCOA
	String idir = GetFileFolder(GetAppFolder());
	String myapps = DirectoryExists(idir + "/uppsrc") ? idir + "/MyApps"  : GetHomeDirFile("MyApps");
#else
	String myapps = (DirectoryExists(GetExeDirFile("uppsrc")) ? GetExeDirFile  : GetHomeDirFile)("MyApps");
#endif

	for(pass = 0; pass < 2; pass++) {
		if(pass)
			MakeAssembly(myapps);
	#ifdef PLATFORM_COCOA
		Scan(idir + "/uppsrc");
		Scan(idir + "/*");
	#endif
	#ifdef FLATPAK
		Scan(GetHomeDirFile("/.local/src/upp/uppsrc"));
		Scan(GetHomeDirFile("/.local/src/upp/*"));
	#endif
		Scan(GetExeFolder() + "/uppsrc");
		Scan(GetExeFolder() + "/*");
		Scan(GetHomeDirFile("upp.src/uppsrc"));
		Scan(GetHomeDirFile("upp.src/*"));
		Scan(GetHomeDirFile("upp/uppsrc"));
		Scan(GetHomeDirFile("upp/*"));
	#ifndef PLATFORM_COCOA // otherwise macos complains about Documents/Music etc...
		Scan(GetHomeDirFile("*"));
		for(FindFile ff(GetHomeDirFile("*")); ff; ff.Next())
			if(ff.IsFolder())
				Scan(ff.GetPath() + "/*");
	#endif
	}

	CreateBuildMethods();
	return true;
}

#endif
