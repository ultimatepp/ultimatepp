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
#define GUIPLATFORM_INCLUDE          <LinuxFb/LinuxFb.h>
#define FRAMEBUFFER_INCLUDE          <LinuxFb/Local.h>
#endif

#ifdef flagSDLFB
#define GUIPLATFORM_KEYCODES_INCLUDE <SDLFb/Keys.h>
//need to make SDL_keysym.h known before K_ enum
#define GUIPLATFORM_INCLUDE          <SDLFb/SDLFb.h>
#define FRAMEBUFFER_INCLUDE          <SDLFb/Local.h>
#endif

#ifdef flagWINGL
#define GUIPLATFORM_KEYCODES_INCLUDE <WinGl/Keys.h>
#define GUIPLATFORM_INCLUDE          <WinGl/WinGl.h>
#endif
