#include "CtrlLib.h"

namespace Upp {

#define  TFILE <CtrlLib/CtrlLib.t>
#include <Core/t.h>

void CtrlSetDefaultSkin(void (*_skin)());

INITIALIZER(CtrlLib) {
	CtrlSetDefaultSkin(ChHostSkin);
};

}
