#include <CtrlLib/CtrlLib.h>

#ifdef GUI_WINGL

NAMESPACE_UPP

void ChSysInit()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();
}

END_UPP_NAMESPACE

#endif
