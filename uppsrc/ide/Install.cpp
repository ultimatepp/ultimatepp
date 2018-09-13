#include "ide.h"
//#include "Install.h"

#ifndef PLATFORM_WIN32

const char *gcc_bm =
R"(BUILDER = "GCC";
COMPILER = "";
COMMON_OPTIONS = "";
COMMON_CPP_OPTIONS = "-std=c++14 -Wno-parentheses";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0 -Wall  -DflagGTK";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "0";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections  -DflagGTK  -DflagGTK";
RELEASE_FLAGS = "";
RELEASE_LINK = "-Wl,--gc-sections";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "/usr/include/freetype2;/usr/include/gtk-2.0;/usr/include/glib-2.0;/usr/lib/glib-2.0/include;/usr/lib/gtk-2.0/include;/usr/include/cairo;/usr/include/pango-1.0;/usr/include/atk-1.0;/usr/X11R6/include;/usr/X11R6/include/freetype2;/usr/X11R6/include/gtk-2.0;/usr/X11R6/include/glib-2.0;/usr/X11R6/lib/glib-2.0/include;/usr/X11R6/lib/gtk-2.0/include;/usr/X11R6/include/cairo;/usr/X11R6/include/pango-1.0;/usr/X11R6/include/atk-1.0;/usr/lib/x86_64-linux-gnu/glib-2.0/include;/usr/lib/x86_64-linux-gnu/gtk-2.0/include;/usr/include/freetype2;/usr/include/gtk-2.0;/usr/include/glib-2.0;/usr/lib/glib-2.0/include;/usr/lib/gtk-2.0/include;/usr/include/cairo;/usr/include/pango-1.0;/usr/include/atk-1.0;/usr/X11R6/include;/usr/X11R6/include/freetype2;/usr/X11R6/include/gtk-2.0;/usr/X11R6/include/glib-2.0;/usr/X11R6/lib/glib-2.0/include;/usr/X11R6/lib/gtk-2.0/include;/usr/X11R6/include/cairo;/usr/X11R6/include/pango-1.0;/usr/X11R6/include/atk-1.0;/usr/include/gdk-pixbuf-2.0;/usr/lib/x86_64-linux-gnu/glib-2.0/include;/usr/lib/x86_64-linux-gnu/gtk-2.0/include";
LIB = "/usr/X11R6/lib;/usr/lib";
LINKMODE_LOCK = "0";)";

bool Install()
{
	String out = GetHomeDirFile("out");
	String ass = GetConfigFolder();
	String myapps = GetHomeDirFile("MyApps");
	RealizeDirectory(out);
	
	String uppsrc = GetHomeDirFile("uppsrc");

	auto MakeAssembly = [&](String b, String name = Null) {
		name = Nvl(name, GetFileTitle(b));
		String a = ass + '/' + name + ".var";
		if(name != "uppsrc")
			b << ';' << uppsrc;
		SaveFile(a,
			"UPP = " + AsCString(b) + ";\r\n"
			"OUTPUT = " + AsCString(out) + ";\r\n"
		);
		LOG("Creating assembly " << a);
	};

	for(FindFile ff(GetHomeDirFile("*")); ff; ff.Next())
		if(ff.IsFolder()) {
			String path = ff.GetPath();
			for(FindFile ff2(path + "/*"); ff2; ff2.Next()) {
				String p = ff2.GetPath();
				if(FileExists(p + '/' + GetFileTitle(p) + ".upp"))
					MakeAssembly(path);
			}
		}
	
	MakeAssembly(myapps);
	uppsrc = GetHomeDirFile("bazaar") + ';' + uppsrc;
	MakeAssembly(myapps, "MyApps-bazaar");
	String bm = ConfigFile("GCC.bm");
	if(IsNull(LoadFile(bm))) {
		LOG("Creating GCC.bm");
		SaveFile(bm, gcc_bm);
	}
	return true;
}

#endif
