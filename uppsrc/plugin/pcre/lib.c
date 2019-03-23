#define SUPPORT_UTF
#define SUPPORT_UTF8	//is needed anymore?
#define SUPPORT_UCP
#define HAVE_CONFIG_H

////#define HAVE_STDINT_H 0
////#define HAVE_INTTYPES_H 0
//#undef HAVE_STDINT_H
//#undef HAVE_INTTYPES_H

#ifdef _MSC_VER

typedef __int64            int64;
typedef unsigned __int64   uint64;

#else

typedef long long int      int64;
typedef long long unsigned uint64;

#define PCRE_STATIC

#endif

#include "lib/pcre_byte_order.c"
#include "lib/pcre_chartables.c"
#include "lib/pcre_compile.c"
#undef PSSTART
#undef PSEND
#undef NLBLOCK
#include "lib/pcre_config.c"
#include "lib/pcre_dfa_exec.c"
#include "lib/pcre_exec.c"
#include "lib/pcre_fullinfo.c"
#include "lib/pcre_get.c"
#include "lib/pcre_globals.c"
#include "lib/pcre_jit_compile.c"
#include "lib/pcre_maketables.c"
#include "lib/pcre_newline.c"
#include "lib/pcre_ord2utf8.c"
#include "lib/pcre_refcount.c"
#include "lib/pcre_string_utils.c"
#include "lib/pcre_study.c"
#include "lib/pcre_tables.c"
#include "lib/pcre_ucd.c"
#include "lib/pcre_valid_utf8.c"
#include "lib/pcre_version.c"
#include "lib/pcre_xclass.c"
