#ifndef CORE_H
#define CORE_H

#define QLIB3

#if defined(flagMT)
	#define _MULTITHREADED
	#define MULTITHREADED
	#ifdef flagDLL
		#define flagUSEMALLOC
	#endif
#endif

#ifdef flagDLL
	#define _USRDLL
#endif

#ifdef flagHEAPDBG
	#define HEAPDBG
#endif

#if defined(flagDEBUG)
	#ifndef _DEBUG
		#define _DEBUG
	#endif
	#ifndef TESTLEAKS
		#define TESTLEAKS
	#endif
	#ifndef HEAPDBG
		#define HEAPDBG
	#endif
#else
	#ifndef _RELEASE
		#define _RELEASE
	#endif
#endif

#include "config.h"

#include <typeinfo>
#include <stddef.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>

#if defined(PLATFORM_POSIX)
	#ifndef __USE_FILE_OFFSET64
		#define __USE_FILE_OFFSET64
	#endif
	#define DIR_SEP  '/'
	#define DIR_SEPS "/"
	#define PLATFORM_PATH_HAS_CASE 1

	#include <errno.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/time.h>
	#include <sys/file.h>
	#include <time.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <semaphore.h>
	#include <memory.h>
	#include <dirent.h>
	#include <signal.h>
	#include <syslog.h>
	#include <float.h>
 	#include <fenv.h>
 	#ifdef PLATFORM_SOLARIS
		#include <inttypes.h>
	#else
		#include <stdint.h>
	#endif
#endif //PLATFORM_POSIX

#ifdef PLATFORM_POSIX
#define	LOFF_T_      off_t
#define LSEEK64_     lseek
#define FTRUNCATE64_ ftruncate
#endif

#ifdef PLATFORM_LINUX
#undef  LOFF_T_
#define LOFF_T_      loff_t
#undef  LSEEK64_
#define LSEEK64_     lseek64
#undef  FTRUNCATE64_
#define FTRUNCATE64_ ftruncate64
#endif

#ifdef PLATFORM_WIN32

	#if defined(COMPILER_MSC) && defined(CPU_X86)
		#pragma warning(disable: 4035)
	#else
		#ifndef __NOASSEMBLY__
			#define __NOASSEMBLY__
		#endif
	#endif

	#if defined(COMPILER_MINGW)
		#if !defined(WINVER)
			#define WINVER 0xFFFF
		#endif
		#include <float.h>
	#endif

	#define DIR_SEP  '\\'
	#define DIR_SEPS "\\"
	#define PLATFORM_PATH_HAS_CASE 0
	#ifndef PLATFORM_WINCE
		#include <io.h>
	#endif
	#ifndef PLATFORM_MFC // just mini Windows headers
		#ifdef COMPILER_MSC
			#ifndef CPU_ARM
				#ifndef CPU_AMD64
					#ifndef _X86_
						#define _X86_
					#endif
				#else
					#ifndef _AMD64_
						#define _AMD64_
					#endif
					#ifndef __NOASSEMBLY__
						#define __NOASSEMBLY__
					#endif
					#ifndef WIN64
						#define WIN64
					#endif
				#endif
			#endif
			#ifndef _WINDOWS_
				#define _WINDOWS_
			#endif
			#ifndef _INC_WINDOWS
				#define _INC_WINDOWS
			#endif
			#ifndef _STRUCT_NAME
				#define _STRUCT_NAME(x)
				#define DUMMYSTRUCTNAME
				#define DUMMYSTRUCTNAME2
				#define DUMMYSTRUCTNAME3
			#endif
			#ifndef NO_STRICT
				#ifndef STRICT
					#define STRICT 1
				#endif
			#endif
			#include <stdarg.h>
			#include <windef.h>
			#include <winbase.h>
			#include <wingdi.h>
			#include <winuser.h>
			#include <Wincon.h>
			#include <float.h>
		#define byte win32_byte_ // RpcNdr defines byte -> class with Upp::byte
		#define CY win32_CY_
			#include <objidl.h>
			#include <winnetwk.h>
		#undef byte
		#undef CY
			typedef DWORD LCTYPE;
			#define W_P(w, p) w
			#include <winsock2.h>
			#include <ws2tcpip.h>
			typedef int socklen_t;
		#else
			#define W_P(w, p) w
			#if !defined(PLATFORM_CYGWIN)
			#include <winsock2.h>
			#include <ws2tcpip.h>
			#endif
			typedef int socklen_t;
			#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
			#include <windows.h>
			#include <stdint.h>
		#endif
		#include <process.h>
	#endif

	#ifdef RGBA
		#undef RGBA
	#endif
#endif

#ifdef PLATFORM_POSIX

