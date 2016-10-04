#ifndef _NewCore_NewCore_h_
#define _NewCore_NewCore_h_

#include <Core/Core.h>

using namespace Upp;

#define String       NewString
#define StringBuffer NewStringBuffer

#define WString      NewWString
#define WStringBuffer NewWStringBuffer

inline int cmpval__(char x)         { return (byte)x; }
inline int cmpval__(wchar x)        { return (word)x; }
//inline int strlen__(const char *s)  { return strlen(s); }

#include "AString.h"
#include "AString.hpp"

class WString;

#include "String.h"
#include "WString.h"

void *MAlloc(size_t& size);
void  MFree(void *ptr);

void  MCheck();



void IdMapBench();

#endif
