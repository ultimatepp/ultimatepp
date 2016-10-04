#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	Font fnt = StdFont();
	DUMP(fnt.Info().GetOverhang());
	DUMP(fnt.Info().GetLeftSpace('x'));
	DUMP(fnt.Info().GetRightSpace('x'));
}
