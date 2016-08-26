#include "Local.h"

#ifdef GUI_TURTLE

namespace Upp {

void ChSysInit()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();
}

void ChHostSkin()
{
	ChSysInit();
}

}

#endif
