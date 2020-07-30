#ifndef _plugin_glad_Glad_h_
#define _plugin_glad_Glad_h_
#include "include/glad/glad.h"
#if defined(__linux__)
#include "include/glad/glad_glx.h"
#elif defined(_WIN32)
#include "include/glad/glad_wgl.h"
#endif


#endif
