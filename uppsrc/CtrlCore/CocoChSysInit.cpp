#include <CtrlLib/CtrlLib.h>

#ifdef GUI_COCO

NAMESPACE_UPP

void ChSysInit()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();
}

END_UPP_NAMESPACE

#endif
