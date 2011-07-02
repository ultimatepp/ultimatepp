#ifdef flagSKELETON
#define GUIPLATFORM_INCLUDE <Skeleton/Skeleton.h>
#endif

#ifdef flagWINALT
#define GUIPLATFORM_INCLUDE <WinAlt/WinAlt.h>
#endif

#ifdef flagWINFB
#define GUIPLATFORM_KEYCODES_INCLUDE <WinFb/Keys.h>
#define GUIPLATFORM_INCLUDE          <Framebuffer/Framebuffer.h>
#define FRAMEBUFFER_INCLUDE          <WinFb/Local.h>
#endif

#ifdef flagLINUXFB
#define GUIPLATFORM_KEYCODES_INCLUDE <LinuxFb/Keys.h>
#define GUIPLATFORM_INCLUDE          <Framebuffer/Framebuffer.h>
#define FRAMEBUFFER_INCLUDE          <LinuxFb/Local.h>
#endif

#ifdef flagWINGL
#define GUIPLATFORM_INCLUDE <WinGl/WinGl.h>
#endif
