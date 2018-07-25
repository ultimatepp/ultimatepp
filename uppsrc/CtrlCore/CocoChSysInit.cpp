#include <CtrlLib/CtrlLib.h>

#ifdef PLATFORM_COCOA

NAMESPACE_UPP

void ChSysInit()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();
}

END_UPP_NAMESPACE

#endif
