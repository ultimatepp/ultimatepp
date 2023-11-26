#include "Builders.h"

#ifdef PLATFORM_POSIX

#ifdef PLATFORM_OSX

const char *clang_bm =
R"(BUILDER = "CLANG";
COMPILER = "clang++";
COMMON_OPTIONS = "$COMMON$";
COMMON_CPP_OPTIONS = "-std=c++14 -Wall -Wno-logical-op-parentheses";
COMMON_C_OPTIONS = "";
COMMON_LINK = "$COMMON$";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "1";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "$INCLUDE$";
LIB = "$LIB$";
LINKMODE_LOCK = "0";)";

#elif PLATFORM_SOLARIS

const char *gcc_bm =
R"(BUILDER = "GCC";
COMPILER = "";
COMMON_OPTIONS = "";
COMMON_CPP_OPTIONS = "-std=c++14";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "1";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "";
LIB = "";
LINKMODE_LOCK = "0";)";

const char *clang_bm =
R"(BUILDER = "CLANG";
COMPILER = "clang++";
COMMON_OPTIONS = "";
COMMON_CPP_OPTIONS = "-std=c++14 -Wno-logical-op-parentheses";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "1";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "";
LIB = "";
LINKMODE_LOCK = "0";)";

#else

const char *gcc_bm =
R"(BUILDER = "GCC";
COMPILER = "";
COMMON_OPTIONS = "";
COMMON_CPP_OPTIONS = "-std=c++14";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "1";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "-Wl,--gc-sections";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "";
LIB = "";
LINKMODE_LOCK = "0";)";

const char *clang_bm =
R"(BUILDER = "CLANG";
COMPILER = "clang++";
COMMON_OPTIONS = "";
COMMON_CPP_OPTIONS = "-std=c++14 -Wno-logical-op-parentheses";
COMMON_C_OPTIONS = "";
COMMON_LINK = "";
COMMON_FLAGS = "";
DEBUG_INFO = "2";
DEBUG_BLITZ = "1";
DEBUG_LINKMODE = "1";
DEBUG_OPTIONS = "-O0";
DEBUG_FLAGS = "";
DEBUG_LINK = "";
RELEASE_BLITZ = "1";
RELEASE_LINKMODE = "1";
RELEASE_OPTIONS = "-O3 -ffunction-sections -fdata-sections";
RELEASE_FLAGS = "";
RELEASE_LINK = "-Wl,--gc-sections";
DEBUGGER = "gdb";
ALLOW_PRECOMPILED_HEADERS = "0";
DISABLE_BLITZ = "0";
PATH = "";
INCLUDE = "";
LIB = "";
LINKMODE_LOCK = "0";)";

#endif

void CreateBuildMethods()
{
#ifdef PLATFORM_COCOA
	String bm_path = ConfigFile("CLANG.bm");
	if(IsNull(LoadFile(bm_path))) {
		String bm = clang_bm;
		
		auto Path = [&](const char *var, const char *path) {
			String h;
			for(String s : Split(path, ';'))
				if(DirectoryExists(s))
					MergeWith(h, ";", s);
			bm.Replace(var, h);
		};
		
		Path("$INCLUDE$", "/opt/local/include;/usr/include;/opt/homebrew/include;/opt/homebrew/opt/openssl/include");
		Path("$LIB$", "/opt/local/lib;/usr/lib;/opt/homebrew/lib;/opt/homebrew/opt/openssl/lib");
		
		String common;
	#ifdef CPU_ARM
		common = "-arch arm64";
	#endif
		bm.Replace("$COMMON$", common);

		SaveFile(bm_path, bm);
	}
#else
	bool openbsd = ToLower(Sys("uname")).Find("openbsd") >= 0;
	auto Fix = [=](const char *s) {
		String r = s;
		if(openbsd) {
			r.Replace("INCLUDE = \"\";", "INCLUDE = \"/usr/local/opt/openssl/include\";");
			r.Replace("LIB = \"\";", "LIB = \"/usr/local/opt/openssl/lib\";");
		}
		return r;
	};

	String bm = ConfigFile("GCC.bm");
	if(IsNull(LoadFile(bm)))
		SaveFile(bm, Fix(gcc_bm));

	if(Sys("clang --version").GetCount()) {
		String bm = ConfigFile("CLANG.bm");
		if(IsNull(LoadFile(bm)))
			SaveFile(bm, Fix(clang_bm));
	}
#endif
}

String GetDefaultUppOut()
{
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
	return out;
}

#else

String GetDefaultUppOut()
{
	return ConfigFile("_out");
}

#endif
