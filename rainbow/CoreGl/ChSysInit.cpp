#if defined(flagWINGL) || defined(flagLINUXGL)

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

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

END_UPP_NAMESPACE

#endif
