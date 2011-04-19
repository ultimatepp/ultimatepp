#include "CtrlLibBoostPy.h"

NAMESPACE_UPP

void export_CtrlLib()
{
ONCELOCK
{
	export_Core();

	export_CtrlCore();
	export_SliderCtrl();
	export_ValueCtrl();
}
}

END_UPP_NAMESPACE
