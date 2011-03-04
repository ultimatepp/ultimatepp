#ifndef OLE_H
#define OLE_H

#include <Core/Core.h>

#if defined(PLATFORM_WIN32) && !defined(PLATFORM_WINCE) && defined(COMPILER_MSC)

#define Ptr Ptr_
#define StdFont StdFont_
#define Font Font_
#define Picture Picture_
#define byte win32_byte_ // RpcNdr defines byte -> class with Upp::byte
#define CY win32_CY_
#include <comdef.h>
#include <comip.h>
#undef Ptr
#undef StdFont
#undef Font
#undef Picture
#undef byte
#undef CY

#endif

#include "util.h"

#endif//OLE_H
