#ifndef _SDLFb_SDLFbLocal_h
#define _SDLFb_SDLFbLocal_h

#include <CtrlCore/CtrlCore.h>

//has to go second
#include "SDLWrap.h"

NAMESPACE_UPP

extern bool fbEndSession;
void HandleUserEvents(SDL_Event* event);

END_UPP_NAMESPACE

#endif

