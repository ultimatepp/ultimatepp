#include "CtrlLibBoostPy.h"

NAMESPACE_UPP

void export_CtrlLib()
{
ONCELOCK
{
	export_CtrlCore();
	export_EditCtrl();

	export_SliderCtrl();
	export_ValueCtrl();
}
}

END_UPP_NAMESPACE
