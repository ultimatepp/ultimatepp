#include "Local.h"

#ifdef VIRTUALGUI

NAMESPACE_UPP

void ChSysInit()
{
	ChReset();
}

void ChHostSkin()
{
	ChSysInit();
}

END_UPP_NAMESPACE

#endif