#define W_P(w, p) p
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
//#include <libiberty.h>
enum
{
	INVALID_SOCKET = -1,
	TCP_NODELAY    = 1,
	SD_RECEIVE     = 0,
	SD_SEND        = 1,
	SD_BOTH        = 2,
};
typedef int SOCKET;
#endif

#ifdef PLATFORM_WIN32
#include <plugin/z/lib/zlib.h>
#else
#include <zlib.h>
#endif

#include <functional>
#include <algorithm>
#include <string>
#include <complex>

// fix MSC8 beta problem....
#ifdef COMPILER_MSC
#ifndef PLATFORM_WINCE
namespace std {
	inline void __cdecl _Debug_message(const wchar_t *, const wchar_t *, unsigned int line) {}
};
#endif
#endif

namespace Upp {};

#ifdef flagNONAMESPACE
#define NAMESPACE_UPP
#define END_UPP_NAMESPACE
#define UPP
#else
#define NAMESPACE_UPP     namespace Upp {
#define END_UPP_NAMESPACE };
#define UPP               Upp
#endif

NAMESPACE_UPP

#include <Core/Defs.h>

class XmlIO;
class JsonIO;

#include "Mt.h"
#include "Global.h"
#include "Topt.h"
#include "Profile.h"
#include "String.h"

#include "CharSet.h"
#include "TimeDate.h"
#include "Path.h"
#include "Stream.h"
#include "Diag.h"

#include "Vcont.h"
#include "BiCont.h"
#include "Index.h"
#include "Map.h"
#include "Tuple.h"
#include "Other.h"
#include "Algo.h"
#include "FixedMap.h"
#include "InVector.h"

#include "SplitMerge.h"

#include "Value.h"
#include "ValueUtil.h"

#include "Color.h"
#include "Complex.h"

#include "Uuid.h"
#include "Ptr.h"

#include "Callback.h"
#include "Util.h"

#include "FilterStream.h"

#include "Format.h"
#include "Convert.h"

#include "z.h"
#include "Hash.h"

#include "Parser.h"
#include "JSON.h"
#include "XML.h"
#include "Xmlize.h"

#include "Gtypes.h"
#include "Lang.h"
#include "i18n.h"
#include "Topic.h"

#include "App.h"

#include "CoWork.h"

#include "LocalProcess.h"

#include "Inet.h"

#include "Win32Util.h"

#include "Vcont.hpp"
#include "Map.hpp"
#include "InVector.hpp"
#include "InMap.hpp"

#if (defined(HEAPDBG) || defined(TESTLEAKS)) && defined(PLATFORM_POSIX)
extern int sMemDiagInitCount;
#endif

#ifdef PLATFORM_WIN32
NTL_MOVEABLE(POINT)
NTL_MOVEABLE(SIZE)
NTL_MOVEABLE(RECT)
#endif

END_UPP_NAMESPACE

#if (defined(TESTLEAKS) || defined(HEAPDBG)) && defined(PLATFORM_POSIX) && !defined(PLATFORM_OSX11) && defined(UPP_HEAP)

//Place it to the begining of each file to be the first function called in whole executable...

//$-
struct MemDiagCls {
	MemDiagCls() { if(!UPP::sMemDiagInitCount++) UPP::MemoryInitDiagnostics(); }
	~MemDiagCls() { if(!--UPP::sMemDiagInitCount) UPP::MemoryDumpLeaks(); }
};
static const MemDiagCls sMemDiagHelper__upp__;
//$+


#endif

//some global definitions

#if !defined(STLPORT) && _MSC_VER < 1600
inline UPP::int64  abs(UPP::int64 x)          { return x < 0 ? -x : x; }
#endif

#ifdef COMPILER_GCC
inline double abs(double x)                   { return fabs(x); }
#endif

void      RegisterTopic__(const char *topicfile, const char *topic, const char *title, const UPP::byte *data, int len);

#ifdef PLATFORM_WIN32
typedef HMODULE DLLHANDLE;
#else
typedef void   *DLLHANDLE;
#endif

DLLHANDLE LoadDll__(UPP::String& fn, const char *const *names, void *const *procs);
void      FreeDll__(DLLHANDLE dllhandle);

#ifndef flagNONAMESPACE
using Upp::byte; // Dirty solution to Windows.h typedef byte...
#endif

#ifdef PLATFORM_WIN32
#define DLLFILENAME "Kernel32.dll"
#define DLIMODULE   UnicodeWin32
#define DLIHEADER   <Core/Kernel32W.dli>
#include <Core/dli_header.h>

#define DLLFILENAME "Mpr.dll"
#define DLIMODULE   UnicodeWin32Net
#define DLIHEADER   <Core/Mpr32W.dli>
#include <Core/dli_header.h>
#endif

#ifdef MAIN_CONF
#include <main.conf.h>
#endif

#endif //CORE_H
