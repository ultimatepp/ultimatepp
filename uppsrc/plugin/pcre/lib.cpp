#include <Core/Core.h>

using namespace Upp;

#define SUPPORT_UTF8	
#define SUPPORT_UCP
#define HAVE_CONFIG_H

#define HAVE_STDINT_H 0
#define HAVE_INTTYPES_H 0
#define int64_t int64


#ifdef COMPILER_GCC
#define PCRE_STATIC
#endif

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
#include "lib/pcre_info.c"
#include "lib/pcre_maketables.c"
#include "lib/pcre_newline.c"
#include "lib/pcre_ord2utf8.c"
#include "lib/pcre_refcount.c"
#include "lib/pcre_study.c"
#include "lib/pcre_tables.c"
#include "lib/pcre_try_flipped.c"
#include "lib/pcre_ucd.c"
#include "lib/pcre_valid_utf8.c"
#include "lib/pcre_version.c"
#include "lib/pcre_xclass.c"
