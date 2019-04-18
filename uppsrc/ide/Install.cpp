#include "ide.h"
//#include "Install.h"

#ifndef PLATFORM_WIN32

#ifdef PLATFORM_OSX

const char *clang_bm =
R"(BUILDER = "CLANG";
COMPILER = "clang++";
COMMON_OPTIONS = "-mmacosx-version-min=10.13";
COMMON_CPP_OPTIONS = "-std=c++14 -Wall -Wno-logical-op-parentheses";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "0";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "/opt/local/include;/usr/include";
LIB = "/opt/local/lib;/usr/lib";
LINKMODE_LOCK = "0";)";

#else

const char *gcc_bm =
R"(BUILDER = "GCC";
COMPILER = "";
COMMON_OPTIONS = "";
COMMON_CPP_OPTIONS = "-std=c++14 -Wall -Wno-parentheses -Wno-maybe-uninitialized -Wno-strict-overflow";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "0";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "-Wl,--gc-sections";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "/usr/include/freetype2;/usr/include/gtk-2.0;/usr/include/glib-2.0;/usr/lib/glib-2.0/include;/usr/lib/gtk-2.0/include;/usr/include/cairo;/usr/include/pango-1.0;/usr/include/atk-1.0;/usr/X11R6/include;/usr/X11R6/include/freetype2;/usr/X11R6/include/gtk-2.0;/usr/X11R6/include/glib-2.0;/usr/X11R6/lib/glib-2.0/include;/usr/X11R6/lib/gtk-2.0/include;/usr/X11R6/include/cairo;/usr/X11R6/include/pango-1.0;/usr/X11R6/include/atk-1.0;/usr/lib/x86_64-linux-gnu/glib-2.0/include;/usr/lib/x86_64-linux-gnu/gtk-2.0/include;/usr/include/freetype2;/usr/include/gtk-2.0;/usr/include/glib-2.0;/usr/lib/glib-2.0/include;/usr/lib/gtk-2.0/include;/usr/include/cairo;/usr/include/pango-1.0;/usr/include/atk-1.0;/usr/X11R6/include;/usr/X11R6/include/freetype2;/usr/X11R6/include/gtk-2.0;/usr/X11R6/include/glib-2.0;/usr/X11R6/lib/glib-2.0/include;/usr/X11R6/lib/gtk-2.0/include;/usr/X11R6/include/cairo;/usr/X11R6/include/pango-1.0;/usr/X11R6/include/atk-1.0;/usr/include/gdk-pixbuf-2.0;/usr/lib/x86_64-linux-gnu/glib-2.0/include;/usr/lib/x86_64-linux-gnu/gtk-2.0/include;/usr/include/gtkglext-1.0;/usr/lib/gtkglext-1.0/include";
LIB = "/usr/X11R6/lib;/usr/lib";
LINKMODE_LOCK = "0";)";

const char *clang_bm =
R"(BUILDER = "GCC";
COMPILER = "clang++";
COMMON_OPTIONS = "";
COMMON_CPP_OPTIONS = "-std=c++14 -Wno-logical-op-parentheses";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0 -Wall  -Wno-logical-op-parentheses -DflagGTK";
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
INCLUDE = "/usr/include/freetype2;/usr/include/gtk-2.0;/usr/include/glib-2.0;/usr/lib/glib-2.0/include;/usr/lib/gtk-2.0/include;/usr/include/cairo;/usr/include/pango-1.0;/usr/include/atk-1.0;/usr/X11R6/include;/usr/X11R6/include/freetype2;/usr/X11R6/include/gtk-2.0;/usr/X11R6/include/glib-2.0;/usr/X11R6/lib/glib-2.0/include;/usr/X11R6/lib/gtk-2.0/include;/usr/X11R6/include/cairo;/usr/X11R6/include/pango-1.0;/usr/X11R6/include/atk-1.0;/usr/lib/x86_64-linux-gnu/glib-2.0/include;/usr/lib/x86_64-linux-gnu/gtk-2.0/include;/usr/include/freetype2;/usr/include/gtk-2.0;/usr/include/glib-2.0;/usr/lib/glib-2.0/include;/usr/lib/gtk-2.0/include;/usr/include/cairo;/usr/include/pango-1.0;/usr/include/atk-1.0;/usr/X11R6/include;/usr/X11R6/include/freetype2;/usr/X11R6/include/gtk-2.0;/usr/X11R6/include/glib-2.0;/usr/X11R6/lib/glib-2.0/include;/usr/X11R6/lib/gtk-2.0/include;/usr/X11R6/include/cairo;/usr/X11R6/include/pango-1.0;/usr/X11R6/include/atk-1.0;/usr/include/gdk-pixbuf-2.0;/usr/lib/x86_64-linux-gnu/glib-2.0/include;/usr/lib/x86_64-linux-gnu/gtk-2.0/include;/usr/include/gtkglext-1.0;/usr/lib/gtkglext-1.0/include";
LIB = "/usr/X11R6/lib;/usr/lib";
LINKMODE_LOCK = "0";)";

#endif

bool Install(bool& hasvars)
{
	String out = GetHomeDirFile("out");
	String ass = GetConfigFolder();
	String myapps = GetHomeDirFile("MyApps");
	RealizeDirectory(out);

	String uppsrc;

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
#ifdef PLATFORM_COCOA
	String bm = ConfigFile("CLANG.bm");
	if(IsNull(LoadFile(bm)))
		SaveFile(bm, clang_bm);
#else
	String bm = ConfigFile("GCC.bm");
	if(IsNull(LoadFile(bm)))
		SaveFile(bm, gcc_bm);

	if(Sys("clang --version").GetCount()) {
		String bm = ConfigFile("CLANG.bm");
		if(IsNull(LoadFile(bm)))
			SaveFile(bm, clang_bm);
	}

#endif
	return true;
}

#endif
