#include "CtrlLib.h"

namespace Upp {

#define  TFILE <CtrlLib/CtrlLib.t>
#include <Core/t.h>

void CtrlSetDefaultSkin(void (*_skin)());

INITIALIZER(CtrlLib) {
	CtrlSetDefaultSkin(ChHostSkin);

	extern Size (*extGetSmartTextSize)(const char *text, Font font, int cx);
	extGetSmartTextSize = GetSmartTextSize;
	
	extern void (*extDrawSmartText)(Draw& draw, int x, int y, int cx, const char *text, Font font,
                         Color ink, int accesskey, Color qtf_ink);
	extDrawSmartText = DrawSmartText;
};

}
