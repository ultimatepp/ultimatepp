#ifndef _SDL_SDLWrapper_h_
#define _SDL_SDLWrapper_h_

#ifdef COMPILER_MSC
	#include <SDL.h> 
	#include <SDL_thread.h> 
	#include <SDL_syswm.h>
#endif

#ifdef COMPILER_GCC
	#include <SDL/SDL.h>
	#include <SDL/SDL_thread.h>
	#include <SDL/SDL_syswm.h>
#endif

#define INT64_C(val) val##LL
#define UINT64_C(val) val##ULL 

#endif
