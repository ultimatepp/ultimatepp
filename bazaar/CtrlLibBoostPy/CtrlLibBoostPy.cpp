#include "CtrlLibBoostPy.h"
using namespace boost::python;

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

	export_EditField();
	export_EditString();
	export_EditInt();
	export_EditInt64();
	export_EditDouble();

	export_SliderCtrl();
	export_Progress();

	export_Prompts();

	export_Foo();

	//FIXME move outside, it's in bazaar
	export_ValueCtrl();
}
}

END_UPP_NAMESPACE
