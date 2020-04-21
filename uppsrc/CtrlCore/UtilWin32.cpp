#include "CtrlCore.h"

#ifdef GUI_WIN

namespace Upp {

SystemDraw&  ScreenInfo();

bool ScreenInPaletteMode()
{
	return ScreenInfo().PaletteMode();
}

}

#endif
