#include "CtrlLib.h"

namespace Upp {

#define  TFILE <CtrlLib/CtrlLib.t>
#include <Core/t.h>

void CtrlSetDefaultSkin(void (*_skin)());

extern Size (*extGetSmartTextSize)(const char *text, Font font, int cx);
extern void (*extDrawSmartText)(Draw& draw, int x, int y, int cx, const char *text, Font font,
                     Color ink, int accesskey, Color qtf_ink);

INITIALIZER(CtrlLib) {
	CtrlSetDefaultSkin(ChHostSkin);
	
	extGetSmartTextSize = GetSmartTextSize;

	extDrawSmartText = DrawSmartText;
};

}
