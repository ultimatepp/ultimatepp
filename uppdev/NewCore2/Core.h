#ifndef _NewCore_NewCore_h_
#define _NewCore_NewCore_h_

#include <Core/Core.h>

using namespace Upp;

#define String       NewString
#define StringBuffer NewStringBuffer

#define WString      NewWString
#define WStringBuffer NewWStringBuffer

#include "String.h"
#include "WString.h"

void *MAlloc(size_t& size);
void  MFree(void *ptr);

void  MCheck();


#endif
