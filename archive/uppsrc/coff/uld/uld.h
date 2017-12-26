#ifndef __uld_uld__
#define __uld_uld__

#include <coff/coff.h>

using namespace Upp;

inline dword PeekIL(const void *p) { return Peek32le(p); }
inline word  PeekIW(const void *p) { return Peek16le(p); }

inline void  PokeIL(const void *p, dword d) { return Poke32le(p, d); }
inline void  PokeIW(const void *p, word d)  { return Poke16le(p, d); }

#endif//__uld_uld__
