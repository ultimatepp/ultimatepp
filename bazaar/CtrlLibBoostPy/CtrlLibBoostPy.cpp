#include "CtrlLibBoostPy.h"

NAMESPACE_UPP

void export_CtrlLib()
{
ONCELOCK
{
	export_CtrlCore();
	export_TopWindow();

	export_Pusher();
	export_Button();
	export_ButtonOption();
	export_Option();
	export_Switch();

	export_EditCtrl();
	export_SliderCtrl();
	export_Progress();

	export_Prompts();

	//FIXME move outside, it's in bazaar
	export_ValueCtrl();
}
}

END_UPP_NAMESPACE
