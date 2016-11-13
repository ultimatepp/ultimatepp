#include "CtrlLib.h"

namespace Upp {

#define  TFILE <CtrlLib/CtrlLib.t>
#include <Core/t.h>

void CtrlSetDefaultSkin(void (*fn1)(), void (*fn2)());

INITIALIZER(CtrlLib) {
#if defined(GUI_WIN) || defined(PLATFORM_X11)
	CtrlSetDefaultSkin(ChStdSkin, ChHostSkin);
#else
	CtrlSetDefaultSkin(ChStdSkin, ChStdSkin);
#endif
};

}
