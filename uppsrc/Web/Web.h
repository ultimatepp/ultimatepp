#ifndef WEB_H
#define WEB_H

#include <Core/Core.h>
//#include <RichText/RichText.h>

#if defined(PLATFORM_WIN32)
#define W_P(w, p) w
#if !defined(PLATFORM_CYGWIN)
#include <winsock2.h>
#endif
typedef int socklen_t;
#elif defined(PLATFORM_POSIX)
#define W_P(w, p) p
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
//#include <libiberty.h>
#else
#error Unsupported platform
#endif//PLATFORM switch

NAMESPACE_UPP

#include "util.h"
#include "html.h"
//#include "RichHtml.h"
#include "socket.h"
#include "httpsrv.h"
#include "httpcli.h"
#include "auth.h"
#include "smtp.h"
#include "sproc.h"
#include "ScgiServer.h"

END_UPP_NAMESPACE

#endif
