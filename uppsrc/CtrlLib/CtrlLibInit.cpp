#include "CtrlLib.h"

namespace Upp {

#define  TFILE <CtrlLib/CtrlLib.t>
#include <Core/t.h>

void CtrlSetDefaultSkin(void (**_skin)());

extern Size (*extGetSmartTextSize)(const char *text, Font font, int cx);
extern void (*extDrawSmartText)(Draw& draw, int x, int y, int cx, const char *text, Font font,
                     Color ink, int accesskey, Color qtf_ink);

INITIALIZER(CtrlLib) {
	static void (*skin[3])() = {
		ChHostSkin,
		ChStdSkin,
		ChDarkSkin,
	};
	CtrlSetDefaultSkin(skin);
	
	extGetSmartTextSize = GetSmartTextSize;

	extDrawSmartText = [](Draw& draw, int x, int y, int cx, const char *text, Font font,
                          Color ink, int accesskey, Color qtf_ink) {
		DrawSmartText(draw, x, y, cx, text, font, ink, accesskey, qtf_ink);
	};
};

}
