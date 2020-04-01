#include "ide.h"
//#include "Install.h"

#ifndef PLATFORM_WIN32

bool Install(bool& hasvars)
{
	String ass = GetConfigFolder();

	String myapps = DirectoryExists(GetExeDirFile("uppsrc")) ? GetExeDirFile("MyApps") : GetHomeDirFile("MyApps");

	String uppsrc;
	
	String out;
	String p = GetExeFolder();
	while(p.GetCount() > 1 && DirectoryExists(p)) {
		String h = AppendFileName(p, ".cache");
		if(DirectoryExists(h)) {
			out = h;
			break;
		}
		p = GetFileFolder(p);
	}
	
	out = Nvl(out, GetHomeDirFile(".cache")) + "/upp.out";
	
	RealizeDirectory(out);

	auto MakeAssembly = [&](String b, String name = Null) {
		name = Nvl(name, GetFileTitle(b));
		String a = ass + '/' + name + ".var";
		if(name == "uppsrc")
			uppsrc = Nvl(uppsrc, b);
		else {
			if(uppsrc.GetCount() == 0)
				uppsrc = FileExists(GetHomeDirFile("upp.src/uppsrc/ide/ide.upp"))
				         ? GetHomeDirFile("upp.src/uppsrc") : GetHomeDirFile("uppsrc");
			b << ';' << uppsrc;
		}
		if(!FileExists(a))
			SaveFile(a,
				"UPP = " + AsCString(b) + ";\r\n"
				"OUTPUT = " + AsCString(out) + ";\r\n"
			);
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
	if(!hasvars) {
		Scan(GetFileFolder(GetFileFolder(GetExeFilePath())) + "/SharedSupport/uppsrc");
		Scan(GetFileFolder(GetFileFolder(GetExeFilePath())) + "/SharedSupport/*");
	}
#endif

	Scan(GetExeFolder() + "/uppsrc");
	Scan(GetExeFolder() + "/*");
	Scan(GetHomeDirFile("upp.src/uppsrc"));
	Scan(GetHomeDirFile("upp.src/*"));
	Scan(GetHomeDirFile("upp/uppsrc"));
	Scan(GetHomeDirFile("upp/*"));
	Scan(GetHomeDirFile("*"));
	for(FindFile ff(GetHomeDirFile("*")); ff; ff.Next())
		if(ff.IsFolder())
			Scan(ff.GetPath() + "/*");
	
	MakeAssembly(myapps);
	uppsrc = GetHomeDirFile("bazaar") + ';' + uppsrc;
	MakeAssembly(myapps, "MyApps-bazaar");
	CreateBuildMethods();
	return true;
}

#endif
