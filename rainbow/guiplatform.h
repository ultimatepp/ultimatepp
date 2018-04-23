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

#ifdef flagLINUXGL
#define GUIPLATFORM_KEYCODES_INCLUDE <LinuxGl/Keys.h>
#define GUIPLATFORM_INCLUDE          <LinuxGl/LinuxGl.h>
#endif

#ifdef flagSDL20
#define GUIPLATFORM_KEYCODES_INCLUDE <SDL20/Keys.h>
//need to make SDL_keysym.h known before K_ enum
#define GUIPLATFORM_INCLUDE          <SDL20/SDL20.h>
#endif

#ifdef flagSDL20GL
#define GUIPLATFORM_KEYCODES_INCLUDE <SDL20GL/Keys.h>
//need to make SDL_keysym.h known before K_ enum
#define GUIPLATFORM_INCLUDE          <SDL20GL/SDL20GL.h>
#endif

#ifdef flagVIRTUALGUI
#define GUIPLATFORM_KEYCODES_INCLUDE <VirtualGui/Keys.h>
//need to make SDL_keysym.h known before K_ enum
#define GUIPLATFORM_INCLUDE          <VirtualGui/VirtualGui.h>
#endif

#ifdef flagTELPP
#define GUIPLATFORM_KEYCODES_INCLUDE <Telpp/Keys.h>
//need to make SDL_keysym.h known before K_ enum
#define GUIPLATFORM_INCLUDE          <Telpp/Telpp.h>
#endif

#ifdef flagTURTLE
#define GUIPLATFORM_KEYCODES_INCLUDE <Turtle/Keys.h>
//need to make SDL_keysym.h known before K_ enum
#define GUIPLATFORM_INCLUDE          <Turtle/Turtle.h>
#define GUIPLATFORM_NOSCROLL
#endif

#ifdef flagSDL2GL
#define GUIPLATFORM_KEYCODES_INCLUDE <SDL2GL/Keys.h>
//need to make SDL_keysym.h known before K_ enum
#define GUIPLATFORM_INCLUDE          <SDL2GL/SDL2GL.h>
#endif
