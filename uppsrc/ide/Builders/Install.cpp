#include "Builders.h"

#ifdef PLATFORM_POSIX

#ifdef PLATFORM_OSX

const char *clang_bm =
R"(BUILDER = "CLANG";
COMPILER = "clang++";
COMMON_OPTIONS = "$COMMON$";
COMMON_CPP_OPTIONS = "-std=c++17 -Wall -Wno-logical-op-parentheses";
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
DEBUGGER = "lldb";
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
COMMON_CPP_OPTIONS = "-std=c++17";
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
COMMON_OPTIONS = "-mpopcnt";
COMMON_CPP_OPTIONS = "-std=c++17 -Wno-logical-op-parentheses";
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
COMMON_OPTIONS = "$COMMON$";
COMMON_CPP_OPTIONS = "-std=c++17";
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
COMMON_OPTIONS = "$COMMON$";
COMMON_CPP_OPTIONS = "-std=c++17 -Wno-logical-op-parentheses";
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
		
		Path("$INCLUDE$", "/opt/local/include;/usr/include;/usr/local/include;/opt/homebrew/include;/opt/homebrew/opt/openssl/include");
		Path("$LIB$", "/opt/local/lib;/usr/lib;/usr/local/lib;/opt/homebrew/lib;/opt/homebrew/opt/openssl/lib");
		
		String common;
	#ifdef CPU_ARM
		common = "-arch arm64";
	#endif
		bm.Replace("$COMMON$", common);

		SaveFile(bm_path, bm);
	}
#else
	bool openbsd = ToLower(Sys(Host::CMDLINE_PREFIX + "uname")).Find("openbsd") >= 0;
	auto Fix = [=](const char *s) {
		String r = s;
		if(openbsd) {
			r.Replace("INCLUDE = \"\";", "INCLUDE = \"/usr/local/opt/openssl/include\";");
			r.Replace("LIB = \"\";", "LIB = \"/usr/local/opt/openssl/lib\";");
		}
		String common;
	#ifdef CPU_X86
		common = "-mpopcnt";
	#endif
		r.Replace("$COMMON$", common);
		return r;
	};

	String bm = ConfigFile("GCC.bm");
	if(IsNull(LoadFile(bm)))
		SaveFile(bm, Fix(gcc_bm));
	
	if(Sys(Host::CMDLINE_PREFIX + "clang --version").GetCount()) {
		String bm = ConfigFile("CLANG.bm");
		if(IsNull(LoadFile(bm)))
			SaveFile(bm, Fix(clang_bm));
	}
#endif
}

#endif
