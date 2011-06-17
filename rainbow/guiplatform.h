#ifdef flagSKELETON
#define GUIPLATFORM_INCLUDE <Skeleton/Skeleton.h>
#endif

#ifdef flagWINALT
#define GUIPLATFORM_INCLUDE <WinAlt/WinAlt.h>
#endif

#ifdef flagFB
#define GUIPLATFORM_INCLUDE <Framebuffer/Framebuffer.h>
#endif
