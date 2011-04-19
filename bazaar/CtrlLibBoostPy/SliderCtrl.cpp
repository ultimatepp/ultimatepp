#include "SliderCtrl.h"
using namespace boost::python;

NAMESPACE_UPP

void export_SliderCtrl()
{
ONCELOCK
{
	scope().attr("__doc__") = "SliderCtrl's docstring";
	class_<SliderCtrl, bases<Ctrl>, SliderCtrl, boost::noncopyable>("SliderCtrl", "An Upp SliderCtrl", no_init)
		.def("inc", &SliderCtrl::Inc)
		.def("dec", &SliderCtrl::Dec)
		.def("step", &SliderCtrl::GetStep)
	;
}
}

END_UPP_NAMESPACE
