#include "CtrlLib.h"

namespace Upp {

#define  TFILE <CtrlLib/CtrlLib.t>
#include <Core/t.h>

INITIALIZER(CtrlLib) {
	Ctrl::SetSkin(ChHostSkin);
};

}
