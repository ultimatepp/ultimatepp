#include "CtrlLibBoostPy.h"

NAMESPACE_UPP

void export_CtrlLib()
{
ONCELOCK
{
	export_CtrlCore();
	export_TopWindow();

	export_EditCtrl();
	export_SliderCtrl();
	export_ValueCtrl();
	export_Progress();

	export_Prompts();
}
}

END_UPP_NAMESPACE
