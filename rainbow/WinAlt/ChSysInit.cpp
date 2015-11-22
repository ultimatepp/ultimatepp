#include <CtrlLib/CtrlLib.h>

#ifdef GUI_WINALT

NAMESPACE_UPP

void ChSysInit()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();
}

END_UPP_NAMESPACE

#endif
