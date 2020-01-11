#include "CtrlLib.h"

namespace Upp {

#define  TFILE <CtrlLib/CtrlLib.t>
#include <Core/t.h>

void CtrlSetDefaultSkin(void (*fn1)(), void (*fn2)());

INITIALIZER(CtrlLib) {
	CtrlSetDefaultSkin(ChBaseSkin, ChHostSkin);
};

}
