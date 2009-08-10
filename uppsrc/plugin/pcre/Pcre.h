#ifndef _plugin_pcre_pcre_h_
#define _plugin_pcre_pcre_h_

#include <Core/Core.h>

#ifdef PLATFORM_WIN32
#define PCRE_EXP_DECL extern 
#endif

#include "lib/pcre.h"

NAMESPACE_UPP

#include "RegExp.h"

END_UPP_NAMESPACE

#endif
