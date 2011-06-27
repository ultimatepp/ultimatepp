#ifdef flagSKELETON
#define GUIPLATFORM_INCLUDE <Skeleton/Skeleton.h>
#endif

#ifdef flagWINALT
#define GUIPLATFORM_INCLUDE <WinAlt/WinAlt.h>
#endif

#ifdef flagWINFB
#define GUIPLATFORM_KEYCODES_INCLUDE <WinFb/Keys.h>
#define GUIPLATFORM_INCLUDE <Framebuffer/Framebuffer.h>
#endif