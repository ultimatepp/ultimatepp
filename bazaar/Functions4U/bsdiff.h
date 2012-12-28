#ifndef _Functions4U_bsdiff_h_
#define _Functions4U_bsdiff_h_

#define ftello 	ftell
#define fseeko 	fseek

NAMESPACE_UPP

extern String errMsg;
String BsGetLastError();
bool Err(String str);

#if defined(_WINDOWS_) || defined(__MINGW32__)
	#include <io.h>
	#include <share.h>
	#include <sys/stat.h>
	
	#define lseek 	_lseek
	#define read 	_read	
	#define write 	_write	
	#define close 	_close		
	typedef unsigned char u_char;

	#define _SH_DENYNO 0x40 	
	#if !defined(__MINGW32__)
		typedef long pid_t;
		typedef signed int ssize_t;
	#endif
#endif

END_UPP_NAMESPACE

#endif
