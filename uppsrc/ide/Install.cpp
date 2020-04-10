#include "ide.h"
//#include "Install.h"

#ifdef PLATFORM_POSIX

bool Install(bool& hasvars)
{
	String ass = GetConfigFolder();

	String uppsrc, bazaar;
	
	String out = GetDefaultUppOut();
	
	int pass = 0;

	auto MakeAssembly = [&](String b, String name = Null) {
		name = Nvl(name, GetFileTitle(b));
		String a = ass + '/' + name + ".var";
		if(pass == 0) {
			if(name == "uppsrc" && IsNull(uppsrc))
				uppsrc = b;
			if(name == "bazaar" && IsNull(bazaar))
				bazaar = b;
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
	if(!hasvars) {
		Scan(GetFileFolder(GetFileFolder(GetExeFilePath())) + "/SharedSupport/uppsrc");
		Scan(GetFileFolder(GetFileFolder(GetExeFilePath())) + "/SharedSupport/*");
	}
#endif

	String myapps = (DirectoryExists(GetExeDirFile("uppsrc")) ? GetExeDirFile  : GetHomeDirFile)("MyApps");

	for(pass = 0; pass < 2; pass++) {
		if(pass) {
			MakeAssembly(myapps);
			String h = uppsrc;
			uppsrc = bazaar + ';' + uppsrc;
			MakeAssembly(myapps, "MyApps-bazaar");
			uppsrc = h;
		}
	#ifdef PLATFORM_COCOA
		String app = GetAppFolder();
		if(app.GetCount()) {
			String f = GetFileFolder(app);
			Scan(f + "/uppsrc");
			Scan(f + "/*");
			myapps = f + "/MyApps";
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
	}
	

	
	CreateBuildMethods();
	return true;
}

#endif
