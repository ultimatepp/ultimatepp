#include <CtrlCore/CtrlCore.h>

#ifdef GUI_SKELETON

//#include <winnls.h>

//#include "imm.h"

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

bool  GetShift() { return false; }
bool  GetCtrl() { return false; }
bool  GetAlt() { return false; }
bool  GetCapsLock() { return false; }
bool  GetMouseLeft() { return false; }
bool  GetMouseRight() { return false; }
bool  GetMouseMiddle() { return false; }
Point GetMousePos() { return Point(0, 0); }

END_UPP_NAMESPACE

#endif
