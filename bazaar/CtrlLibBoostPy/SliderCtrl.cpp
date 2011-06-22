#include "Common.h"
using namespace boost::python;

NAMESPACE_UPP

void export_SliderCtrl()
{
ONCELOCK
{
	class_<SliderCtrl, bases<Ctrl>, boost::noncopyable>("SliderCtrl", "Upp SliderCtrl")
		.def("inc", &SliderCtrl::Inc)
		.def("dec", &SliderCtrl::Dec)
		.def("step", &SliderCtrl::GetStep)
	;
}
}

END_UPP_NAMESPACE
